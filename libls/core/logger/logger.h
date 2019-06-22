#ifndef __LOGGER_H__
#define __LOGGER_H__
#pragma once

namespace lslib
{
    namespace logger
    {

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

#ifdef USE_LOG4CXX

#pragma warning(disable:4231)

#include <log4cxx/logger.h>
        using namespace log4cxx;

//////////////////////////////////////////////////////////////////////////
        typedef LoggerPtr _loggerptr;

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
        /*LSLIB_API */extern _loggerptr g_logger;
        /*LSLIB_API */extern _loggerptr g_netlogger;
        LSLIB_API void RegistGlobalLogger(_loggerptr pLogger);
        LSLIB_API void RegistNetLogger(_loggerptr pLogger);
#else

#include "lock/lock.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
        using namespace std;

        typedef const char* _loggerptr;

//////////////////////////////////////////////////////////////////////////
        class CLogger/* : public CThread*/
        {
        private:
            CLogger();
            ~CLogger();

        public:
            static void
            Init(const char* file = NULL, ELOG_LEVEL level = LOG_LEVEL_INFO);
            static void
            Destory();
            static void
            Log(ELOG_LEVEL level, const char* format, ...);
            static void
            SetLogLevel(ELOG_LEVEL level);

        protected:
            static string
            GetDateNowStr(const char split = '-');
            static string
            GetTimeNowStr(bool format = true);

        private:
            static ELOG_LEVEL m_eLevel;
            static FILE* m_pFile;
            static bool m_bInited;
            static CMutexLock m_mutex;
        };

    /*初始化日志模块
     @file  输出文件名，若为NULL，则输出到控制台
     @level 输出日志级别
     */
#define LOG_INIT(file, level)       CLogger::Init(file, level)
    /*清理日志模块*/
#define LOG_DESTORY()               CLogger::Destory()

#if 1
//使用前，需要先调用 LOG_INIT(file, level)
#define TRACE_LOG(logger, fmt, ...)              CLogger::Log(LOG_LEVEL_TRACE, __FILE__, __LINE__,  __VA_ARGS__)
#define DEBUG_LOG(logger, fmt, ...)              CLogger::Log(LOG_LEVEL_DEBUG, __FILE__, __LINE__,  __VA_ARGS__)
#define INFO_LOG(logger, fmt, ...)               CLogger::Log(LOG_LEVEL_INFO, __FILE__, __LINE__,  __VA_ARGS__)
#define WARN_LOG(logger, fmt, ...)               CLogger::Log(LOG_LEVEL_WARN, __FILE__, __LINE__,  __VA_ARGS__)
#define ERROR_LOG(logger, fmt, ...)              CLogger::Log(LOG_LEVEL_ERROR, __FILE__, __LINE__,  __VA_ARGS__)
#define FATAL_LOG(logger, fmt, ...)              CLogger::Log(LOG_LEVEL_FATAL, __FILE__, __LINE__,  __VA_ARGS__)
#else
#define TRACE_LOG(logger, fmt, ...)
#define DEBUG_LOG(logger, fmt, ...)
#define INFO_LOG(logger, fmt, ...)
#define WARN_LOG(logger, fmt, ...)
#define ERROR_LOG(logger, fmt, ...)
#define FATAL_LOG(logger, fmt, ...)
#endif

#endif // USE_LOG4CXX

    } // namespace logger

} // namespace lslib

#endif // __LOGGER_H__
