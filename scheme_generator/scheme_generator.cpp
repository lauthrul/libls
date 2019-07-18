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
    g_dbWrapper.SetHost("192.168.169.215");
    g_dbWrapper.SetUser("suser");
    g_dbWrapper.SetPassword("cyberuser");
    g_dbWrapper.SetDB("autoservice_debug");
    g_dbWrapper.Connect();
#endif
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
    routine.Run();

    CustomDestory();
    return 0;
}
