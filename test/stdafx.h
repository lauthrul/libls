// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"


// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#ifdef __MINGW32__
#else
#define USE_LOG4CXX
#define USE_LIBCURL
#define USE_LIBICONV
#endif

#include <libls.h>