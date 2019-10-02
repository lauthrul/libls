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

        void Log(_loggerptr logger, ELOG_LEVEL level, lpcstr file, int line, lpcstr function, int threadId, ...)
        {
            if (logger == 0) return;

            va_list args;
            va_start(args, threadId); // last fixed param
            char* format = va_arg(args, char*); // first arg
            string str = strtool::format(format, args);
            va_end(args);

            ::log4cxx::spi::LocationInfo location(file, function, line);
            switch (level)
            {
                case LOG_LEVEL_FATAL:
                    if (logger->isFatalEnabled())
                        logger->forcedLog(::log4cxx::Level::getFatal(), str, location);
                    break;
                case LOG_LEVEL_ERROR:
                    if (logger->isErrorEnabled())
                        logger->forcedLog(::log4cxx::Level::getError(), str, location);
                    break;
                case LOG_LEVEL_WARN:
                    if (logger->isWarnEnabled())
                        logger->forcedLog(::log4cxx::Level::getWarn(), str, location);
                    break;
                case LOG_LEVEL_INFO:
                    if (logger->isInfoEnabled())
                        logger->forcedLog(::log4cxx::Level::getInfo(), str, location);
                    break;
                case LOG_LEVEL_DEBUG:
                    if (logger->isDebugEnabled())
                        logger->forcedLog(::log4cxx::Level::getDebug(), str, location);
                    break;
                case LOG_LEVEL_TRACE:
                    if (logger->isTraceEnabled())
                        logger->forcedLog(::log4cxx::Level::getTrace(), str, location);
                    break;
            }
        }

#else
        //////////////////////////////////////////////////////////////////////////
        struct SLogLayout
        {
            string strName;
            string strFormat;
        };

        enum ELogAppenderType
        {
            APPENDER_CONSOLE, APPENDER_FILE
        };

        enum ELogAppenderRoolling
        {
            NO_ROLLING,
            FILE_ROLLING,
            DAILY_ROLLING
        };

        struct SLogAppender
        {
            string strName;
            ELogAppenderType eType;
            string strFile;
            ELogAppenderRoolling eRooling;
            string strDatePattern;
            int nMaxFileSize;
            int nMaxFileCounts;

            // program data
            string strLogDate;
            int nLogFileIndex;
            int nWritedSize;
            FILE* fp;

            SLogAppender() : nLogFileIndex(-1), nWritedSize(-1), fp(NULL) {};
        };

        struct SLogger
        {
            string strName;
            ELOG_LEVEL eLevel;
            SLogLayout layout;
            SLogAppender appender;
        };

        struct SLogConfig
        {
            string m_strFile;
            map<string, SLogLayout> m_mapLayouts;
            map<string, SLogAppender> m_mapAppenders;
            map<string, SLogger> m_mapLogger;

            SLogConfig();
            SLogConfig(lpcstr lpstrFilePath);

            bool Parse();
            bool Parse(lpcstr lpstrFilePath);

            SLogLayout* GetLayout(lpcstr name);
            SLogAppender* GetAppender(lpcstr name);
            SLogger* GetLogger(lpcstr name);
        };

        struct SLogEntity
        {
            _loggerptr logger;
            ELOG_LEVEL level;
            string file;
            int line;
            string function;
            int threadId;
            string msg;
        };
        define_stl_type(SLogEntity);

        //////////////////////////////////////////////////////////////////////////
        class CLogManager : public CThread
        {
        public:
            CLogManager();
            virtual ~CLogManager();

        public:
            virtual lpcstr GetName() { return "CLogManager"; }
            virtual void OnExecute();

        public:
            static void Init(lpcstr configFile);
            static bool IsInited();
            static void Destroy();
            static _loggerptr GetLogger(lpcstr lpstrLoggerName);
            static void SetLogLevel(_loggerptr pLogger, ELOG_LEVEL eLevel);
            static void Log(_loggerptr logger, ELOG_LEVEL level, lpcstr file, int line, lpcstr function, int threadId, ...);
            static void Log(const SLogEntity& entity);

        private:
            static bool m_bInited;
            static SLogConfig m_logConfig;
            static SLogEntity_list m_lstLogEntitysTmp;
            static SLogEntity_list m_lstLogEntitys;
            static CMutexLock m_mtxLogEntityTmp;
            static CMutexLock m_mtxLogEntity;
            static Time m_tmStart;
        };

        bool CLogManager::m_bInited = false;
        SLogConfig CLogManager::m_logConfig;
        SLogEntity_list CLogManager::m_lstLogEntitysTmp;
        SLogEntity_list CLogManager::m_lstLogEntitys;
        CMutexLock CLogManager::m_mtxLogEntityTmp;
        CMutexLock CLogManager::m_mtxLogEntity;
        Time CLogManager::m_tmStart;

        //////////////////////////////////////////////////////////////////////////
        SLogConfig::SLogConfig()
        {
        }

        SLogConfig::SLogConfig(lpcstr lpstrFilePath) : m_strFile(lpstrFilePath)
        {
        }

        bool SLogConfig::Parse()
        {
            return Parse(m_strFile.c_str());
        }

        bool SLogConfig::Parse(lpcstr lpstrFilePath)
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

            string str;
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
                    appender.nMaxFileSize = strtool::to_int(str) * 1024 * 1024;
                }
                else if (str.find("KB"))
                {
                    str = str.substr(0, str.length() - 2);
                    appender.nMaxFileSize = strtool::to_int(str) * 1024;
                }
                else
                {
                    appender.nMaxFileSize = strtool::to_int(str);
                }

                str = Xml::GetAttribute(*pNode, "max_file_counts");
                appender.nMaxFileCounts = strtool::to_int(str);

                // init data
                {
                    os::mkdir(os::path_get_dir(appender.strFile.c_str()).c_str());

                    if (appender.strLogDate.empty())
                    {
                        string writeTime = os::get_file_attr(appender.strFile.c_str()).writeTime;
                        if (!writeTime.empty())
                        {
                            Time tm; tm.FromStr(writeTime.c_str());
                            appender.strLogDate = tm.GetDateStr();
                        }
                        else appender.strLogDate = Time::CurrentDateStr();
                    }

                    if (appender.nWritedSize < 0)
                        appender.nWritedSize = os::get_file_size(appender.strFile.c_str());

                    if (appender.nLogFileIndex < 0)
                    {
                        os::enum_file_array arr_files;
                        os::enumerate_files(arr_files,
                                            os::path_get_dir(appender.strFile.c_str()).c_str(),
                                            NULL,
                                            os::path_get_name(appender.strFile.c_str()).c_str(),
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
                SLogLayout* pLayout = GetLayout(str.c_str());
                if (pLayout) logger.layout = *pLayout;

                str = Xml::GetAttribute(*pNode, "appender");
                SLogAppender* pAppender = GetAppender(str.c_str());
                if (pAppender) logger.appender = *pAppender;
                m_mapLogger[logger.strName] = logger;
            }

            return true;
        }

        SLogLayout* SLogConfig::GetLayout(lpcstr name)
        {
            map<string, SLogLayout>::iterator it = m_mapLayouts.find(name);
            if (it != m_mapLayouts.end()) return &it->second;
            return NULL;
        }

        SLogAppender* SLogConfig::GetAppender(lpcstr name)
        {
            map<string, SLogAppender>::iterator it = m_mapAppenders.find(name);
            if (it != m_mapAppenders.end()) return &it->second;
            return NULL;
        }

        SLogger* SLogConfig::GetLogger(lpcstr name)
        {
            map<string, SLogger>::iterator it = m_mapLogger.find(name);
            if (it != m_mapLogger.end()) return &it->second;
            return NULL;
        }

        //////////////////////////////////////////////////////////////////////////

        CLogManager::CLogManager() : CThread(false)
        {
        }

        CLogManager::~CLogManager()
        {
        }

        void CLogManager::Init(lpcstr configFile)
        {
            if (!m_logConfig.Parse(configFile)) return;
            m_bInited = true;
        }

        bool CLogManager::IsInited()
        {
            return m_bInited;
        }

        _loggerptr CLogManager::GetLogger(lpcstr lpstrLoggerName)
        {
            return m_logConfig.GetLogger(lpstrLoggerName);
        }

        void CLogManager::SetLogLevel(_loggerptr pLogger, ELOG_LEVEL eLevel)
        {
            pLogger->eLevel = eLevel;
        }

        void CLogManager::Destroy()
        {
            for (map<string, SLogAppender>::iterator it = m_logConfig.m_mapAppenders.begin(); it != m_logConfig.m_mapAppenders.end(); it++)
            {
                SLogAppender& appender = it->second;
                if (appender.fp != NULL)
                {
                    fclose(appender.fp);
                    appender.fp = NULL;
                }
            }
        }

        void CLogManager::Log(_loggerptr logger, ELOG_LEVEL level, lpcstr file, int line, lpcstr function, int threadId, ...)
        {
            if (/*!m_bInited || */logger == NULL) return;

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
                entity.msg = strtool::format(format, args);
                va_end(args);

                m_lstLogEntitysTmp.push_back(entity);
            }
            m_mtxLogEntityTmp.Unlock();
        }

        void CLogManager::Log(const SLogEntity& entity)
        {
            if (/*!m_bInited || */entity.logger == NULL) return;

            m_mtxLogEntityTmp.Lock();
            m_lstLogEntitysTmp.push_back(entity);
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

            string str;
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
                                        str = Time::CurrentDateStr();
                                        if (str > appender.strLogDate)
                                        {
                                            if (appender.fp != NULL) fclose(appender.fp);
                                            os::rename(appender.strFile.c_str(),
                                                       strtool::format("%s_%s", str.c_str(), appender.strFile.c_str()).c_str());
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
                                                os::rm(strtool::format("%s.%d", appender.strFile.c_str(), i).c_str());
                                            for (int i = appender.nLogFileIndex; i >= 0; i--)
                                            {
                                                if (i == 0)
                                                    os::rename(appender.strFile.c_str(),
                                                               strtool::format("%s.%d", appender.strFile.c_str(), i + 1).c_str());
                                                else
                                                    os::rename(strtool::format("%s.%d", appender.strFile.c_str(), i).c_str(),
                                                               strtool::format("%s.%d", appender.strFile.c_str(), i + 1).c_str());
                                            }
                                            appender.fp = NULL;
                                            appender.nLogFileIndex = MIN(appender.nLogFileIndex + 1, appender.nMaxFileCounts - 1);
                                            appender.nWritedSize = 0;
                                        }
                                    }
                                    break;
                            }
                            if (appender.fp == NULL)
                                appender.fp = fopen(appender.strFile.c_str(), "a+");
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
                    str = strtool::replace(str, "%d", tm.GetDateStr());
                    str = strtool::replace(str, "%t", tm.GetTimeStr());
                    str = strtool::replace(str, "%ms", strtool::format("%d", tm.GetMilliSec()));
                    str = strtool::replace(str, "%T", tm.GetDateTimeStr(true));
                    str = strtool::replace(str, "%f", entity.file);
                    str = strtool::replace(str, "%l", strtool::format("%d", entity.line));
                    str = strtool::replace(str, "%F", entity.function);
                    str = strtool::replace(str, "%L", strlvl);
                    str = strtool::replace(str, "%p", strtool::format("0x%08x", entity.threadId));
                    str = strtool::replace(str, "%r", strtool::format("%d", tm.BetweenAllMilliSec(m_tmStart)));
                    str = strtool::replace(str, "%m", entity.msg);
                    str.append("\n");

                    appender.nWritedSize += fwrite(str.c_str(), 1, str.length(), appender.fp);
                }

                m_lstLogEntitys.erase(it++);
            }
            m_mtxLogEntity.Unlock();
        }

        void Log(_loggerptr logger, ELOG_LEVEL level, lpcstr file, int line, lpcstr function, int threadId, ...)
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
            entity.msg = strtool::format(format, args);
            va_end(args);

            CLogManager::Log(entity);
        }

#endif // USE_LOG4CXX

        //////////////////////////////////////////////////////////////////////////
        LSLIB_API void InitLogger(lpcstr lpstrCfgXml)
        {
            if (strtool::is_empty(lpstrCfgXml)) return;
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

        LSLIB_API _loggerptr GetLogger(lpcstr lpstrLoggerName)
        {
            if (strtool::is_empty(lpstrLoggerName)) return (int)NULL;
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
