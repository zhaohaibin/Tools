//
//  http_request.h
//  KingFile
//
//  Created by ks on 15/12/28.
//  Copyright (c) 2015年 ks. All rights reserved.
//

#ifndef __KingFile__http_request__
#define __KingFile__http_request__

#include <stdio.h>
#include "libxlive/CrossPlatformConfig.h"
#include "curl/curl.h"
#include "LibCurlHttp/HttpHlp.h"
#include "server_api/server_api.h"
#include "RemoteRequestParamsFactory.h"
#include "JsonCpp/Json.h"
#include "role/role.h"

class http_base_request
{
public:
    http_base_request(const std::string& url);
    virtual ~http_base_request(){}
    
    ErrorNo perform();
    std::string get_result();
    
    void set_header( const char* p_headers);
    void set_send_max_speed(int max_speed);
    void set_recv_max_speed(int max_speed);
    void set_time_out(int time_out);
    void enable_https(bool b_eanble);
    void set_pem_path(const std::string& cert_path);
	void set_cert(const std::string& cert);
    
    void set_progress_cmd(const std::function<void(double, double, double, double, double)>& cmd);
    void set_cancle_check_cmd(const std::function<bool (void)>& cmd);
    
protected:
    static int progress_callback(void *clientp,   double dltotal,   double dlnow,   double ultotal,   double ulnow);
    static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);
	static CURLcode sslctx_function(CURL * curl, void * sslctx, void * parm);
    
    virtual void set_custom_opt();
    virtual void set_curl_recv_data_function();
    virtual void set_https_protocol();
    virtual void set_curl_post_data();

private:
    ErrorNo init_curl();
    void release_curl();
    
    void set_curl_progress_callback();
    void set_curl_url();
    void set_curl_header();
    void set_curl_max_speed();
    void set_curl_time_out();
    void set_curl_protocols();
    bool is_cancle();
    void update_progress(double tf_dtotal, double tf_dnow, double tf_utotal, double tf_unow);
    ErrorNo do_perform();
protected:
    std::string		m_recv_data;
    std::string     m_post_data;
    std::string		m_url;
	std::string		m_cert;
    CURL*			m_p_curl;
    curl_slist*		m_p_header;
    int				m_time_out;
    int				m_send_max_speed;
    int				m_recv_max_seppd;
    std::string		m_cert_path;
    bool			m_enable_https;
    std::function<bool (void)>		m_cancle_check_cmd;
    std::function<void(double, double, double, double, double)> m_progress_cmd;
};

////////////////////////////////////////////////////////////////////////////////
class http_get_request : public http_base_request
{
public:
    http_get_request(const std::string& url);
    
};

/////////////////////////////////////////////////////////////////////////////////
class http_post_request : public http_base_request
{
public:
    http_post_request(const std::string& url, const std::string& post_data);
    virtual ~http_post_request();
    
    void set_post_data(const std::string& post_data);
};


///////////////////////////////////////////////////////////////////////////////////
class http_upload_post_request : public http_post_request
{
public:
    http_upload_post_request(const std::string& url, const std::string& post_data);
    virtual ~http_upload_post_request();
    
protected:
    virtual void set_curl_post_data();
                           
    static size_t read_data_function(void* buffer, size_t size, size_t n, void* data_stream);
private:
    unsigned int m_read_beg_pos;
};

//////////////////////////////////////////////////////////////////////////////////
class http_download_post_request : public http_post_request
{
public:
    http_download_post_request(
                          const CBoostPath& path_name,
                          const std::string& url,
                          const std::string& post_data);
    virtual ~http_download_post_request();
    
    ErrorNo download();
protected:
    virtual void set_curl_recv_data_function();
private:
    ErrorNo open_file();
    void close_file();
private:
    FILE*               m_p_file;
    CBoostPath          m_file_full_path;
};

#endif /* defined(__KingFile__http_request__) */


















