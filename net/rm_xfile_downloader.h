//
//  rm_xfile_downloader.h
//  KingFile
//
//  Created by ks on 15/12/30.
//  Copyright (c) 2015年 ks. All rights reserved.
//

#ifndef __KingFile__rm_xfile_downloader__
#define __KingFile__rm_xfile_downloader__

#include <stdio.h>
#include <string>

#include "table_data.h"
#include "libxlive/LibXLiveError.h"
#include "../InternalStruct.h"
#include "RemoteRequestParamsFactory.h"


struct file_stub;
struct block_stub;

class rm_xfile_downloader
{
public:
    rm_xfile_downloader(const std::string& token, std::shared_ptr<download_file_table_data> sp_file_data, const std::string& block_temp_folder);
    ~rm_xfile_downloader();
    
    ErrorNo download();
    void set_download_prepare_url(const string& url);
    void set_download_block_url(const string& url);
    void set_progress_cmd(std::function<void(double, double, double)> cmd);
    void set_cancle_check_cmd(const std::function<bool (void)>& cmd);
private:
    ErrorNo download_prepare(string& recv_data);
    ErrorNo parse_prepare_file_data(const string& prepare_data,file_stub& fstub);
    ErrorNo download_blocks(const file_stub& fstub);
    ErrorNo download_block(const CBoostPath& block_path, PARAMS_FACTORY::download_block_params_factory& params_factory);
    CBoostPath format_block_temp_path(const string& temp_folder, const block_stub& bstub, UInt64 xid);
    
    void update_progress(double tf_dtotal, double tf_dnow, double tf_utotal, double tf_unow, double speed);
    
    bool is_cancle();
    
    uint64_t calculate_speed(double tf_total_size, size_t curent_time);
    bool able_to_update_progress(size_t curent_time);
private:
    string m_token;
    std::shared_ptr<download_file_table_data> m_sp_file_data;
    string m_prepare_url;
    string m_download_block_url;
    string m_file_save_path;
    string m_block_temp_folder;
    PARAMS_FACTORY::download_prepare_params_factory m_dp_params_factory;
    PARAMS_FACTORY::download_block_params_factory   m_db_params_factory;
    STORAGE::file_block_info m_file_block;
    unsigned int m_tf_complete_total_size;
    std::function<void(double, double, double)> m_progress_cmd;
    std::function<bool (void)> m_cancle_check_cmd;
    
    double m_last_tf_total_size;
    size_t m_last_update_progress_time;
    
    
};
#endif /* defined(__KingFile__rm_xfile_downloader__) */
































