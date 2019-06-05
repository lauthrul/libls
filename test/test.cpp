// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
#include "testcase/testcase.hpp"
#include "testcase/logger.hpp"
#include "testcase/httpclient.hpp"
#include "testcase/string.hpp"
#include "testcase/type.hpp"
#include "testcase/os.hpp"
#include "testcase/registry.hpp"
#include "testcase/process.hpp"
#include "testcase/algorithm.hpp"
#include "testcase/math.hpp"

extern fn_test_list test_cases;
int _tmain(int argc, _TCHAR* argv[])
{
    for (fn_test_list::iterator it = test_cases.begin(); it != test_cases.end(); it++)
    {
        fn_test fn = (*it);
        (fn != NULL) ? (*fn)() : 1;
    }
    getchar();
    return 0;
}
