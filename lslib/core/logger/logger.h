#pragma once
#pragma warning(disable:4231)

#include <log4cxx/logger.h>
using namespace log4cxx;

namespace lslib
{
    namespace logger
    {
        //////////////////////////////////////////////////////////////////////////
        typedef LoggerPtr _loggerptr;

        enum ELOG_LEVEL
        {
            LOG_LEVEL_NONE,
            LOG_LEVEL_FATAL,
            LOG_LEVEL_ERROR,
            LOG_LEVEL_WARN,
            LOG_LEVEL_INFO,
            LOG_LEVEL_DEBUG,
            LOG_LEVEL_TRACE,
            LOG_LEVEL_ALL,
        };

        LSLIB_API void InitLogger(_lpcstr lpstrCfgXml);
        LSLIB_API _loggerptr GetLogger(_lpcstr lpstrLoggerName);
        LSLIB_API void SetLogLevel(_loggerptr pLogger, ELOG_LEVEL eLevel);

        //////////////////////////////////////////////////////////////////////////
        /*
        * params:
        *   logger: (_loggerptr) logger instance
        *   fmt: (const char*) log string format
        */
        #ifdef _DEBUG
        #define FATAL_LOG(logger, fmt, ...)             { if (logger) LOG4CXX_FATAL(logger, lstring().format(lstring("[%s] - ") + (fmt), __FUNCTION__, __VA_ARGS__)); }
        #define ERROR_LOG(logger, fmt, ...)             { if (logger) LOG4CXX_ERROR(logger, lstring().format(lstring("[%s] - ") + (fmt), __FUNCTION__, __VA_ARGS__)); }
        #define WARN_LOG(logger, fmt, ...)              { if (logger) LOG4CXX_WARN(logger, lstring().format(lstring("[%s] - ") + (fmt), __FUNCTION__, __VA_ARGS__)); }
        #define INFO_LOG(logger, fmt, ...)              { if (logger) LOG4CXX_INFO(logger, lstring().format(lstring("[%s] - ") + (fmt), __FUNCTION__, __VA_ARGS__)); }
        #define DEBUG_LOG(logger, fmt, ...)             { if (logger) LOG4CXX_DEBUG(logger, lstring().format(lstring("[%s] - ") + (fmt), __FUNCTION__, __VA_ARGS__)); }
        #define TRACE_LOG(logger, fmt, ...)             { if (logger) LOG4CXX_TRACE(logger, lstring().format(lstring("[%s] - ") + (fmt), __FUNCTION__, __VA_ARGS__)); }
        #else
        #define FATAL_LOG(logger, fmt, ...)             { if (logger) LOG4CXX_FATAL(logger, lstring().format((fmt), __VA_ARGS__)); }
        #define ERROR_LOG(logger, fmt, ...)             { if (logger) LOG4CXX_ERROR(logger, lstring().format((fmt), __VA_ARGS__)); }
        #define WARN_LOG(logger, fmt, ...)              { if (logger) LOG4CXX_WARN(logger, lstring().format((fmt), __VA_ARGS__)); }
        #define INFO_LOG(logger, fmt, ...)              { if (logger) LOG4CXX_INFO(logger, lstring().format((fmt), __VA_ARGS__)); }
        #define DEBUG_LOG(logger, fmt, ...)             { if (logger) LOG4CXX_DEBUG(logger, lstring().format((fmt), __VA_ARGS__)); }
        #define TRACE_LOG(logger, fmt, ...)             { if (logger) LOG4CXX_TRACE(logger, lstring().format((fmt), __VA_ARGS__)); }
        #endif

        //////////////////////////////////////////////////////////////////////////
        LSLIB_API extern _loggerptr g_logger;
        LSLIB_API extern _loggerptr g_netlogger;
        LSLIB_API void RegistGlobalLogger(_loggerptr pLogger);
        LSLIB_API void RegistNetLogger(_loggerptr pLogger);

    } // namespace logger

} // namespace lslib
