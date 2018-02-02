//
// Created by zhaohaibin on 2/2/18.
//

#include "SmartLog.h"

namespace smartlog
{
    void InitLog(const string& path, const string fileName)
    {
        boost::log::add_common_attributes();
        boost::log::core::get()->add_global_attribute("Scope",
                                                      boost::log::attributes::named_scope());
        boost::log::core::get()->set_filter(
                boost::log::trivial::severity >= boost::log::trivial::trace
        );

        /* log formatter:
	* [TimeStamp] [ThreadId] [Severity Level] [Scope] Log message
	*/
        auto fmtTimeStamp = boost::log::expressions::
        format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f");
        auto fmtThreadId = boost::log::expressions::
        attr<boost::log::attributes::current_thread_id::value_type>("ThreadID");
        auto fmtSeverity = boost::log::expressions::
        attr<boost::log::trivial::severity_level>("Severity");
        auto fmtScope = boost::log::expressions::format_named_scope("Scope",
                                                                    boost::log::keywords::format = "%n(%f:%l)",
                                                                    boost::log::keywords::iteration = boost::log::expressions::reverse,
                                                                    boost::log::keywords::depth = 2);
        boost::log::formatter logFmt =
                boost::log::expressions::format("[%1%] (%2%) [%3%] [%4%] %5%")
                % fmtTimeStamp % fmtThreadId % fmtSeverity % fmtScope
                % boost::log::expressions::smessage;

        /* console sink */
        auto consoleSink = boost::log::add_console_log(std::clog);
        consoleSink->set_formatter(logFmt);

        /* fs sink */
        boost::filesystem::path logFilePath(path);
        logFilePath.append(fileName);
        auto fsSink = boost::log::add_file_log(
                boost::log::keywords::file_name = logFilePath.generic_string(),
                boost::log::keywords::rotation_size = 3 * 1024 * 1024,
                boost::log::keywords::min_free_space = 6 * 1024 * 1024,
                boost::log::keywords::max_size = 18 * 1024 * 1024,
                boost::log::keywords::max_files = 6,
                boost::log::keywords::scan_method = boost::log::sinks::file::scan_all,
                boost::log::keywords::target = logFilePath.parent_path().generic_string(),
                boost::log::keywords::open_mode = std::ios_base::app);
        fsSink->set_formatter(logFmt);
        fsSink->locked_backend()->auto_flush(true);
    }
}