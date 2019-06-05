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
    typedef vector<type>            type##_array; \
    typedef list<type>              type##_list; \
    typedef set<type>               type##_set;

#define declare_stl_pointer(type) \
    typedef vector<type*>           p##type##_array; \
    typedef list<type*>             p##type##_list; \
    typedef set<type*>              p##type##_set;


//////////////////////////////////////////////////////////////////////////
// enum related functions

// declare in .h file
#define declare_enum_str(enum_type, export) \
    export string get_##enum_type##_str(enum_type enum_value); \
    export enum_type get_##enum_type##_from_str(_lpcstr str); \

// place in .cpp file
#define enum_str_begin(enum_type) \
    map<enum_type, _lpcstr> init_##enum_type##_str_table() \
    { \
        map<enum_type, _lpcstr> _table;

#define enum_str_item(enum_value) \
        _table[enum_value] = #enum_value;

#define enum_str_end(enum_type) \
        return _table; \
    } \
    map<enum_type, _lpcstr> enum_type##_str_table = init_##enum_type##_str_table(); \
    string get_##enum_type##_str(enum_type enum_value) \
    { \
        return (enum_type##_str_table)[enum_value]; \
    } \
    enum_type get_##enum_type##_from_str(_lpcstr str) \
    {\
        for (map<enum_type, _lpcstr>::iterator it = (enum_type##_str_table).begin(); \
                it != (enum_type##_str_table).end(); it++) \
        { \
            if (strcmp(it->second, str) == 0) return it->first; \
        } \
        return (enum_type##_str_table).begin()->first; \
    }

// customer calls
#define enum_str(enum_type, enum_value) \
    get_##enum_type##_str(enum_value)

#define enum_from_str(enum_type, str) \
    get_##enum_type##_from_str(str)
}