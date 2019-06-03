#include "testcase.hpp"
using namespace lslib::logger;

void test_logger()
{
    InitLogger("log4cxx.xml");
    RegistGlobalLogger(GetLogger("applogger"));
    RegistNetLogger(GetLogger("netlogger"));
    SetLogLevel(g_logger, LOG_LEVEL_INFO);
    SetLogLevel(g_netlogger, LOG_LEVEL_ALL);

    INFO_LOG(g_logger, "============================");
    INFO_LOG(g_netlogger, "============================");
    DEBUG_LOG(g_logger, "log test, num:%d, string: %s", 12345, "string");
    DEBUG_LOG(g_netlogger, "log test, num:%d, string: %s", 12345, "string");
}
declare_test_case(test_logger);
