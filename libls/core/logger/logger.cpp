#pragma once
#include "stdafx.h"
#include "logger.h"

namespace lslib
{
    namespace logger
    {

#ifdef USE_LOG4CXX

// #include <log4cxx/consoleappender.h>
// #include <log4cxx/basicconfigurator.h>
// #include <log4cxx/simplelayout.h>
// #include <log4cxx/xml/domconfigurator.h>
//         //////////////////////////////////////////////////////////////////////////
//         _loggerptr loggerInternal = NULL;
//         _loggerptr GetInternalLogger()
//         {
//             if (loggerInternal == NULL)
//             {
//                 log4cxx::ConsoleAppender* consoleAppender = new log4cxx::ConsoleAppender(log4cxx::LayoutPtr(new log4cxx::SimpleLayout()));
//                 log4cxx::BasicConfigurator::configure(log4cxx::AppenderPtr(consoleAppender));
//                 loggerInternal = log4cxx::Logger::getRootLogger();
//             }
//             return loggerInternal;
//         }
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

                SLogAppender appender;
                appender.strName = Xml::GetAttribute(*pNode, "name");

                str = Xml::GetAttribute(*pNode, "type");
                if (str == "console") appender.eType = SLogAppender::ELogAppenderType::CONSOLE;
                else if (str == "file") appender.eType = SLogAppender::ELogAppenderType::FILE;

                appender.strFile = Xml::GetAttribute(*pNode, "file");

                str = Xml::GetAttribute(*pNode, "rolling");
                if (str == "no_rolling") appender.eRooling = SLogAppender::ELogAppenderRoolling::NO_ROLLING;
                else if (str == "daily_rooling") appender.eRooling = SLogAppender::ELogAppenderRoolling::DAILY_ROLLING;
                else if (str == "file_rooling") appender.eRooling = SLogAppender::ELogAppenderRoolling::FILE_ROLLING;

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
            for (map<lstring, SLogger>::iterator it = m_logConfig.m_mapLogger.begin(); it != m_logConfig.m_mapLogger.end(); it++)
            {
                SLogger& logger = it->second;
                if (logger.fp != NULL)
                {
                    fclose(logger.fp);
                    logger.fp = NULL;
                }
            }
        }

        void CLogManager::Log(_loggerptr logger, ELOG_LEVEL level, _lpcstr file, int line, int threadId, const char* format, ...)
        {
            if (!m_bInited) return;

            m_mtxLogEntityTmp.Lock();
            {
                SLogEntity entity;
                entity.logger = logger;
                entity.level = level;
                entity.file = file;
                entity.line = line;
                entity.threadId = threadId;

                va_list args;
                va_start(args, format);
                int len = vsnprintf(NULL, 0, format, args);
                char* pbuf = (char*)malloc(len + 1);
                memset(pbuf, 0, len + 1);
                vsnprintf(pbuf, len, format, args);
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

            m_mtxLogEntity.Lock();
            it = m_lstLogEntitys.begin();
            while (!m_lstLogEntitys.empty())
            {
                SLogEntity& entity = *it;
                _loggerptr logger = entity.logger;
                if (logger == NULL) continue;

                //
                switch (logger->appender.eType)
                {
                    case SLogAppender::ELogAppenderType::CONSOLE:
                        {
                            if (logger->fp == NULL)
                                logger->fp = stdout;
                        }
                        break;
                    case SLogAppender::ELogAppenderType::FILE:
                        {
                            switch (logger->appender.eRooling)
                            {
                                case SLogAppender::ELogAppenderRoolling::NO_ROLLING:
                                    break;
                                case SLogAppender::ELogAppenderRoolling::DAILY_ROLLING:
                                    {
                                        // init data
                                        if (logger->strLogDate.empty())
                                        {
                                            lstring writeTime = os::get_file_attr(logger->appender.strFile).writeTime;
                                            if (!writeTime.empty())
                                            {
                                                Time tm; tm.Parser(writeTime);
                                                logger->strLogDate = tm.GetDateStr();
                                            }
                                            else logger->strLogDate = Time::GetCurDateStr();
                                        }
                                        // rolling
                                        lstring strDate = Time::GetCurDateStr();
                                        if (strDate > logger->strLogDate)
                                        {
                                            if (logger->fp != NULL) fclose(logger->fp);
                                            os::rename(logger->appender.strFile, strDate + "_" + logger->appender.strFile);
                                            logger->fp = NULL;
                                            logger->strLogDate = strDate;
                                        }
                                    }
                                    break;
                                case SLogAppender::ELogAppenderRoolling::FILE_ROLLING:
                                    {
                                        // init data
                                        if (logger->nWritedSize < 0)
                                            logger->nWritedSize = os::get_file_size(logger->appender.strFile);
                                        if (logger->nLogFileIndex < 0)
                                        {
                                            os::enum_file_array arr_files;
                                            os::enumerate_files(arr_files,
                                                                os::path_get_dir(logger->appender.strFile) + "/*.*",
                                                                NULL,
                                                                os::path_get_name(logger->appender.strFile),
                                                                false);
                                            if (!arr_files.empty()) logger->nLogFileIndex = arr_files.size() - 1;
                                            else logger->nLogFileIndex = 0;
                                        }
                                        // rolling
                                        if (logger->nWritedSize >= logger->appender.nMaxFileSize)
                                        {
                                            if (logger->fp != NULL) fclose(logger->fp);
                                            for (int i = logger->appender.nMaxFileCounts - 1; i <= logger->nLogFileIndex; i++)
                                                os::rm(logger->appender.strFile + lstring().format(".%d", i));
                                            for (int i = logger->nLogFileIndex; i >= 0; i--)
                                            {
                                                if (i == 0)
                                                    os::rename(logger->appender.strFile,
                                                               logger->appender.strFile + lstring().format(".%d", i + 1));
                                                else
                                                    os::rename(logger->appender.strFile + lstring().format(".%d", i),
                                                               logger->appender.strFile + lstring().format(".%d", i + 1));
                                            }
                                            logger->fp = NULL;
                                            logger->nLogFileIndex = min(logger->nLogFileIndex + 1, logger->appender.nMaxFileCounts - 1);
                                            logger->nWritedSize = 0;
                                        }
                                    }
                                    break;
                            }
                            if (logger->fp == NULL)
                                logger->fp = fopen(logger->appender.strFile, "a+");
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
                    lstring msg = logger->layout.strFormat;
                    Time tm = Time::GetCurDateTime();
                    msg.replace("%d", tm.GetDateStr().c_str());
                    msg.replace("%t", tm.GetTimeStr().c_str());
                    msg.replace("%ms", lstring().format("%d", tm.GetMilliSec()));
                    msg.replace("%T", tm.GetDateTimeStr(true).c_str());
                    msg.replace("%f", entity.file);
                    msg.replace("%l", lstring().format("%d", entity.line));
                    msg.replace("%L", strlvl);
                    msg.replace("%p", lstring().format("0x%08x", entity.threadId));
                    msg.replace("%m", entity.msg);
                    msg.append("\n");

                    logger->nWritedSize += fwrite(msg, 1, msg.length(), logger->fp);
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
            if (is_empty(lpstrLoggerName)) return NULL;
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
        /*LSLIB_API */ extern _loggerptr g_logger = NULL; // GetInternalLogger();
        /*LSLIB_API */ extern _loggerptr g_netlogger = NULL; // GetInternalLogger();

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
