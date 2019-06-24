#include "testcase.hpp"
using namespace lslib::logger;

void test_logger()
{
#ifdef USE_LOG4CXX
    InitLogger("log4cxx.xml");
    _loggerptr lplogger = GetLogger("logapp");
    _loggerptr lpnetlogger = GetLogger("lognet");
#else
    InitLogger("logcfg.xml");
    _loggerptr lplogger = GetLogger("logger_app");
    _loggerptr lpnetlogger = GetLogger("logger_net");
#endif
    RegistGlobalLogger(lplogger);
    RegistNetLogger(lpnetlogger);
    SetLogLevel(lplogger, LOG_LEVEL_ALL);
    SetLogLevel(lpnetlogger, LOG_LEVEL_INFO);

    INFO_LOG(lplogger, "============================");
    INFO_LOG(lpnetlogger, "============================");
    for (int i = 0; i < 1000000; i++)
    {
        Time tm;
        DEBUG_LOG(lplogger, "log test, num:%d, string: %s", i, tm.GetDateTimeStr(true).c_str());
        DEBUG_LOG(lpnetlogger, "log test, num:%d, string: %s", i, tm.GetDateTimeStr(true).c_str());
    }
}
declare_test_case(test_logger);
