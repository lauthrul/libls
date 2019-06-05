#include "testcase.hpp"
using namespace lslib::os;
using namespace lslib::net;
using namespace lslib::logger;

void test_httpclient()
{
    InitLogger("log4cxx.xml");
    _loggerptr lplogger = GetLogger("applogger");
    _loggerptr lpnetlogger = GetLogger("netlogger");
    RegistGlobalLogger(lplogger);
    RegistNetLogger(lpnetlogger);

    INFO_LOG(lplogger, "============================");
    INFO_LOG(lpnetlogger, "============================");

    //////////////////////////////////////////////////////////////////////////
    // crack url test
    lstring sz[] =
    {
        "www.baidu.com",
        "www.baidu.com:8080",
        "www.baidu.com/",
        "www.baidu.com:8080/",
        "www.baidu.com/path/subpath",
        "www.baidu.com/path/subpath/",
        "www.baidu.com:8080/path/subpath",
        "www.baidu.com:8080/path/subpath/",
        "www.baidu.com:8080/path/subpath/?ver=1.0.0",
        "www.baidu.com:8080/path/subpath/?ver=1.0.0&uid=xxx&user=test",
        "www.baidu.com:8080/path/subpath#tag",
        "www.baidu.com:8080/path/subpath/?ver=1.0.0#tag",
        "www.baidu.com:8080/path/subpath/?ver=1.0.0&uid=xxx&user=test#tag",
        "https://www.baidu.com",
        "https://www.baidu.com:8080",
        "https://www.baidu.com/",
        "https://www.baidu.com:8080/",
        "https://www.baidu.com/path/subpath",
        "https://www.baidu.com/path/subpath/",
        "https://www.baidu.com:8080/path/subpath",
        "https://www.baidu.com:8080/path/subpath/",
        "https://www.baidu.com:8080/path/subpath/?ver=1.0.0",
        "https://www.baidu.com:8080/path/subpath/?ver=1.0.0&uid=xxx&user=test",
        "https://www.baidu.com:8080/path/subpath#tag",
        "https://www.baidu.com:8080/path/subpath/?ver=1.0.0#tag",
        "https://www.baidu.com:8080/path/subpath/?ver=1.0.0&uid=xxx&user=test#tag",
    };

    SHttpUrl vUrl;
    lstring str;
    for (size_t i = 0; i < sizeof(sz) / sizeof(lstring); i++)
    {
        const lstring& url = sz[i];
        vUrl = CrackUrl(url);
        cout << "[" << url << "]" << endl;
        cout << "schema: " << vUrl.strScheme << endl;
        cout << "hostname: " << vUrl.strHostName << endl;
        cout << "port: " << vUrl.nPort << endl;
        cout << "path: " << vUrl.strPath << endl;
        cout << "query: " << endl;
        for (map<lstring, lstring>::iterator it = vUrl.mapQuerys.begin();
                it != vUrl.mapQuerys.end(); it++)
        {
            cout << "\t" << it->first << ": " << it->second << endl;
        }
        cout << "tag: " << vUrl.strTag << endl;
        cout << "[" << ReverseUrl(vUrl) << "]" << endl;
        cout << "------------------" << endl;
    }

    //////////////////////////////////////////////////////////////////////////
    // http get test
    lstring sz1[] =
    {
        "www.baidu.com",
        "https://www.baidu.com/favicon.ico",
    };
    SHttpGetParam v;
    SHttpResult r;
    for (size_t i = 0; i < sizeof(sz1) / sizeof(lstring); i++)
    {
        v.strUrl = sz1[i];
        r = CHttpClient::HttpGet(v);
        cout << "http get: " << v.strUrl << ", code: " << r.nCode  << ", size: " << r.nDataLen << endl;
    }

    //////////////////////////////////////////////////////////////////////////
    // http post test
    lstring sz2[] =
    {
        "https://passport.baidu.com/v2/api/?login",
        "staticpage=https%3A%2F%2Fwww.baidu.com%2Fcache%2Fuser%2Fhtml%2Fv3Jump.html&charset=UTF-8&token=b90392392c3b56432d18780a64a4647a&tpl=mn&subpro=&apiver=v3&tt=1556095956383&codestring=njGd606e25d9855de8b026d157ede010313bbd2a6067a010028&safeflg=0&u=https%3A%2F%2Fwww.baidu.com%2Fs%3Fie%3Dutf-8%26f%3D8%26rsv_bp%3D1%26ch%3D%26tn%3Dbaiduerr%26bar%3D%26wd%3Dtest&isPhone=&detect=1&gid=3225158-D700-43BE-BB38-0D5CD81F02A4&quick_user=0&logintype=dialogLogin&logLoginType=pc_loginDialog&idc=&loginmerge=true&splogin=rate&username=sharlon&password=ZgXkxirq1Pmo8MWS4cJg0SnZnU3yRif0OLNH7qrT1NPLDYyy5KsfrNyqUWbymxyWJrSXweTPDmqOWqH29lg3aTGeBHdI8r17ib2wr13grIEw584UaS1cBng4RjsbBs3p30G9MlSSa23C4Q9WC%2B8mdZtTNJsxBlzj%2FH9DWRaBkvQ%3D&verifycode=0fet&mem_pass=on&rsakey=rOHNpbi7Qv4dlcVeuhxzx7Z69sMYZa7S&crypttype=12&ppui_logintime=20115&countrycode=&fp_uid=&fp_info=&loginversion=v4&ds=paq2xwI3vm1Cj3Q79gm6J8fblHMMpMTDYNQavPJTaGeJYe0TjNXmGsYfLER6YQHZYDH22UG6o9arEe%2B9XsMDEVUJr8uzTg5t3iFHZrhmTSq1TKE0ZLmXOXQAVJ40AAUoihtDxvuHE2%2BKAv%2BGsBc2Cvm0wHHV11xmy0Putyf0lJZcwkfmDS8ppD8CJici7BZr552vz1uR7u%2BrzERvIg9DFHuVaXvkMC7rhRhwzlvtC6pI5Rb1LOlJyz%2Bt4g4UKZdGsEX47IQ9RwvsPY0NtvLwhtpEZNK7R0z%2FNvYfOZTR21j5uR2D6bddhRDjrof17wpZ1uaFMg68mJs0ZqaXK%2BBsUuFpdL4eUzOTiVB2l2PNJsQcNU8%2F4lSdD%2FjNrBIDwjdWKtgAMyaedvt7PuYpXTu9wuPaUw2IlXUKfahgEu9FmsWPbL1hIB%2Bx1%2FjjMJZZqAj7GT1K9UAfLMFe7B6qaST%2F9pmKO95Z2AXekPAxRk9uIEOOpRJMxP04tJPSEqPyo0IqkfCA9LGgZ1xbpgqnxRcwjOY3F%2BQsSk0RRtKE5f5WuJ%2B%2F6ArEdKw9wzxT0fxlg9GFJMYjJRgO6a%2B%2F0F0PX8v9h6jeSek4hkloX%2BkAb8dSATm5QLuZlniM%2BHmVjG62TDwavjx5N4PqaFSXU0PZT3tqe5YQ71CiRAzbbmQ%2FjtEHblUD6HXPiNwxWBDBuJW0oRMnVRPvafrhLdaXna1PeaQpQ%2BVp8Y%2BYqMvCGKDSeQkPwDDOeyLavL1S7Em0fFGOGo3QXL8847ecOQbCsBnfOEeB8pLkh8n1M%2B6VjUhDYpu%2B9jDGik8x62I1ZM7pSQRQA2VLljCqoKGeZ6YiwXomShSSty3RO%2BzyXzkfS31dI%2FaeIMER69ZNsW0ppl83n2CFxcmMwiDLwlyU3iGDScOOwbOSQETYSpCZDE5RYe07mBEj3fb3bgBHhuI3q0UHwt1J5hjGetDZEgVz8Hlc5JPesIbZfvw8FjIFShhtA8w6TYFvTR6yfCHuhgRLmZS6tGQGZmY0h1bPRjk8hYcJU%2FNvvoDW%2BAK%2Fjj7d5bMb9cDZ%2BLlO5KvIbyL0jlSgg%2BkyxT06%2FqyFbtvJN3umv4yyxDkrHY%2BUDGgV7isnmYk5BmHiaopCAcN%2BOXEPBlPIClhNhESwZDap&tk=6034nGv%2FJ0WDPiiowyCb%2BdnYpzvj8yAg4VmMkpc3bhbXchkn9%2BUeskkK3HioCrqyetYc&dv=tk0.66756056525117251556095936609%40wws0zI8kqfJs4C6x2hEJdOG4oFBzsWBxdxp%7EJSEb4YvDWyoBWfAmXGFGyrG34oBxdFGxhOAEhOBih0pioiPi238k3gOk3%7E8kqfJs4C6x2hEJdOG4oFBzsWBxd%7EvB2cvHUkP%7E6U8ksxoV9XOkpW8kqfJs4C6x2hEJdOG4oFBzsWBxd%7EvB2cvHUkP%7E6U8ksxCkRWOk9y8kqfJs4C6x2hEJdOG4oFBzsWBxd%7EvB2cvHUkP%7E6U8ks%7EAEpWOq__gs0VgAEAyAmWlCEFfAz3ioSXwAEAXADWxokGfokFlAmXwAE5io1WSCE9fokR%7EAmXGFGyrG34oBxdFGxhOAEhOBi4S9%7Ed3vGojPH6UPH6wAEsyomWxoEFfoEFlASXwAEslCmW%7EAEqfoVASASXGFGyrG34oBxdFGxhOAEhOB%7EvjPi6UpUJAP%7ENcP32lPHW_IkkMEk%7EqnmWph%7EshADWS8kpzBspNtfW8V9%7EozG%7EAkG%7EoE5xAEsiAVGXoEG%7EAk3xCEA%7EoVqyhsIMmXQCsvAmWW8k5xokAfAEpyASWSoE9W8kslokqfAVG%7EAmWSoE9W8kslAkq_&vcodefrom=checkuname&traceid=ABF1B601&callback=parent.bd__pcbs__enyuz6",
    };
    SHttpPostParam vp;
    for (size_t i = 0; i < sizeof(sz2) / sizeof(lstring) / 2; i++)
    {
        vp.strUrl = sz2[i];
        vp.strPost = sz2[i + 1];
        r = CHttpClient::HttpPost(vp);
        cout << "http post: " << vp.strUrl << ", code: " << r.nCode  << ", size: " << r.nDataLen << endl;
    }

    //////////////////////////////////////////////////////////////////////////
    // http download test
    mkdir("download_test");
    lstring sz3[] =
    {
        "https://www.baidu.com/favicon.ico",
        "https://www.google.com/images/branding/googlelogo/2x/googlelogo_color_272x92dp.png",
    };
    SHttpDowloadParam vd;
    for (size_t i = 0; i < sizeof(sz3) / sizeof(lstring); i++)
    {
        vd.strUrl = sz3[i];
        vd.strFile = path_combine("download_test", path_get_name(vd.strUrl));
        r = CHttpClient::DownloadFile(vd);
        cout << "http download: " << vd.strUrl << ", code: " << r.nCode  << ", size: " << r.nDataLen << endl;
    }
}
declare_test_case(test_httpclient);
