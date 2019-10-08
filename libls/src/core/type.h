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
    /**
        size of each types under different complier

        [type]     [windows_32]      [linux_64]
        -----------------------------------------
        lint8:          1                 1
        lint16:         2                 2
        lint32:         4                 4
        lint64:         8                 8
        luint8:         1                 1
        luint16:        2                 2
        luint32:        4                 4
        luint64:        8                 8
        lbool:          1                 1
        lint:           4                 4
        lfloat:         4                 4
        ldouble:        8                 8
        luint:          4                 4
        lbyte:          1                 1
        lword:          2                 2
        ldword:         4                 8
        luchar:         1                 1
        lchar:          1                 1
        lwchar:         2                 4
        lpint8:         4                 8
        lpint16:        4                 8
        lpint32:        4                 8
        lpint64:        4                 8
        lpuint8:        4                 8
        lpuint16:       4                 8
        lpuint32:       4                 8
        lpuint64:       4                 8
        lpvoid:         4                 8
        lpbool:         4                 8
        lpint:          4                 8
        lpfloat:        4                 8
        lpdouble:       4                 8
        lpuint:         4                 8
        lpbyte:         4                 8
        lpword:         4                 8
        lpdword:        4                 8
        lpustr:         4                 8
        lpcustr:        4                 8
        lpstr:          4                 8
        lpcstr:         4                 8
        lpwstr:         4                 8
        lpcwstr:        4                 8
    **/
    typedef signed char             lint8;
    typedef lint8*                  lpint8;

    typedef short                   lint16;
    typedef lint16*                 lpint16;

    typedef int                     lint32;
    typedef lint32*                 lpint32;

    typedef long long               lint64;
    typedef lint64*                 lpint64;

    typedef unsigned char           luint8;
    typedef luint8*                 lpuint8;

    typedef unsigned short          luint16;
    typedef luint16*                lpuint16;

    typedef unsigned int            luint32;
    typedef luint32*                lpuint32;

    typedef unsigned long long      luint64;
    typedef luint64*                lpuint64;

#define define_ls_type(type) \
    typedef type l##type; \
    typedef type* lp##type;

    define_ls_type(void);                       // 1 bytes
    define_ls_type(bool);                       // 1 bytes
    define_ls_type(int);                        // 4 bytes
    define_ls_type(float);                      // 4 bytes
    define_ls_type(double);                     // 8 bytes

    typedef unsigned char           lbyte;      // 1 bytes
    typedef lbyte*                  lpbyte;

    typedef unsigned int            luint;      // 4 bytes
    typedef luint*                  lpuint;     // 4 bytes

    typedef unsigned short          lword;      // 2 bytes
    typedef lword*                  lpword;

    typedef unsigned long           ldword;     // 32bit: 4 bytes, 64bit: 8 bytes
    typedef ldword*                 lpdword;

    typedef unsigned char           luchar;     // 1 bytes
    typedef luchar*                 lpustr;
    typedef const luchar*           lpcustr;

    typedef char                    lchar;      // 1 bytes
    typedef lchar*                  lpstr;
    typedef const lchar*            lpcstr;

    typedef wchar_t                 lwchar;     // 32bit: 2bytes, 64bit: 4 bytes
    typedef lwchar*                 lpwstr;
    typedef const lwchar*           lpcwstr;

#define define_stl_type(type) \
    typedef vector<type>            type##_array; \
    typedef list<type>              type##_list; \
    typedef set<type>               type##_set;

#define declare_stl_map(key_type, type, name) \
    typedef map<key_type, type>     name;

    define_stl_type(lint8);
    define_stl_type(lpint8);

    define_stl_type(lint16);
    define_stl_type(lpint16);

    define_stl_type(lint32);
    define_stl_type(lpint32);

    define_stl_type(lint64);
    define_stl_type(lpint64);

    define_stl_type(luint8);
    define_stl_type(lpuint8);

    define_stl_type(luint16);
    define_stl_type(lpuint16);

    define_stl_type(luint32);
    define_stl_type(lpuint32);

    define_stl_type(luint64);
    define_stl_type(lpuint64);

    define_stl_type(lvoid);
    define_stl_type(lpvoid);

    define_stl_type(lbool);
    define_stl_type(lpbool);

    define_stl_type(lint);
    define_stl_type(lpint);

    define_stl_type(lfloat);
    define_stl_type(lpfloat);

    define_stl_type(ldouble);
    define_stl_type(lpdouble);

    define_stl_type(luint);
    define_stl_type(lpuint);

    define_stl_type(lbyte);
    define_stl_type(lpbyte);

    define_stl_type(lword);
    define_stl_type(lpword);

    define_stl_type(ldword);
    define_stl_type(lpdword);

    define_stl_type(luchar);
    define_stl_type(lpustr);
    define_stl_type(lpcustr);

    define_stl_type(lchar);
    define_stl_type(lpstr);
    define_stl_type(lpcstr);

    define_stl_type(lwchar);
    define_stl_type(lpwstr);
    define_stl_type(lpcwstr);

//////////////////////////////////////////////////////////////////////////
// enum related functions

// declare in .h file
#define declare_enum_str(enum_type, export) \
    export string get_##enum_type##_str(enum_type enum_value); \
    export enum_type get_##enum_type##_from_str(lpcstr str); \

// place in .cpp file
#define enum_str_begin(enum_type) \
    map<enum_type, lpcstr> init_##enum_type##_str_table() \
    { \
        map<enum_type, lpcstr> _table;

#define enum_str_item(enum_value) \
        _table[enum_value] = #enum_value;

#define enum_str_item_ex(enum_value, str) \
        _table[enum_value] = str;

#define enum_str_end(enum_type) \
        return _table; \
    } \
    map<enum_type, lpcstr> enum_type##_str_table = init_##enum_type##_str_table(); \
    string get_##enum_type##_str(enum_type enum_value) \
    { \
        return (enum_type##_str_table)[enum_value]; \
    } \
    enum_type get_##enum_type##_from_str(lpcstr str) \
    {\
        for (map<enum_type, lpcstr>::iterator it = (enum_type##_str_table).begin(); \
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
