#pragma once
#include "stdafx.h"
#include "logger.h"

namespace lslib
{
	namespace logger
	{
		//////////////////////////////////////////////////////////////////////////
		LSLIB_API void InitLogger(_lpcstr lpstrCfgXml)
		{
			DOMConfigurator::configure(lpstrCfgXml);
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
			switch(eLevel)
			{
			case LOG_LEVEL_NONE:	level = log4cxx::Level::getOff();		break;
			case LOG_LEVEL_FATAL:	level = log4cxx::Level::getFatal();		break;
			case LOG_LEVEL_ERROR:	level = log4cxx::Level::getError();		break;
			case LOG_LEVEL_WARN:	level = log4cxx::Level::getWarn();		break;
			case LOG_LEVEL_INFO:	level = log4cxx::Level::getInfo();		break;
			case LOG_LEVEL_DEBUG:	level = log4cxx::Level::getDebug();		break;
			case LOG_LEVEL_TRACE:	level = log4cxx::Level::getTrace();		break;
			case LOG_LEVEL_ALL:		level = log4cxx::Level::getAll();		break;
			}
			pLogger->setLevel(level);
		}

		//////////////////////////////////////////////////////////////////////////
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
