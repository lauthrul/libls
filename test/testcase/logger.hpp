#include "testcase.hpp"
using namespace lslib::logger;

void test_logger()
{
    InitLogger("log4cxx.xml");
    _loggerptr lplogger = GetLogger("applogger");
    _loggerptr lpnetlogger = GetLogger("netlogger");
    RegistGlobalLogger(lplogger);
    RegistNetLogger(lpnetlogger);
    SetLogLevel(lplogger, LOG_LEVEL_INFO);
    SetLogLevel(lpnetlogger, LOG_LEVEL_ALL);

    INFO_LOG(lplogger, "============================");
    INFO_LOG(lpnetlogger, "============================");
    DEBUG_LOG(lplogger, "log test, num:%d, string: %s", 12345, "string");
    DEBUG_LOG(lpnetlogger, "log test, num:%d, string: %s", 12345, "string");
}
declare_test_case(test_logger);
