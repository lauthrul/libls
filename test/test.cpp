// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "lslib.h"
using namespace lslib;
using namespace lslib::os;
#include <iomanip>

int _tmain(int argc, _TCHAR* argv[])
{
	is_exist(CString("a") + "b");
	is_exist(lstring("a") + 'b');
	is_exist(lstring("a") + "b");

// #include "testcase/type.hpp"
// #include "testcase/os.hpp"
// #include "testcase/math.hpp"
#include "testcase/logger.hpp"

	getchar();
	return 0;
}

