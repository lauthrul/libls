// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#ifdef _MSC_VER

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的


#pragma warning(disable:4251)
#pragma warning(disable:4805)
#pragma warning(disable:4800)
#pragma warning(disable:4996)

#endif

#include "libls.h"
