#pragma once
#include "targetver.h"

//////////////////////////////////////////////////////////////////////////
#ifdef __MINGW32__
#else
#define USE_LOG4CXX
#define USE_LIBCURL
#define USE_LIBICONV
#endif

#include "libls.h"
using namespace lslib;
using namespace lslib::net;
using namespace lslib::logger;

#include "db/db_wrapper.h"
#include "business/NetManager.h"

//////////////////////////////////////////////////////////////////////////
#define Console                 printf

//////////////////////////////////////////////////////////////////////////
extern _loggerptr               g_pLogger;
extern _loggerptr               g_pNetlogger;
extern CDBWrapper               g_dbWrapper;
extern CNetManager              g_netManager;