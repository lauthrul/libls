// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#pragma warning(disable:4251)
#pragma warning(disable:4805)
#pragma warning(disable:4800)
#pragma warning(disable:4996)

#ifdef __MINGW32__
#else
#define USE_LOG4CXX
#define USE_LIBCURL
#define USE_LIBICONV
#endif

#include <libls.h>
using namespace lslib;
