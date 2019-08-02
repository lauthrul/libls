// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "routine.h"

//////////////////////////////////////////////////////////////////////////
void CustomInit()
{
    // init logger
    InitLogger("log4cxx.xml");
    g_pLogger = GetLogger("applogger");
    g_pNetlogger = GetLogger("netlogger");
    RegistGlobalLogger(g_pLogger);
    RegistNetLogger(g_pNetlogger);
    INFO_LOG(g_pLogger, "================================");

    // setup http client env
    net::httpclient_thread_setup();

    // init db wrapper
#ifndef MAKE_TEST_DATA
    g_dbWrapper.SetHost(CCfgHandler::GetCfgText(CFG_MODULE_System, CFG_KEY_DB_HOST, CFG_DEFAULT_DB_HOST, g_strCfgFile.c_str()).c_str());
    g_dbWrapper.SetPort(CCfgHandler::GetCfgInt(CFG_MODULE_System, CFG_KEY_DB_PORT, CFG_DEFAULT_DB_PORT, g_strCfgFile.c_str()));
    g_dbWrapper.SetUser(CCfgHandler::GetCfgText(CFG_MODULE_System, CFG_KEY_DB_USER, CFG_DEFAULT_DB_USER, g_strCfgFile.c_str()).c_str());
    g_dbWrapper.SetPassword(CCfgHandler::GetCfgText(CFG_MODULE_System, CFG_KEY_DB_PWD, CFG_DEFAULT_DB_PWD, g_strCfgFile.c_str()).c_str());
    g_dbWrapper.SetDB(CCfgHandler::GetCfgText(CFG_MODULE_System, CFG_KEY_DB_DATABASE, CFG_DEFAULT_DB_DATABASE, g_strCfgFile.c_str()).c_str());
    g_dbWrapper.Connect();
#endif

    // mqtt client
    g_mqttClient.Connect(CCfgHandler::GetCfgText(CFG_MODULE_System, CFG_KEY_MQTT_HOST, CFG_DEFAULT_MQTT_HOST, g_strCfgFile.c_str()).c_str(),
                         CCfgHandler::GetCfgText(CFG_MODULE_System, CFG_KEY_MQTT_USER, CFG_DEFAULT_MQTT_USER, g_strCfgFile.c_str()).c_str(),
                         CCfgHandler::GetCfgText(CFG_MODULE_System, CFG_KEY_MQTT_PWD, CFG_DEFAULT_MQTT_PWD, g_strCfgFile.c_str()).c_str());
}

void CustomDestory()
{
    // clean up http client env
    net::httpclient_thread_cleanup();

    // destory logger
    DestroyLogger();
}

//////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    CustomInit();

    CRoutine routine;
    g_netManager.RegisterCallBack(&routine);
    g_mqttClient.SetNotifier(&routine);
    routine.Run();

    CustomDestory();
    return 0;
}
