//
//  rm_xfile_uploader.h
//  KingFile
//
//  Created by ks on 15/12/28.
//  Copyright (c) 2015年 ks. All rights reserved.
//

#ifndef __KingFile__rm_xfile_uploader__
#define __KingFile__rm_xfile_uploader__

#include <stdio.h>
#include "libxlive/CrossPlatformConfig.h"
#include "LibCurlHttp/HttpHlp.h"
#include "server_api/server_api.h"
#include "RemoteRequestParamsFactory.h"
#include "JsonCpp/Json.h"
#include "role/role.h"
#include "table_data.h"
#include "RemoteRequestParamsFactory.h"
#include "../EncodeFile/CBlockGenerator.h"
#include "../../LibCurlHttp/HttpHlp.h"
#include "RemoteFileSystemHelper.h"

class rm_xfile_uploader
{
public:
    rm_xfile_uploader(const std::string& token, std::shared_ptr<upload_file_table_data> sp_file_data, const std::string& block_temp_folder);
    ~rm_xfile_uploader();
    
    ErrorNo upload();
    void set_upload_create_url(const string& url);
    void set_upload_block_url(const string& url);
    void set_progress_cmd(std::function<void(double, double, double)> cmd);
    void set_cancle_check_cmd(const std::function<bool (void)>& cmd);
    
    uint64_t get_xfile_xid();
    xfile_shared_ptr get_upload_xfile();
private:
    ErrorNo upload_create(string& recv_data);
    ErrorNo parse_create_file_data(const string& recv_data, STORAGE::file_block_info& file_block);
    ErrorNo upload_blocks(const STORAGE::file_block_info& file_block);
    ErrorNo upload_block(const CBoostPath& block_path, int index);
    ErrorNo commit();
    void make_upload_request_header(HttpHlp::Headers& header);
    
    void update_progress(double tf_dtotal, double tf_dnow, double tf_utotal, double tf_unow, double speed);
    bool is_cancle();
    
    void make_upload_xfile();
    
    bool able_to_update_progress(size_t curent_time);
    
    uint64_t calculate_speed(double tf_total_size, size_t curent_time);
private:
    string m_token;
    std::shared_ptr<upload_file_table_data> m_sp_file_data;
    string m_create_url;
    string m_upload_block_url;
    string m_file_sha1;
    PARAMS_FACTORY::upload_create_params_factory m_uc_params_factory;
    CBlockGenerator m_block_generator;
    
    string m_up_create_recv_data;
    STORAGE::file_block_info m_file_block;
    unsigned int m_tf_complete_total_size;
    std::function<void(double, double, double)> m_progress_cmd;
    std::function<bool (void)> m_cancle_check_cmd;
    xfile_shared_ptr m_sp_xfile;                                    //用于保存上传的文件信息
    
    double m_last_tf_total_size;
    size_t m_last_update_progress_time;
};

#endif /* defined(__KingFile__rm_xfile_uploader__) */




















