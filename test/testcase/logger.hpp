using namespace lslib::logger;


InitLogger("log4cxx.xml");
RegistGlobalLogger(GetLogger("applogger"));
RegistNetLogger(GetLogger("netlogger"));

DEBUG_LOG(g_logger, "log test, num:%d, string: %s", 12345, "string");
