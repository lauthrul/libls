// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#ifdef _MSC_VER

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�:
#include <windows.h>


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��


#pragma warning(disable:4251)
#pragma warning(disable:4805)
#pragma warning(disable:4800)
#pragma warning(disable:4996)

#endif

#ifdef __MINGW32__
#else
#define USE_LOG4CXX
#define USE_LIBCURL
#define USE_LIBICONV
#endif

#include "libls.h"
