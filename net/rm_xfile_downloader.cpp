//
//  rm_xfile_downloader.cpp
//  KingFile
//
//  Created by ks on 15/12/30.
//  Copyright (c) 2015年 ks. All rights reserved.
//

#include "rm_xfile_downloader.h"
#include "http_request.h"

#include "../Path/BoostPath.h"
#include "../../Misc/FileHelper.h"
#include "JsonCpp/Json.h"
#include "http_request.h"
#include "../EncodeFile/block_data.h"
#include "../EncodeFile/CBlockMerger.h"
#include "host/dynamic_host_holderimpl.h"
#include "libxlive/CommonDefinition.h"
#include <map>

struct block_stub
{
    unsigned int id;
    string stub;
    string sha1;
    string md5;
};

struct file_stub
{
    typedef map<unsigned int, block_stub>::const_iterator block_stub_const_iterator;
    
    void insert_block_stub(block_stub& bstub)
    {
        block_stubs.insert(map<unsigned int, block_stub>::value_type(bstub.id,bstub));
    }
    
    block_stub_const_iterator bstub_begin() const
    {
        return block_stubs.begin();
    }
    block_stub_const_iterator bstub_end() const
    {
        return block_stubs.end();
    }
    UInt64 xid;
    UInt64 version;
    unsigned int block_max_id;
    map<unsigned int, block_stub> block_stubs;
    
};

rm_xfile_downloader::rm_xfile_downloader(const std::string& token, std::shared_ptr<download_file_table_data> sp_file_data, const std::string& block_temp_folder)
: m_sp_file_data(sp_file_data)
, m_token(token)
, m_tf_complete_total_size(0)
, m_block_temp_folder(block_temp_folder)
, m_file_save_path(sp_file_data->m_path)
{
    m_dp_params_factory.m_xid = sp_file_data->m_xid;
    m_dp_params_factory.m_token = m_token;
    m_dp_params_factory.m_file_ver = sp_file_data->m_version;
}

rm_xfile_downloader::~rm_xfile_downloader()
{

}

ErrorNo rm_xfile_downloader::download()
{
    ErrorNo ret = XLIVE_OK;
    string prepare_recv_data;
    
    if(is_cancle())
        return TF_USER_CANCLE;
    
    ret = download_prepare(prepare_recv_data);
    if(ret != XLIVE_OK)
        return ret;

    file_stub fstub;
    ret = parse_prepare_file_data(prepare_recv_data, fstub);
    
    if(ret != XLIVE_OK)
        return ret;
    
    ret = download_blocks(fstub);
    return ret;
}

void rm_xfile_downloader::set_download_prepare_url(const string& url)
{
    m_prepare_url = url;
}
void rm_xfile_downloader::set_download_block_url(const string& url)
{
    m_download_block_url = url;
}
void rm_xfile_downloader::set_progress_cmd(std::function<void(double, double, double)> cmd)
{
    m_progress_cmd = cmd;
}

ErrorNo rm_xfile_downloader::download_prepare(string& recv_data)
{
    ErrorNo re = XLIVE_OK;
    string post_data = m_dp_params_factory.get_data();
    http_post_request dp_request(m_prepare_url, m_dp_params_factory.get_data());
    dp_request.enable_https(false);
    re = dp_request.perform();
    if(re == XLIVE_OK)
        recv_data = dp_request.get_result();
    return re;
}

ErrorNo rm_xfile_downloader::parse_prepare_file_data(const string& prepare_data,file_stub& fstub)
{
    ErrorNo ret = XLIVE_OK;
    Json::Value jobj;
    Json::Reader().parse(prepare_data, jobj);
    int code = jobj["code"].asInt();
    if(code != 0)
        return ret = (ErrorNo)code;
    else
    {
        Json::Value jdata = jobj["data"];
        fstub.xid = jdata["xid"].asUInt64();
        fstub.version = jdata["file_version"].asUInt64();
        fstub.block_max_id = jdata["block_max"].asUInt();
        Json::Value jblock_stubs = jdata["stub"];
        for(int i = 0; i < jblock_stubs.size(); ++i)
        {
            block_stub bstub;
            bstub.id = jblock_stubs[i]["i"].asUInt();
            bstub.stub = jblock_stubs[i]["t"].asString();
            bstub.sha1 = jblock_stubs[i]["s"].asString();
            bstub.md5 = jblock_stubs[i]["w"].asString();
            fstub.insert_block_stub(bstub);
        }
    }
    return  ret;
}

ErrorNo rm_xfile_downloader::download_blocks(const file_stub& fstub)
{
    ErrorNo ret = XLIVE_OK;
    CBlockMerger bmeger(m_block_temp_folder,m_sp_file_data->m_file_size);
    file_stub::block_stub_const_iterator it = fstub.bstub_begin();
    file_stub::block_stub_const_iterator it_end = fstub.bstub_end();
    
    for(; it != it_end; ++it)
    {
        m_db_params_factory.m_token = m_token;
        m_db_params_factory.m_id = it->second.id;
        m_db_params_factory.m_stub = it->second.stub;
        m_db_params_factory.m_xid = fstub.xid;
        
        CBoostPath block_path = format_block_temp_path(m_block_temp_folder, it->second, fstub.xid);
        ret = download_block(block_path, m_db_params_factory);
        
        unsigned int block_size = (it->second.id == m_sp_file_data->m_file_size/BLOCK_SIZE)?m_sp_file_data->m_file_size%BLOCK_SIZE:BLOCK_SIZE;
        if(ret == XLIVE_OK)
        {
            if(ret == XLIVE_OK)
                m_tf_complete_total_size += block_size;
            bmeger.push(block_path);
        }else
        {
            if(is_cancle())
                ret = TF_USER_CANCLE;
            break;
        }
    }
    if(ret == XLIVE_OK)
        bmeger.mergeTo(m_file_save_path);
    return  ret;
}
ErrorNo rm_xfile_downloader::download_block(const CBoostPath& block_path, PARAMS_FACTORY::download_block_params_factory& params_factory)
{
    ErrorNo ret = XLIVE_OK;
    http_download_post_request request(block_path, m_download_block_url, params_factory.get_data());
    request.set_progress_cmd(
        std::bind(&rm_xfile_downloader::update_progress,
                  this,std::placeholders::_1,
                  std::placeholders::_2,
                  std::placeholders::_3,
                  std::placeholders::_4,
                  std::placeholders::_5));
    request.set_cancle_check_cmd(m_cancle_check_cmd);
    ret = request.download();
    return  ret;
}

CBoostPath rm_xfile_downloader::format_block_temp_path(const string& temp_folder, const block_stub& bstub, UInt64 xid)
{
    CBoostPath block_path(temp_folder);
    string file_name = StringHelper::ui64ToString(xid);
    file_name += "_";
    file_name += StringHelper::intToString(bstub.id);
    file_name += "_";
    file_name += bstub.sha1;
    file_name += ".done";
    block_path.append(file_name);
    return block_path;
}

void rm_xfile_downloader::update_progress(double tf_dtotal, double tf_dnow, double tf_utotal, double tf_unow, double speed)
{
    if(m_progress_cmd)
    {
        size_t curent_time = time(0);
        if(able_to_update_progress(curent_time))
        {
            speed = calculate_speed(m_tf_complete_total_size+tf_dnow, curent_time);
            m_progress_cmd(m_tf_complete_total_size+tf_dnow, tf_dnow, speed);
        }
        
    }
}

void rm_xfile_downloader::set_cancle_check_cmd(const std::function<bool (void)>& cmd)
{
    m_cancle_check_cmd = cmd;
}

bool rm_xfile_downloader::is_cancle()
{
    if(m_cancle_check_cmd)
        return m_cancle_check_cmd();
    return false;
}

uint64_t rm_xfile_downloader::calculate_speed(double tf_total_size, size_t curent_time)
{
    double tf_size = tf_total_size - m_last_tf_total_size;
    uint64_t speed = 0;
    if(tf_size > 0)
    {
        speed = tf_size/(curent_time - m_last_update_progress_time);
    }
    m_last_tf_total_size = tf_total_size;
    m_last_update_progress_time = curent_time;
    return speed;
}

bool rm_xfile_downloader::able_to_update_progress(size_t curent_time)
{
    if(curent_time - m_last_update_progress_time >= 1)
        return true;
    return false;
}







