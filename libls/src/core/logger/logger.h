#ifndef __LOGGER_H__
#define __LOGGER_H__
#pragma once

#ifdef USE_LOG4CXX
#pragma warning(disable:4231)
#include <log4cxx/logger.h>
using namespace log4cxx;
#else
#include "thread/thread.h"
#endif

namespace lslib
{
    /** @brief 日志操作相关API
        @details 使用log4cxx时，配置文件参考log4cxx说明。使用内部实现时，配置如下：
        @code{.xml}
        <logger version="1.0">
            <!--
            @name:              name of layout
            @format:            log format.
            %d      date
            %t      time
            %ms     millisecond
            %T      date time with millisecond
            %f      file
            %l      line
            %F      function
            %L      log level
            %p      thread id
            %r      milliseconds now from start up
            %m      messges
            -->
            <layout name="layout_app" format="[%L] [%T] [%r] (%p, %f:%l) [%F] - %m" />

            <!--
            @name:              name of appender
            @type:              values: [console, file]
            @file               for @type=file. file path for log output
            @rooling:           for @type=file. values: [no_rolling, daily_rooling, file_rooling]
            @date_pattern       for @rooling=daily_rooling. file pattern for backup log file name
            @max_file_size      for @rooling=file_rooling. file size for each log file
            @max_file_counts    for @rooling=file_rooling. max backup file counts
            -->
            <appender name="appender_app" type="console" file="log/logapp.log" rolling="file_rooling" date_pattern="yyyyMMdd_{}" max_file_size="30MB" max_file_counts="10"></appender>

            <!--
            @name:              name of logger
            @level:             log level. values: [trace, debug, info, warn, error, fatal, all]
            @appender:          reference of appender name
            -->
            <logger name="logger_app" level="all" layout="layout_app" appender="appender_app"></logger>
        </logger>
        @endcode
        日志配置分为3个模块，layout, appender, logger
        其中
        - layout    配置日志打印格式
        - appender  配置日志输入输出配置
        - logger    配置日志实例，不同配置表示不同日志实例，程序对实例进行操作
    */
    namespace logger
    {
        /// 日志级别
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
        typedef log4cxx::LoggerPtr _loggerptr;
        LSLIB_API void Log(_loggerptr logger, ELOG_LEVEL level,  _lpcstr file, int line, _lpcstr function, int threadId, ...);
#else

        //////////////////////////////////////////////////////////////////////////
        struct SLogLayout
        {
            string strName;
            string strFormat;
        };

        //////////////////////////////////////////////////////////////////////////
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

        //////////////////////////////////////////////////////////////////////////
        struct SLogger
        {
            string strName;
            ELOG_LEVEL eLevel;
            SLogLayout layout;
            SLogAppender appender;
        };
        typedef SLogger* _loggerptr;

        //////////////////////////////////////////////////////////////////////////
        struct SLogConfig
        {
            string m_strFile;
            map<string, SLogLayout> m_mapLayouts;
            map<string, SLogAppender> m_mapAppenders;
            map<string, SLogger> m_mapLogger;

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
            virtual _lpcstr GetName() { return "CLogManager"; }
            virtual void OnExecute();

        public:
            static void Init(_lpcstr configFile);
            static bool IsInited();
            static void Destroy();
            static _loggerptr GetLogger(_lpcstr lpstrLoggerName);
            static void SetLogLevel(_loggerptr pLogger, ELOG_LEVEL eLevel);
            LSLIB_API static void Log(_loggerptr logger, ELOG_LEVEL level,  _lpcstr file, int line, _lpcstr function, int threadId, ...);

        private:
            static bool m_bInited;
            static SLogConfig m_logConfig;
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
            static list<SLogEntity> m_lstLogEntitysTmp;
            static list<SLogEntity> m_lstLogEntitys;
            static CMutexLock m_mtxLogEntityTmp;
            static CMutexLock m_mtxLogEntity;
        };

#endif // USE_LOG4CXX

        //////////////////////////////////////////////////////////////////////////

        /// @brief 初始化日志模块
        /// @param lpstrCfgXml 日志配置文件路径
        LSLIB_API void InitLogger(_lpcstr lpstrCfgXml);

        /// 销毁日志模块
        LSLIB_API void DestroyLogger();

        /// @brief 获取日志实例
        /// @param lpstrLoggerName 配置文件路径中对应的日志实例名称
        LSLIB_API _loggerptr GetLogger(_lpcstr lpstrLoggerName);

        /// @brief 设置日志级别
        /// @param pLogger 日志实例
        /// @param eLevel 日志级别
        LSLIB_API void SetLogLevel(_loggerptr pLogger, ELOG_LEVEL eLevel);

        /**
         * params:
         *   logger: (_loggerptr) logger instance
         *   fmt: (const char*) log string format
         */
#ifdef USE_LOG4CXX
#define FATAL_LOG(logger, ...)             Log(logger, LOG_LEVEL_FATAL, __FILE__, __LINE__, __FUNCTION__, __THREAD__, __VA_ARGS__)
#define ERROR_LOG(logger, ...)             Log(logger, LOG_LEVEL_ERROR, __FILE__, __LINE__, __FUNCTION__, __THREAD__, __VA_ARGS__)
#define WARN_LOG(logger, ...)              Log(logger, LOG_LEVEL_WARN, __FILE__, __LINE__, __FUNCTION__, __THREAD__, __VA_ARGS__)
#define INFO_LOG(logger, ...)              Log(logger, LOG_LEVEL_INFO, __FILE__, __LINE__, __FUNCTION__, __THREAD__, __VA_ARGS__)
#define DEBUG_LOG(logger, ...)             Log(logger, LOG_LEVEL_DEBUG, __FILE__, __LINE__, __FUNCTION__, __THREAD__, __VA_ARGS__)
#define TRACE_LOG(logger, ...)             Log(logger, LOG_LEVEL_TRACE, __FILE__, __LINE__, __FUNCTION__, __THREAD__, __VA_ARGS__)
#else
#define FATAL_LOG(logger, ...)             CLogManager::Log(logger, LOG_LEVEL_FATAL, __FILE__, __LINE__, __FUNCTION__, __THREAD__, __VA_ARGS__)
#define ERROR_LOG(logger, ...)             CLogManager::Log(logger, LOG_LEVEL_ERROR, __FILE__, __LINE__, __FUNCTION__, __THREAD__, __VA_ARGS__)
#define WARN_LOG(logger, ...)              CLogManager::Log(logger, LOG_LEVEL_WARN, __FILE__, __LINE__, __FUNCTION__, __THREAD__, __VA_ARGS__)
#define INFO_LOG(logger, ...)              CLogManager::Log(logger, LOG_LEVEL_INFO, __FILE__, __LINE__, __FUNCTION__, __THREAD__, __VA_ARGS__)
#define DEBUG_LOG(logger, ...)             CLogManager::Log(logger, LOG_LEVEL_DEBUG, __FILE__, __LINE__, __FUNCTION__, __THREAD__, __VA_ARGS__)
#define TRACE_LOG(logger, ...)             CLogManager::Log(logger, LOG_LEVEL_TRACE, __FILE__, __LINE__, __FUNCTION__, __THREAD__, __VA_ARGS__)
#endif // USE_LOG4CXX

//////////////////////////////////////////////////////////////////////////
        /*LSLIB_API */extern _loggerptr g_logger; // 全局日志实例
        /*LSLIB_API */extern _loggerptr g_netlogger; // 全局网络日志实例

        /// @brief 注册全局日志实例
        /// @param pLogger 通过 ::GetLogger 获取的日志实例
        /// @details 注册全局日志实例后，函数库内部的日志打印也将输出到日志文件，否则不输出
        LSLIB_API void RegistGlobalLogger(_loggerptr pLogger);

        /// @brief 注册网络日志实例
        /// @param pLogger 通过 ::GetLogger 获取的日志实例
        /// @details 注册全局日志实例后，函数库内部网络相关的日志打印也将输出到日志文件，否则不输出
        LSLIB_API void RegistNetLogger(_loggerptr pLogger);

    } // namespace logger

} // namespace lslib


#endif // __LOGGER_H__
