//
// Created by zhaohaibin on 2/2/18.
//

#ifndef IMAGEMATCHER_SMARTLOG_H
#define IMAGEMATCHER_SMARTLOG_H

#include <string>
using namespace std;


#include <boost/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/attributes/named_scope.hpp>


namespace smartlog
{
    void InitLog(const string& path, const string fileName);

#define LOG_INFO(msg) \
    BOOST_LOG_FUNCTION();\
    BOOST_LOG_TRIVIAL(info) << msg;

#define LOG_WARNING(msg) \
    BOOST_LOG_FUNCTION(); \
    BOOST_LOG_TRIVIAL(warning) << msg;

#define LOG_ERROR(msg) \
    BOOST_LOG_FUNCTION(); \
    BOOST_LOG_TRIVIAL(error) << msg

}


#endif //IMAGEMATCHER_SMARTLOG_H
