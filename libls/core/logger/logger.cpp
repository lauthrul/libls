#pragma once
#include "stdafx.h"
#include "logger.h"
#include <stdarg.h>

#ifdef USE_LOG4CXX
#include <log4cxx/consoleappender.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/simplelayout.h>
#include <log4cxx/xml/domconfigurator.h>
#endif

namespace lslib
{
    namespace logger
    {

#ifdef USE_LOG4CXX

        //////////////////////////////////////////////////////////////////////////
//         _loggerptr loggerInternal = 0;
//         _loggerptr GetInternalLogger()
//         {
//             if (loggerInternal == 0)
//             {
//                 log4cxx::ConsoleAppender* consoleAppender = new log4cxx::ConsoleAppender(log4cxx::LayoutPtr(new log4cxx::SimpleLayout()));
//                 log4cxx::BasicConfigurator::configure(log4cxx::AppenderPtr(consoleAppender));
//                 loggerInternal = log4cxx::Logger::getRootLogger();
//             }
//             return loggerInternal;
//         }

        void Log(_loggerptr logger, ELOG_LEVEL level, _lpcstr file, int line, _lpcstr function, int threadId, ...)
        {
            if (logger == 0) return;

            va_list args;
            va_start(args, threadId); // last fixed param

            char* format = va_arg(args, char*); // first arg
#ifdef WIN32
            int len = vsnprintf(NULL, 0, format, args); // next args
            char* pbuf = (char*)malloc(len + 1);
            memset(pbuf, 0, len + 1);
            vsnprintf(pbuf, len, format, args);
#else
            char* pbuf = NULL;
            vasprintf(&pbuf, format, args);
#endif
            va_end(args);

            ::log4cxx::spi::LocationInfo location(file, function, line);
            switch (level)
            {
                case LOG_LEVEL_FATAL:
                    if (logger->isFatalEnabled())
                        logger->forcedLog(::log4cxx::Level::getFatal(), pbuf, location);
                    break;
                case LOG_LEVEL_ERROR:
                    if (logger->isErrorEnabled())
                        logger->forcedLog(::log4cxx::Level::getError(), pbuf, location);
                    break;
                case LOG_LEVEL_WARN:
                    if (logger->isWarnEnabled())
                        logger->forcedLog(::log4cxx::Level::getWarn(), pbuf, location);
                    break;
                case LOG_LEVEL_INFO:
                    if (logger->isInfoEnabled())
                        logger->forcedLog(::log4cxx::Level::getInfo(), pbuf, location);
                    break;
                case LOG_LEVEL_DEBUG:
                    if (logger->isDebugEnabled())
                        logger->forcedLog(::log4cxx::Level::getDebug(), pbuf, location);
                    break;
                case LOG_LEVEL_TRACE:
                    if (logger->isTraceEnabled())
                        logger->forcedLog(::log4cxx::Level::getTrace(), pbuf, location);
                    break;

//                 case LOG_LEVEL_FATAL: LOG4CXX_FATAL(logger, pbuf); break;
//                 case LOG_LEVEL_ERROR: LOG4CXX_ERROR(logger, pbuf); break;
//                 case LOG_LEVEL_WARN: LOG4CXX_WARN(logger, pbuf); break;
//                 case LOG_LEVEL_INFO: LOG4CXX_INFO(logger, pbuf); break;
//                 case LOG_LEVEL_DEBUG: LOG4CXX_DEBUG(logger, pbuf); break;
//                 case LOG_LEVEL_TRACE: LOG4CXX_TRACE(logger, pbuf); break;
            }
            free(pbuf);
        }

#else

        //////////////////////////////////////////////////////////////////////////
        SLogConfig::SLogConfig()
        {
        }

        SLogConfig::SLogConfig(_lpcstr lpstrFilePath) : m_strFile(lpstrFilePath)
        {
        }

        bool SLogConfig::Parse()
        {
            return Parse(m_strFile);
        }

        bool SLogConfig::Parse(_lpcstr lpstrFilePath)
        {
            Xml xml;
            xml.ParseFile(lpstrFilePath);
            if (xml.m_xmlRoot == NULL) return false;

            m_strFile = lpstrFilePath;

            XmlNode* pNode = xml.m_xmlRoot->FirstChildElement("layout");
            for (; pNode != NULL; pNode = pNode->NextSiblingElement("layout"))
            {
                if (pNode == NULL) continue;

                SLogLayout layout;
                layout.strName = Xml::GetAttribute(*pNode, "name");
                layout.strFormat = Xml::GetAttribute(*pNode, "format");
                m_mapLayouts[layout.strName] = layout;
            }

            lstring str;
            pNode = xml.m_xmlRoot->FirstChildElement("appender");
            for (; pNode != NULL; pNode = pNode->NextSiblingElement("appender"))
            {
                if (pNode == NULL) continue;

                // read xml data
                SLogAppender appender;
                appender.strName = Xml::GetAttribute(*pNode, "name");

                str = Xml::GetAttribute(*pNode, "type");
                if (str == "console") appender.eType = APPENDER_CONSOLE;
                else if (str == "file") appender.eType = APPENDER_FILE;

                appender.strFile = Xml::GetAttribute(*pNode, "file");

                str = Xml::GetAttribute(*pNode, "rolling");
                if (str == "no_rolling") appender.eRooling = NO_ROLLING;
                else if (str == "daily_rooling") appender.eRooling = DAILY_ROLLING;
                else if (str == "file_rooling") appender.eRooling = FILE_ROLLING;

                appender.strDatePattern = Xml::GetAttribute(*pNode, "date_pattern");

                str = Xml::GetAttribute(*pNode, "max_file_size");
                if (str.find("MB"))
                {
                    str = str.substr(0, str.length() - 2);
                    appender.nMaxFileSize = str.to_int() * 1024 * 1024;
                }
                else if (str.find("KB"))
                {
                    str = str.substr(0, str.length() - 2);
                    appender.nMaxFileSize = str.to_int() * 1024;
                }
                else
                {
                    appender.nMaxFileSize = str.to_int();
                }

                str = Xml::GetAttribute(*pNode, "max_file_counts");
                appender.nMaxFileCounts = str.to_int();

                // init data
                {
                    os::mkdir(os::path_get_dir(appender.strFile));

                    if (appender.strLogDate.empty())
                    {
                        lstring writeTime = os::get_file_attr(appender.strFile).writeTime;
                        if (!writeTime.empty())
                        {
                            Time tm; tm.Parser(writeTime);
                            appender.strLogDate = tm.GetDateStr();
                        }
                        else appender.strLogDate = Time::GetCurDateStr();
                    }

                    if (appender.nWritedSize < 0)
                        appender.nWritedSize = os::get_file_size(appender.strFile);

                    if (appender.nLogFileIndex < 0)
                    {
                        os::enum_file_array arr_files;
                        os::enumerate_files(arr_files,
                                            os::path_get_dir(appender.strFile),
                                            NULL,
                                            os::path_get_name(appender.strFile),
                                            false);
                        if (!arr_files.empty()) appender.nLogFileIndex = arr_files.size() - 1;
                        else appender.nLogFileIndex = 0;
                    }
                }

                m_mapAppenders[appender.strName] = appender;
            }

            pNode = xml.m_xmlRoot->FirstChildElement("logger");
            for (; pNode != NULL; pNode = pNode->NextSiblingElement("logger"))
            {
                if (pNode == NULL) continue;

                SLogger logger;
                logger.strName = Xml::GetAttribute(*pNode, "name");

                str = Xml::GetAttribute(*pNode, "level");
                if (str == "none")  logger.eLevel = LOG_LEVEL_NONE;
                else if (str == "trace")  logger.eLevel = LOG_LEVEL_TRACE;
                else if (str == "debug") logger.eLevel = LOG_LEVEL_DEBUG;
                else if (str == "info") logger.eLevel = LOG_LEVEL_INFO;
                else if (str == "warn") logger.eLevel = LOG_LEVEL_WARN;
                else if (str == "error") logger.eLevel = LOG_LEVEL_ERROR;
                else if (str == "fatal") logger.eLevel = LOG_LEVEL_FATAL;
                else if (str == "all") logger.eLevel = LOG_LEVEL_ALL;

                str = Xml::GetAttribute(*pNode, "layout");
                SLogLayout* pLayout = GetLayout(str);
                if (pLayout) logger.layout = *pLayout;

                str = Xml::GetAttribute(*pNode, "appender");
                SLogAppender* pAppender = GetAppender(str);
                if (pAppender) logger.appender = *pAppender;
                m_mapLogger[logger.strName] = logger;
            }

            return true;
        }

        SLogLayout* SLogConfig::GetLayout(_lpcstr name)
        {
            map<lstring, SLogLayout>::iterator it = m_mapLayouts.find(name);
            if (it != m_mapLayouts.end()) return &it->second;
            return NULL;
        }

        SLogAppender* SLogConfig::GetAppender(_lpcstr name)
        {
            map<lstring, SLogAppender>::iterator it = m_mapAppenders.find(name);
            if (it != m_mapAppenders.end()) return &it->second;
            return NULL;
        }

        SLogger* SLogConfig::GetLogger(_lpcstr name)
        {
            map<lstring, SLogger>::iterator it = m_mapLogger.find(name);
            if (it != m_mapLogger.end()) return &it->second;
            return NULL;
        }

        //////////////////////////////////////////////////////////////////////////

        bool CLogManager::m_bInited = false;
        SLogConfig CLogManager::m_logConfig;
        list<CLogManager::SLogEntity> CLogManager::m_lstLogEntitysTmp;
        list<CLogManager::SLogEntity> CLogManager::m_lstLogEntitys;
        CMutexLock CLogManager::m_mtxLogEntityTmp;
        CMutexLock CLogManager::m_mtxLogEntity;
        Time m_tmStart;

        CLogManager::CLogManager() : CThread(false)
        {
        }

        CLogManager::~CLogManager()
        {
        }

        void CLogManager::Init(_lpcstr configFile)
        {
            if (!m_logConfig.Parse(configFile)) return;
            m_bInited = true;
        }

        bool CLogManager::IsInited()
        {
            return m_bInited;
        }

        _loggerptr CLogManager::GetLogger(_lpcstr lpstrLoggerName)
        {
            return m_logConfig.GetLogger(lpstrLoggerName);
        }

        void CLogManager::SetLogLevel(_loggerptr pLogger, ELOG_LEVEL eLevel)
        {
            pLogger->eLevel = eLevel;
        }

        void CLogManager::Destroy()
        {
            for (map<lstring, SLogAppender>::iterator it = m_logConfig.m_mapAppenders.begin(); it != m_logConfig.m_mapAppenders.end(); it++)
            {
                SLogAppender& appender = it->second;
                if (appender.fp != NULL)
                {
                    fclose(appender.fp);
                    appender.fp = NULL;
                }
            }
        }

        void CLogManager::Log(_loggerptr logger, ELOG_LEVEL level, _lpcstr file, int line, _lpcstr function, int threadId, ...)
        {
            if (!m_bInited || logger == NULL) return;

            m_mtxLogEntityTmp.Lock();
            {
                SLogEntity entity;
                entity.logger = logger;
                entity.level = level;
                entity.file = file;
                entity.line = line;
                entity.function = function;
                entity.threadId = threadId;

                va_list args;
                va_start(args, threadId); // last fixed param

                char* format = va_arg(args, char*); // first arg
#ifdef WIN32
                int len = vsnprintf(NULL, 0, format, args); // next args
                char* pbuf = (char*)malloc(len + 1);
                memset(pbuf, 0, len + 1);
                vsnprintf(pbuf, len, format, args);
#else
                char* pbuf = NULL;
                vasprintf(&pbuf, format, args);
#endif
                va_end(args);

                entity.msg = pbuf;
                free(pbuf);

                m_lstLogEntitysTmp.push_back(entity);
            }
            m_mtxLogEntityTmp.Unlock();
        }

        void CLogManager::OnExecute()
        {
            m_mtxLogEntityTmp.Lock();
            list<SLogEntity>::iterator it = m_lstLogEntitysTmp.begin();
            while (it != m_lstLogEntitysTmp.end() && m_lstLogEntitys.size() < 1000)
            {
                SLogEntity& entity = *it;
                m_lstLogEntitys.push_back(entity);
                it = m_lstLogEntitysTmp.erase(it);
            }
            m_mtxLogEntityTmp.Unlock();

            lstring str;
            m_mtxLogEntity.Lock();
            it = m_lstLogEntitys.begin();
            while (!m_lstLogEntitys.empty())
            {
                SLogEntity& entity = *it;
                _loggerptr logger = entity.logger;
                if (logger == NULL) continue;
                SLogAppender& appender = logger->appender;

                //
                switch (appender.eType)
                {
                    case APPENDER_CONSOLE:
                        {
                            if (appender.fp == NULL)
                                appender.fp = stdout;
                        }
                        break;
                    case APPENDER_FILE:
                        {
                            switch (appender.eRooling)
                            {
                                case NO_ROLLING:
                                    break;
                                case DAILY_ROLLING:
                                    {
                                        // rolling
                                        str = Time::GetCurDateStr();
                                        if (str > appender.strLogDate)
                                        {
                                            if (appender.fp != NULL) fclose(appender.fp);
                                            os::rename(appender.strFile, str + "_" + appender.strFile);
                                            appender.fp = NULL;
                                            appender.strLogDate = str;
                                        }
                                    }
                                    break;
                                case FILE_ROLLING:
                                    {
                                        // rolling
                                        if (appender.nWritedSize >= appender.nMaxFileSize)
                                        {
                                            if (appender.fp != NULL) fclose(appender.fp);
                                            for (int i = appender.nMaxFileCounts - 1; i <= appender.nLogFileIndex; i++)
                                                os::rm(appender.strFile + lstring().format(".%d", i));
                                            for (int i = appender.nLogFileIndex; i >= 0; i--)
                                            {
                                                if (i == 0)
                                                    os::rename(appender.strFile,
                                                               appender.strFile + lstring().format(".%d", i + 1));
                                                else
                                                    os::rename(appender.strFile + lstring().format(".%d", i),
                                                               appender.strFile + lstring().format(".%d", i + 1));
                                            }
                                            appender.fp = NULL;
                                            appender.nLogFileIndex = min(appender.nLogFileIndex + 1, appender.nMaxFileCounts - 1);
                                            appender.nWritedSize = 0;
                                        }
                                    }
                                    break;
                            }
                            if (appender.fp == NULL)
                                appender.fp = fopen(appender.strFile, "a+");
                        }
                        break;
                }

                //
                if (entity.level <= logger->eLevel)
                {
                    char* strlvl;
                    switch (entity.level)
                    {
                        case LOG_LEVEL_TRACE: strlvl = "TRACE"; break;
                        case LOG_LEVEL_DEBUG: strlvl = "DEBUG"; break;
                        case LOG_LEVEL_INFO: strlvl = "INFO"; break;
                        case LOG_LEVEL_WARN: strlvl = "WARN"; break;
                        case LOG_LEVEL_ERROR: strlvl = "ERROR"; break;
                        case LOG_LEVEL_FATAL: strlvl = "FATAL"; break;
                    }

                    //
                    str = logger->layout.strFormat;
                    Time tm;
                    str.replace("%d", tm.GetDateStr().c_str());
                    str.replace("%t", tm.GetTimeStr().c_str());
                    str.replace("%ms", lstring().format("%d", tm.GetMilliSec()).c_str());
                    str.replace("%T", tm.GetDateTimeStr(true).c_str());
                    str.replace("%f", entity.file);
                    str.replace("%l", lstring().format("%d", entity.line).c_str());
                    str.replace("%F", entity.function);
                    str.replace("%L", strlvl);
                    str.replace("%p", lstring().format("0x%08x", entity.threadId).c_str());
                    str.replace("%r", lstring().format("%d", tm.BetweenAllMilliSec(m_tmStart)).c_str());
                    str.replace("%m", entity.msg);
                    str.append("\n");

                    appender.nWritedSize += fwrite(str, 1, str.length(), appender.fp);
                }

                m_lstLogEntitys.erase(it++);
            }
            m_mtxLogEntity.Unlock();
        }

#endif // USE_LOG4CXX

        //////////////////////////////////////////////////////////////////////////
        LSLIB_API void InitLogger(_lpcstr lpstrCfgXml)
        {
            if (is_empty(lpstrCfgXml)) return;
#ifdef USE_LOG4CXX
            log4cxx::xml::DOMConfigurator::configure(lpstrCfgXml);
#else
            static CLogManager sLogManager;
            if (!sLogManager.IsInited())
                sLogManager.Init(lpstrCfgXml);
#endif
        }

        LSLIB_API void DestroyLogger()
        {
#ifdef USE_LOG4CXX
#else
            CLogManager::Destroy();
#endif
        }

        LSLIB_API _loggerptr GetLogger(_lpcstr lpstrLoggerName)
        {
            if (is_empty(lpstrLoggerName)) return (int)NULL;
#ifdef USE_LOG4CXX
            return Logger::getLogger(lpstrLoggerName);
#else
            return CLogManager::GetLogger(lpstrLoggerName);
#endif
        }

        LSLIB_API void SetLogLevel(_loggerptr pLogger, ELOG_LEVEL eLevel)
        {
            if (pLogger == NULL) return;

#ifdef USE_LOG4CXX
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
#else
            CLogManager::SetLogLevel(pLogger, eLevel);
#endif
        }

        //////////////////////////////////////////////////////////////////////////
        /*LSLIB_API */ extern _loggerptr g_logger = 0; // GetInternalLogger();
        /*LSLIB_API */ extern _loggerptr g_netlogger = 0; // GetInternalLogger();

        LSLIB_API void RegistGlobalLogger(_loggerptr pLogger)
        {
            g_logger = pLogger;
        }

        LSLIB_API void RegistNetLogger(_loggerptr pLogger)
        {
            g_netlogger = pLogger;
        }

    } // log

} // lslib
