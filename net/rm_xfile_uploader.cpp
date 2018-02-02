//
//  rm_xfile_uploader.cpp
//  KingFile
//
//  Created by ks on 15/12/28.
//  Copyright (c) 2015年 ks. All rights reserved.
//

#include "rm_xfile_uploader.h"
#include "http_request.h"

#include "../Path/BoostPath.h"
#include "../../Misc/FileHelper.h"
#include "JsonCpp/Json.h"
#include "http_request.h"
#include "../EncodeFile/block_data.h"
#include "host/dynamic_host_holderimpl.h"


rm_xfile_uploader::rm_xfile_uploader(const std::string& token, std::shared_ptr<upload_file_table_data> sp_file_data, const std::string& block_temp_folder)
: m_sp_file_data(sp_file_data)
, m_token(token)
, m_block_generator(block_temp_folder)
, m_tf_complete_total_size(0)
{
    m_uc_params_factory.m_name = m_sp_file_data->m_name;
    m_uc_params_factory.m_token = m_token;
    m_uc_params_factory.m_size = m_sp_file_data->m_file_size;
    m_uc_params_factory.m_xid = m_sp_file_data->m_p_xid;
    m_last_update_progress_time = time(0);
}

rm_xfile_uploader::~rm_xfile_uploader()
{
}


ErrorNo rm_xfile_uploader::upload()
{
    ErrorNo ret = XLIVE_OK;
    string create_recv_data;
    ret = upload_create(create_recv_data);
    if(ret != XLIVE_OK)
    {
        cout << "create error"<<endl;
        return ret;
    }
    
    ret = parse_create_file_data(m_up_create_recv_data, m_file_block);
    make_upload_xfile();
    if(ret != XLIVE_OK)
    {
        cout << "parse create file recv data error"<<endl;
        return ret;
    }
    
    ret = upload_blocks(m_file_block);
    if(ret != XLIVE_OK)
    {
        cout << "upload bolck error"<<endl;
        return ret;
    }
    

    ret = commit();

    return ret;
}
ErrorNo rm_xfile_uploader::upload_create(string& recv_data)
{
    ErrorNo ret = XLIVE_OK;
    
    m_block_generator.setFile(m_sp_file_data->m_local_path);
    ret = m_block_generator.hashBlock();
    m_uc_params_factory.m_blockinfo_list = m_block_generator.getBlocksInfo();
    m_file_sha1 = m_block_generator.getFileSha1();
    m_uc_params_factory.m_sha1 = m_file_sha1;
    
    
    http_post_request request(m_create_url, m_uc_params_factory.get_data());
    request.enable_https(false);
    ret = request.perform();
    recv_data = request.get_result();
    m_up_create_recv_data = recv_data;
    return  ret;
}

void rm_xfile_uploader::set_upload_create_url(const string& url)
{
    m_create_url = url;
}
void rm_xfile_uploader::set_upload_block_url(const string& url)
{
    m_upload_block_url = url;
}

ErrorNo rm_xfile_uploader::parse_create_file_data(const string& recv_data, STORAGE::file_block_info& file_block)
{
    ErrorNo ret = XLIVE_OK;
    Json::Value jobj;
    Json::Reader().parse(recv_data, jobj);
    int code = jobj["code"].asInt();
    if(code != 0)
        return ErrorNo(code);
    Json::Value jdata = jobj["data"];
    //STORAGE::file_block_info file_block_info;
    file_block.file_version = jdata["file_version"].asUInt64();
    file_block.xid = jdata["xid"].asUInt64();
    
    Json::Value jstubs = jdata["stub"];
    for(int i = 0; i < jstubs.size(); ++i)
    {
        STORAGE::block_info block;
        block.id = jstubs[i]["i"].asInt();
        block.t = jstubs[i]["t"].asString();
        file_block.block_list.push_back(block);
    }
    
    return ret;
}

ErrorNo rm_xfile_uploader::upload_blocks(const STORAGE::file_block_info& file_block)
{
    ErrorNo ret = XLIVE_OK;
    size_t block_size = file_block.block_list.size();
    for(int i = 0; i < block_size; ++i)
    {
        int block_id = file_block.block_list[i].id;
        unsigned int org_block_size = m_block_generator.getOrgBlockSize(i);
        
        CBoostPath block_path;
        unsigned int encode_block_size = 0;
        ret = m_block_generator.generate_block(block_id,block_path, encode_block_size);
        if(ret != XLIVE_OK)
        {
            cout << "cut block error"<<endl;
            break;
        }
        
        
        ret = upload_block(block_path, i);
        if(ret == XLIVE_OK)
            m_tf_complete_total_size += org_block_size;
        else
        {
            if(is_cancle())
                ret = TF_USER_CANCLE;
            break;
        }
    }
    return ret;
}

ErrorNo rm_xfile_uploader::upload_block(const CBoostPath& block_path, int index)
{
    ErrorNo ret = XLIVE_OK;
    string block_sha1;
    string block_md5;
    FileHelper::getFileSha1(block_path, block_sha1);
    FileHelper::getFileMd5(block_path, block_md5);
    m_file_block.block_list[index].sha1 = block_sha1;
    m_file_block.block_list[index].md5 = block_md5;
    
    HttpHlp::Headers header;
    make_upload_request_header(header);
    
    string url = server_api::make_url(dynamic_host_holder::instance()->get_host(BLOCK_HOST), server_api::file_upload_block_api);
    
    block_data block;
    string block_data_buffer;
    block.m_weak = block_md5;
    block.m_strong = block_sha1;
    block.m_stub = m_file_block.block_list[index].t;
    block.m_xid = m_file_block.xid;
    block.m_bid = m_file_block.block_list[index].id;
    block.m_version = m_file_block.file_version;
    block.make(block_path, block_data_buffer);
    
    http_upload_post_request upload_request(url,block_data_buffer);
    upload_request.set_header(header.get());
    upload_request.set_progress_cmd(
        std::bind(&rm_xfile_uploader::update_progress,
                  this,std::placeholders::_1,
                  std::placeholders::_2,
                  std::placeholders::_3,
                  std::placeholders::_4,
                  std::placeholders::_5));
    upload_request.set_cancle_check_cmd(m_cancle_check_cmd);
    ret = upload_request.perform();
    string recv_data;
    if(ret == XLIVE_OK )
    {
        recv_data = upload_request.get_result();
    }
    return ret;
}

ErrorNo rm_xfile_uploader::commit()
{
    ErrorNo ret = XLIVE_OK;

    Json::Value jobj;
    jobj["token"] = m_token;
    jobj["xid"] = m_file_block.xid;
    jobj["fileVer"] = m_file_block.file_version;
    jobj["blockMax"] = (int)m_file_block.block_list.size();
    
    Json::Value jobj_block_list;
    for(int i = 0; i < m_file_block.block_list.size(); ++i)
    {
        Json::Value jobj_block;
        jobj_block["i"] = m_file_block.block_list[i].id;
        jobj_block["w"] = m_file_block.block_list[i].md5;
        jobj_block["s"] = m_file_block.block_list[i].sha1;
        jobj_block["t"] = m_file_block.block_list[i].t;
        jobj_block_list.append(jobj_block);
    }
    jobj["blockCommitInfo"] = jobj_block_list;
    jobj["sha1"] = m_file_sha1;
    
    string url = server_api::make_url(OPERATION_SERVER_HOST, server_api::file_upload_commit_api);
    string post_data = Json::FastWriter().write(jobj);
    http_post_request request(url, post_data);
    ret = request.perform();
    string result = request.get_result();
    return ret;
}

void rm_xfile_uploader::make_upload_request_header(HttpHlp::Headers& header)
{
    header.add("Connection", "Keep-Alive");
    header.add("X-Auth-Token", m_token.c_str());
}

void rm_xfile_uploader::update_progress(double tf_dtotal, double tf_dnow, double tf_utotal, double tf_unow, double speed)
{
    if(m_progress_cmd)
    {
//        m_progress_cmd(m_tf_complete_total_size+tf_unow, tf_unow, speed);
        size_t curent_time = time(0);
        if(able_to_update_progress(curent_time))
        {
            speed = calculate_speed(m_tf_complete_total_size+tf_unow, curent_time);
            m_progress_cmd(m_tf_complete_total_size+tf_unow, tf_dnow, speed);
        }
    }
}

void rm_xfile_uploader::set_progress_cmd(std::function<void(double, double, double)> cmd)
{
    m_progress_cmd = cmd;
}

void rm_xfile_uploader::set_cancle_check_cmd(const std::function<bool (void)>& cmd)
{
    m_cancle_check_cmd = cmd;
}

uint64_t rm_xfile_uploader::get_xfile_xid()
{
    return m_file_block.xid;
}

bool rm_xfile_uploader::is_cancle()
{
    if(m_cancle_check_cmd)
        return m_cancle_check_cmd();
    return false;
}

void rm_xfile_uploader::make_upload_xfile()
{
    m_sp_xfile.reset(new xfile());
    m_sp_xfile->m_p_xid = m_sp_file_data->m_p_xid;
    m_sp_xfile->m_xtype = m_sp_file_data->m_file_type;
    m_sp_xfile->m_xid   = m_file_block.xid;
    m_sp_xfile->m_size  = m_sp_file_data->m_file_size;
    m_sp_xfile->m_ctime = m_sp_file_data->m_ctime;
    m_sp_xfile->m_is_share  = m_sp_file_data->m_space_type;
    m_sp_xfile->m_name  = m_sp_file_data->m_name;
}

bool rm_xfile_uploader::able_to_update_progress(size_t curent_time)
{
    if(curent_time - m_last_update_progress_time >= 1)
        return true;
    return false;
}

uint64_t rm_xfile_uploader::calculate_speed(double tf_total_size, size_t curent_time)
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

xfile_shared_ptr rm_xfile_uploader::get_upload_xfile()
{
    return m_sp_xfile;
}

























