// Copyright (c) 2019 lauthrul(lauthrul@foxmail.com)
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or
// without modification, are permitted provided that the
// following conditions are met.
//
// Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following
// disclaimer in the documentation and/or other materials
// provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
// CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
// INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//////////////////////////////////////////////////////////////////////////

#ifdef LSLIB_STATIC
#   define LSLIB_API
#else
#   if defined(LSLIB_EXPORTS)
#       if  defined(_MSC_VER)
#           define LSLIB_API __declspec(dllexport)
#       else
#           define LSLIB_API
#       endif
#   else
#       if defined(_MSC_VER)
#           define LSLIB_API __declspec(dllimport)
#       else
#           define LSLIB_API
#       endif
#   endif
#endif

#define UILIB_COMDAT __declspec(selectany)

#if defined(_MSC_VER)
#   if defined _M_IX86
#       pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#   elif defined _M_IA64
#       pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#   elif defined _M_X64
#       pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#   else
#       pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#   endif
#endif

//////////////////////////////////////////////////////////////////////////
// system includes

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <malloc.h>
#include <iostream>
#include <cassert>
#include <string>
using namespace std;

#if defined(_MSC_VER)
#include <tchar.h>
#include <atlbase.h>
#include <atlstr.h>
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <richedit.h>
#include <assert.h>
#include <crtdbg.h>
#else
#include <stdint.h>
#include "unistd.h"
#endif


//////////////////////////////////////////////////////////////////////////

#ifndef __ref__
#define __ref__
#endif
#ifndef __in__
#define __in__
#endif
#ifndef __out__
#define __out__
#endif
#ifndef __inout__
#define __inout__
#endif

//////////////////////////////////////////////////////////////////////////
// custom includes

#include "core/type.h"
#include "core/mmr.h"
#include "core/string/strtool.h"
#include "core/crypto/crypto.h"
#include "core/math/math.h"
#include "core/lock/lock.h"
#include "core/time/timer.h"
#include "core/os/os.h"
#include "core/thread/thread.h"
#include "core/zip/zip_x.h"
#include "core/xml/xml.h"
#include "core/json/json_x.h"
#include "core/logger/logger.h"
#include "core/net/httpclient.h"
#include "core/thread/idle_thread.h"
#include "core/task/TaskDefine.h"
#include "core/task/TaskAgent.h"
#include "core/task/TaskManager.h"
#ifdef _MSC_VER
#include "core/registry/registry.h"
#include "core/process/process.h"
#endif
#include "utils/utils.h"
#include "utils/cmdline.h"
#include "utils/cfghandler.h"

//////////////////////////////////////////////////////////////////////////
// custom macro defines

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef CLAMP
#define CLAMP(x, a, b) (MIN(b, MAX(a, x)))
#endif

#ifndef FLOAT_EQUAL
#define FLOAT_EQUAL(a, b)		((a) - (b) >= -0.00001 && (a) - (b) <= 0.00001)
#endif
