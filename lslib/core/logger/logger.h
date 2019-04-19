#pragma once
#include <log4cxx/xml/domconfigurator.h> 
#include <log4cxx/patternlayout.h> 
#include <log4cxx/rolling/rollingfileappender.h> 
#include <log4cxx/rolling/fixedwindowrollingpolicy.h> 
#include <log4cxx/rolling/filterbasedtriggeringpolicy.h> 
#include <log4cxx/filter/levelrangefilter.h> 
#include <log4cxx/helpers/pool.h> 
#include <log4cxx/logger.h> 
#include <log4cxx/propertyconfigurator.h> 
#include <log4cxx/dailyrollingfileappender.h> 
#include <log4cxx/helpers/stringhelper.h> 

using namespace log4cxx; 
using namespace log4cxx::rolling; 
using namespace log4cxx::xml; 
using namespace log4cxx::filter; 
using namespace log4cxx::helpers; 

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
		*	logger: (LoggerPtr) logger instance
		*	format: (const char*) log string format
		*/
		#ifndef _DEBUG
		#define FATAL_LOG(logger, format, ...)				LOG4CXX_FATAL(logger, (lstring("[%s] - ") + (format)), __FUNCTION__, __VA_ARGS__)
		#define ERROR_LOG(logger, format, ...)				LOG4CXX_ERROR(logger, (lstring("[%s] - ") + (format)), __FUNCTION__, __VA_ARGS__)
		#define WARN_LOG(logger, format, ...)				LOG4CXX_WARN(logger, (lstring("[%s] - ") + (format)), __FUNCTION__, __VA_ARGS__)
		#define INFO_LOG(logger, format, ...)				LOG4CXX_INFO(logger, (lstring("[%s] - ") + (format)), __FUNCTION__, __VA_ARGS__)
		#define DEBUG_LOG(logger, format, ...)				LOG4CXX_DEBUG(logger, (lstring("[%s] - ") + (format)), __FUNCTION__, __VA_ARGS__)
		#define TRACE_LOG(logger, format, ...)				LOG4CXX_TRACE(logger, (lstring("[%s] - ") + (format)), __FUNCTION__, __VA_ARGS__)
		#else
		#define FATAL_LOG(logger, format, ...)				LOG4CXX_FATAL(logger, format, __VA_ARGS__)
		#define ERROR_LOG(logger, format, ...)				LOG4CXX_ERROR(logger, format, __VA_ARGS__)
		#define WARN_LOG(logger, format, ...)				LOG4CXX_WARN(logger, format, __VA_ARGS__)
		#define INFO_LOG(logger, format, ...)				LOG4CXX_INFO(logger, format, __VA_ARGS__)
		#define DEBUG_LOG(logger, format, ...)				LOG4CXX_DEBUG(logger, format, __VA_ARGS__)
		#define TRACE_LOG(logger, format, ...)				LOG4CXX_TRACE(logger, format, __VA_ARGS__)
		#endif

		//////////////////////////////////////////////////////////////////////////
		LSLIB_API UILIB_COMDAT _loggerptr g_logger;
		LSLIB_API UILIB_COMDAT _loggerptr g_netlogger;
		LSLIB_API void RegistGlobalLogger(_loggerptr pLogger);
		LSLIB_API void RegistNetLogger(_loggerptr pLogger);

	} // log

} // lslib
