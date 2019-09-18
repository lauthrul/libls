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
    /** @brief ��־���
        @details ʹ��log4cxxʱ�������ļ��ο�log4cxx˵����ʹ���ڲ�ʵ��ʱ���������£�
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
        ��־���÷�Ϊ3��ģ�飬layout, appender, logger
        ����
        - layout    ������־��ӡ��ʽ
        - appender  ������־�����������
        - logger    ������־ʵ������ͬ���ñ�ʾ��ͬ��־ʵ���������ʵ�����в���
    */
    namespace logger
    {
        /// ��־����
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

        /// @brief ��ʼ����־ģ��
        /// @param lpstrCfgXml ��־�����ļ�·��
        LSLIB_API void InitLogger(_lpcstr lpstrCfgXml);

        /// ������־ģ��
        LSLIB_API void DestroyLogger();

        /// @brief ��ȡ��־ʵ��
        /// @param lpstrLoggerName �����ļ�·���ж�Ӧ����־ʵ������
        LSLIB_API _loggerptr GetLogger(_lpcstr lpstrLoggerName);

        /// @brief ������־����
        /// @param pLogger ��־ʵ��
        /// @param eLevel ��־����
        LSLIB_API void SetLogLevel(_loggerptr pLogger, ELOG_LEVEL eLevel);


        /// @brief ��ӡ��־
        /// @param logger   ��־ʵ��
        /// @param level    ��־����
        /// @param file     �����ļ�
        /// @param line     ������
        /// @param function ���뺯��
        /// @param threadId �߳�ID
        /// @param ...      ��־���ݣ�֧�ָ�ʽ�����
        LSLIB_API void Log(_loggerptr logger, ELOG_LEVEL level, _lpcstr file, int line, _lpcstr function, int threadId, ...);

#define FATAL_LOG(logger, ...)             Log(logger, LOG_LEVEL_FATAL, __FILE__, __LINE__, __FUNCTION__, __THREAD__, __VA_ARGS__)
#define ERROR_LOG(logger, ...)             Log(logger, LOG_LEVEL_ERROR, __FILE__, __LINE__, __FUNCTION__, __THREAD__, __VA_ARGS__)
#define WARN_LOG(logger, ...)              Log(logger, LOG_LEVEL_WARN, __FILE__, __LINE__, __FUNCTION__, __THREAD__, __VA_ARGS__)
#define INFO_LOG(logger, ...)              Log(logger, LOG_LEVEL_INFO, __FILE__, __LINE__, __FUNCTION__, __THREAD__, __VA_ARGS__)
#define DEBUG_LOG(logger, ...)             Log(logger, LOG_LEVEL_DEBUG, __FILE__, __LINE__, __FUNCTION__, __THREAD__, __VA_ARGS__)
#define TRACE_LOG(logger, ...)             Log(logger, LOG_LEVEL_TRACE, __FILE__, __LINE__, __FUNCTION__, __THREAD__, __VA_ARGS__)

        //////////////////////////////////////////////////////////////////////////
        /*LSLIB_API */extern _loggerptr g_logger; // ȫ����־ʵ��
        /*LSLIB_API */extern _loggerptr g_netlogger; // ȫ��������־ʵ��

        /// @brief ע��ȫ����־ʵ��
        /// @param pLogger ͨ�� ::GetLogger ��ȡ����־ʵ��
        /// @details ע��ȫ����־ʵ���󣬺������ڲ�����־��ӡҲ���������־�ļ����������
        LSLIB_API void RegistGlobalLogger(_loggerptr pLogger);

        /// @brief ע��������־ʵ��
        /// @param pLogger ͨ�� ::GetLogger ��ȡ����־ʵ��
        /// @details ע��ȫ����־ʵ���󣬺������ڲ�������ص���־��ӡҲ���������־�ļ����������
        LSLIB_API void RegistNetLogger(_loggerptr pLogger);

    } // namespace logger

} // namespace lslib


#endif // __LOGGER_H__
