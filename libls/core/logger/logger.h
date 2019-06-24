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

// #pragma warning(disable:4231)
//
// #include <log4cxx/logger.h>
//         using namespace log4cxx;
//
//         typedef LoggerPtr _loggerptr;

#else

#include "thread/thread.h"

        //////////////////////////////////////////////////////////////////////////
        struct SLogLayout
        {
            lstring strName;
            lstring strFormat;
        };

        //////////////////////////////////////////////////////////////////////////
        struct SLogAppender
        {
            lstring strName;
            enum ELogAppenderType
            {
                CONSOLE, FILE
            } eType;
            lstring strFile;
            enum ELogAppenderRoolling
            {
                NO_ROLLING,
                FILE_ROLLING,
                DAILY_ROLLING
            } eRooling;
            lstring strDatePattern;
            int nMaxFileSize;
            int nMaxFileCounts;
        };

        //////////////////////////////////////////////////////////////////////////
        struct SLogger
        {
            lstring strName;
            ELOG_LEVEL eLevel;
            SLogLayout layout;
            SLogAppender appender;

            // program data
            lstring strLogDate;
            int nLogFileIndex;
            int nWritedSize;
            FILE* fp;

            SLogger() : nLogFileIndex(-1), nWritedSize(-1), fp(NULL) {};
        };
        typedef SLogger* _loggerptr;

        //////////////////////////////////////////////////////////////////////////
        struct SLogConfig
        {
            lstring m_strFile;
            map<lstring, SLogLayout> m_mapLayouts;
            map<lstring, SLogAppender> m_mapAppenders;
            map<lstring, SLogger> m_mapLogger;

            SLogConfig();
            SLogConfig(_lpcstr lpstrFilePath);

            bool Parse();
            bool Parse(_lpcstr lpstrFilePath);

            SLogLayout* GetLayout(_lpcstr name);
            SLogAppender* GetAppender(_lpcstr name);
            SLogger* GetLogger(_lpcstr name);
        };

        //////////////////////////////////////////////////////////////////////////
        class CLogManager : public CThread
        {
            public:
                CLogManager();
                virtual ~CLogManager();

            public:
                virtual const char* GetName() { return "CLogManager"; }
                virtual void OnExecute();

            public:
                static void Init(_lpcstr configFile);
                static bool IsInited();
                static void Destroy();
                static _loggerptr GetLogger(_lpcstr lpstrLoggerName);
                static void SetLogLevel(_loggerptr pLogger, ELOG_LEVEL eLevel);
                LSLIB_API static void Log(_loggerptr logger, ELOG_LEVEL level, _lpcstr file, int line, int threadId, _lpcstr format, ...);

            private:
                static bool m_bInited;
                static SLogConfig m_logConfig;
                struct SLogEntity
                {
                    _loggerptr logger;
                    ELOG_LEVEL level;
                    lstring file;
                    int line;
                    int threadId;
                    lstring msg;
                };
                static list<SLogEntity> m_lstLogEntitysTmp;
                static list<SLogEntity> m_lstLogEntitys;
                static CMutexLock m_mtxLogEntityTmp;
                static CMutexLock m_mtxLogEntity;
        };

#endif // USE_LOG4CXX

        //////////////////////////////////////////////////////////////////////////

        LSLIB_API void InitLogger(_lpcstr lpstrCfgXml);
        LSLIB_API void DestroyLogger();
        LSLIB_API _loggerptr GetLogger(_lpcstr lpstrLoggerName);
        LSLIB_API void SetLogLevel(_loggerptr pLogger, ELOG_LEVEL eLevel);

#ifdef WIN32
#define __THREAD__ GetCurrentThreadId()
#else
#endif
        /*
         * params:
         *   logger: (_loggerptr) logger instance
         *   fmt: (const char*) log string format
         */
#ifdef USE_LOG4CXX
#define FATAL_LOG(logger, fmt, ...)             { if (logger) LOG4CXX_FATAL(logger, lstring().format(lstring("[%s] - ") + (fmt), __FUNCTION__, __VA_ARGS__)); }
#define ERROR_LOG(logger, fmt, ...)             { if (logger) LOG4CXX_ERROR(logger, lstring().format(lstring("[%s] - ") + (fmt), __FUNCTION__, __VA_ARGS__)); }
#define WARN_LOG(logger, fmt, ...)              { if (logger) LOG4CXX_WARN(logger, lstring().format(lstring("[%s] - ") + (fmt), __FUNCTION__, __VA_ARGS__)); }
#define INFO_LOG(logger, fmt, ...)              { if (logger) LOG4CXX_INFO(logger, lstring().format(lstring("[%s] - ") + (fmt), __FUNCTION__, __VA_ARGS__)); }
#define DEBUG_LOG(logger, fmt, ...)             { if (logger) LOG4CXX_DEBUG(logger, lstring().format(lstring("[%s] - ") + (fmt), __FUNCTION__, __VA_ARGS__)); }
#define TRACE_LOG(logger, fmt, ...)             { if (logger) LOG4CXX_TRACE(logger, lstring().format(lstring("[%s] - ") + (fmt), __FUNCTION__, __VA_ARGS__)); }
#else
#define FATAL_LOG(logger, fmt, ...)             { if (logger) CLogManager::Log(logger, LOG_LEVEL_FATAL, __FUNCTION__, __LINE__, __THREAD__, fmt, __VA_ARGS__); }
#define ERROR_LOG(logger, fmt, ...)             { if (logger) CLogManager::Log(logger, LOG_LEVEL_ERROR, __FUNCTION__, __LINE__, __THREAD__, fmt, __VA_ARGS__); }
#define WARN_LOG(logger, fmt, ...)              { if (logger) CLogManager::Log(logger, LOG_LEVEL_WARN, __FUNCTION__, __LINE__, __THREAD__, fmt, __VA_ARGS__); }
#define INFO_LOG(logger, fmt, ...)              { if (logger) CLogManager::Log(logger, LOG_LEVEL_INFO, __FUNCTION__, __LINE__, __THREAD__, fmt, __VA_ARGS__); }
#define DEBUG_LOG(logger, fmt, ...)             { if (logger) CLogManager::Log(logger, LOG_LEVEL_DEBUG, __FUNCTION__, __LINE__, __THREAD__, fmt, __VA_ARGS__); }
#define TRACE_LOG(logger, fmt, ...)             { if (logger) CLogManager::Log(logger, LOG_LEVEL_TRACE, __FUNCTION__, __LINE__, __THREAD__, fmt, __VA_ARGS__); }
#endif // USE_LOG4CXX

//////////////////////////////////////////////////////////////////////////
        /*LSLIB_API */extern _loggerptr g_logger;
        /*LSLIB_API */extern _loggerptr g_netlogger;
        LSLIB_API void RegistGlobalLogger(_loggerptr pLogger);
        LSLIB_API void RegistNetLogger(_loggerptr pLogger);

    } // namespace logger

} // namespace lslib


#endif // __LOGGER_H__
