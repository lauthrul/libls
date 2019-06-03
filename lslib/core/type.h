#pragma once
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
using namespace std;

//////////////////////////////////////////////////////////////////////////
namespace lslib
{
    typedef byte*                   _lpbyte;
    typedef unsigned short          _lword;
    typedef unsigned long           _ldword;
    typedef _lword*                 _lpword;
    typedef _ldword*                _lpdword;
    typedef unsigned char           _luchar;
    typedef unsigned char*          _lpustr;
    typedef const unsigned char*    _lpcustr;
    typedef char                    _lchar;
    typedef char*                   _lpstr;
    typedef const char*             _lpcstr;
    typedef wchar_t                 _lwchar;
    typedef wchar_t*                _lpwstr;
    typedef const wchar_t*          _lpcwstr;

#define declare_stl_obj(type) \
    typedef vector<type>    type##_array; \
    typedef list<type>      type##_list; \
    typedef set<type>       type##_set;

#define declare_stl_pointer(type) \
    typedef vector<type*>   p##type##_array; \
    typedef list<type*>     p##type##_list; \
    typedef set<type*>      p##type##_set;
}