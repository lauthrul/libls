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
    /** @brief 日志相关
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
#else
        struct SLogger;
        typedef SLogger* _loggerptr;
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


        /// @brief 打印日志
        /// @param logger   日志实例
        /// @param level    日志级别
        /// @param file     代码文件
        /// @param line     代码行
        /// @param function 代码函数
        /// @param threadId 线程ID
        /// @param ...      日志内容，支持格式化输出
        LSLIB_API void Log(_loggerptr logger, ELOG_LEVEL level, _lpcstr file, int line, _lpcstr function, int threadId, ...);

#define FATAL_LOG(logger, ...)             Log(logger, LOG_LEVEL_FATAL, __FILE__, __LINE__, __FUNCTION__, __THREAD__, __VA_ARGS__)
#define ERROR_LOG(logger, ...)             Log(logger, LOG_LEVEL_ERROR, __FILE__, __LINE__, __FUNCTION__, __THREAD__, __VA_ARGS__)
#define WARN_LOG(logger, ...)              Log(logger, LOG_LEVEL_WARN, __FILE__, __LINE__, __FUNCTION__, __THREAD__, __VA_ARGS__)
#define INFO_LOG(logger, ...)              Log(logger, LOG_LEVEL_INFO, __FILE__, __LINE__, __FUNCTION__, __THREAD__, __VA_ARGS__)
#define DEBUG_LOG(logger, ...)             Log(logger, LOG_LEVEL_DEBUG, __FILE__, __LINE__, __FUNCTION__, __THREAD__, __VA_ARGS__)
#define TRACE_LOG(logger, ...)             Log(logger, LOG_LEVEL_TRACE, __FILE__, __LINE__, __FUNCTION__, __THREAD__, __VA_ARGS__)

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
