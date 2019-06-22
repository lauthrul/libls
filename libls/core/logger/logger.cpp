#pragma once
#include "stdafx.h"
#include "logger.h"

namespace lslib
{
    namespace logger
    {

#ifdef USE_LOG4CXX

#include <log4cxx/consoleappender.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/simplelayout.h>
#include <log4cxx/xml/domconfigurator.h>
        //////////////////////////////////////////////////////////////////////////
        _loggerptr loggerInternal = NULL;
        _loggerptr GetInternalLogger()
        {
            if (loggerInternal == NULL)
            {
                log4cxx::ConsoleAppender* consoleAppender = new log4cxx::ConsoleAppender(log4cxx::LayoutPtr(new log4cxx::SimpleLayout()));
                log4cxx::BasicConfigurator::configure(log4cxx::AppenderPtr(consoleAppender));
                loggerInternal = log4cxx::Logger::getRootLogger();
            }
            return loggerInternal;
        }

        //////////////////////////////////////////////////////////////////////////
        LSLIB_API void InitLogger(_lpcstr lpstrCfgXml)
        {
            if (is_empty(lpstrCfgXml)) return;
            log4cxx::xml::DOMConfigurator::configure(lpstrCfgXml);
        }

        LSLIB_API _loggerptr GetLogger(_lpcstr lpstrLoggerName)
        {
            if (is_empty(lpstrLoggerName)) return NULL;
            return Logger::getLogger(lpstrLoggerName);
        }

        LSLIB_API void SetLogLevel(_loggerptr pLogger, ELOG_LEVEL eLevel)
        {
            if (pLogger == NULL) return;

            LevelPtr level;
#ifdef _DEBUG
            level = log4cxx::Level::getAll();
#else
            level = log4cxx::Level::getInfo();
#endif
            switch (eLevel)
            {
                case LOG_LEVEL_NONE: level = log4cxx::Level::getOff(); break;
                case LOG_LEVEL_FATAL: level = log4cxx::Level::getFatal(); break;
                case LOG_LEVEL_ERROR: level = log4cxx::Level::getError(); break;
                case LOG_LEVEL_WARN: level = log4cxx::Level::getWarn(); break;
                case LOG_LEVEL_INFO: level = log4cxx::Level::getInfo(); break;
                case LOG_LEVEL_DEBUG: level = log4cxx::Level::getDebug(); break;
                case LOG_LEVEL_TRACE: level = log4cxx::Level::getTrace(); break;
                case LOG_LEVEL_ALL: level = log4cxx::Level::getAll(); break;
            }
            pLogger->setLevel(level);
        }

        //////////////////////////////////////////////////////////////////////////
        extern _loggerptr g_logger = NULL;// = GetInternalLogger();
        extern _loggerptr g_netlogger = NULL;// = GetInternalLogger();

        LSLIB_API void RegistGlobalLogger(_loggerptr pLogger)
        {
            g_logger = pLogger;
        }

        LSLIB_API void RegistNetLogger(_loggerptr pLogger)
        {
            g_netlogger = pLogger;
        }
#else

#include <time.h>
#include <cstdarg>

#define             MAX_SIZE_PER_FILE       (1024*1024*50)

        ELOG_LEVEL CLogger::m_eLevel = LOG_LEVEL_INFO;
        FILE* CLogger::m_pFile = stdout;
        bool CLogger::m_bInited = false;
        CMutexLock CLogger::m_mutex;
//////////////////////////////////////////////////////////////////////////
        CLogger::CLogger()
        {
        }

        CLogger::~CLogger()
        {
            Destory();
        }

        void CLogger::Init(const char* file, ELOG_LEVEL level/* = LOG_LEVEL_INFO*/)
        {
            if (m_bInited) return;

            if (file != NULL)
            {
                m_pFile = fopen(file, "r");
                int fsize = 0;
                if (m_pFile != NULL)
                {
                    fseek(m_pFile, 0, SEEK_END);
                    fsize = ftell(m_pFile);
                    fclose(m_pFile);
                }

                if (fsize >= MAX_SIZE_PER_FILE)
                {
                    string strrename = GetDateNowStr('_') + "." + string(file);
                    rename(file, strrename.c_str());
                }

                m_pFile = fopen(file, "a+");
            }

            m_eLevel = level;
            m_bInited = true;
        }

        void CLogger::Destory()
        {
            fclose(m_pFile);
        }

        void CLogger::Log(ELOG_LEVEL level, const char* format, ...)
        {
            if (!m_bInited)
            {
                printf("log module not inited, please init log module with LOG_INIT(file, level) first.\n");
                return;
            }

            m_mutex.Lock();
            if (level <= m_eLevel)
            {
                char* strlvl;
                switch (level)
                {
                case LOG_LEVEL_TRACE:
                    strlvl = "TRACE";
                    break;
                case LOG_LEVEL_DEBUG:
                    strlvl = "DEBUG";
                    break;
                case LOG_LEVEL_INFO:
                    strlvl = "INFO";
                    break;
                case LOG_LEVEL_WARN:
                    strlvl = "WARN";
                    break;
                case LOG_LEVEL_ERROR:
                    strlvl = "ERROR";
                    break;
                case LOG_LEVEL_FATAL:
                    strlvl = "FATAL";
                    break;
                }

                va_list args;
                va_start(args, format);
                int len = vsnprintf(NULL, 0, format, args);
                char* pbuf = (char*)malloc(len + 1);
                memset(pbuf, 0, len + 1);
                vsnprintf(pbuf, len, format, args);
                va_end(args);

                fprintf(m_pFile, "%s\t%s\t%s\n", GetTimeNowStr().c_str(), strlvl, pbuf);
                fflush(m_pFile);
                free(pbuf);
            }
            m_mutex.Unlock();
        }

        void CLogger::SetLogLevel(ELOG_LEVEL level)
        {
            if (m_eLevel != level) m_eLevel = level;
        }

        string CLogger::GetTimeNowStr(bool format /*= true*/)
        {
            time_t rawtime;
            tm *timeinfo;
            char buf[20] = { 0 };

            time(&rawtime);
            timeinfo = localtime(&rawtime);
            if (format)
            {
                sprintf(buf, "%4d-%02d-%02d %02d:%02d:%02d", 1900 + timeinfo->tm_year, 1 + timeinfo->tm_mon, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
            }
            else
            {
                sprintf(buf, "%ld", rawtime);
            }

            return string(buf);
        }

        string CLogger::GetDateNowStr(const char split/* = '-'*/)
        {
            time_t rawtime;
            tm *timeinfo;
            char buf[20] = { 0 };

            time(&rawtime);
            timeinfo = localtime(&rawtime);
            sprintf(buf, "%4d%c%02d%c%02d", 1900 + timeinfo->tm_year, split, 1 + timeinfo->tm_mon, split, timeinfo->tm_mday);
            return string(buf);
        }

#endif // USE_LOG4CXX

    } // log

} // lslib
