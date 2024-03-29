// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

////////////////////////////////////////////////////////////////////////
#include "testcase/testcase.hpp"
#ifdef _MSC_VER
#include "testcase/registry.hpp"
#include "testcase/process.hpp"
#endif
//#include "testcase/math.hpp"
#include "testcase/zip.hpp"
#include "testcase/mmr.hpp"
#include "testcase/logger.hpp"
#include "testcase/cfghandler.hpp"
#include "testcase/json.hpp"
#include "testcase/utils.hpp"
#include "testcase/time.hpp"
#include "testcase/type.hpp"
#include "testcase/crypto.hpp"
#include "testcase/string.hpp"
#include "testcase/os.hpp"
#include "testcase/httpclient.hpp"

extern fn_test_list test_cases;
int main(int argc, char* argv[])
{
    for (fn_test_list::iterator it = test_cases.begin(); it != test_cases.end(); it++)
    {
        fn_test fn = (*it);
        if (fn != NULL) (*fn)();
    }
    getchar();
    return 0;
}
