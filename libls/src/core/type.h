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

#define define_list(type, name)                 typedef list<type> name;
#define define_vector(type, name)               typedef vector<type> name;
#define define_set(type, name)                  typedef set<type> name;
#define define_map(key_type, type, name)        typedef map<key_type, type> name;

#define quick_define_list(type)                 define_list(type, type##_list)
#define quick_define_array(type)                define_vector(type, type##_array)
#define quick_define_set(type)                  define_set(type, type##_set)
#define quick_define_map(key_type, type)        define_map(key_type, type, key_type##_##type##_map)

#define quick_define_stl(type) \
    quick_define_list(type) \
    quick_define_array(type) \
    quick_define_set(type)

    quick_define_stl(lint8);
    quick_define_stl(lpint8);

    quick_define_stl(lint16);
    quick_define_stl(lpint16);

    quick_define_stl(lint32);
    quick_define_stl(lpint32);

    quick_define_stl(lint64);
    quick_define_stl(lpint64);

    quick_define_stl(luint8);
    quick_define_stl(lpuint8);

    quick_define_stl(luint16);
    quick_define_stl(lpuint16);

    quick_define_stl(luint32);
    quick_define_stl(lpuint32);

    quick_define_stl(luint64);
    quick_define_stl(lpuint64);

    quick_define_stl(lvoid);
    quick_define_stl(lpvoid);

    quick_define_stl(lbool);
    quick_define_stl(lpbool);

    quick_define_stl(lint);
    quick_define_stl(lpint);

    quick_define_stl(lfloat);
    quick_define_stl(lpfloat);

    quick_define_stl(ldouble);
    quick_define_stl(lpdouble);

    quick_define_stl(luint);
    quick_define_stl(lpuint);

    quick_define_stl(lbyte);
    quick_define_stl(lpbyte);

    quick_define_stl(lword);
    quick_define_stl(lpword);

    quick_define_stl(ldword);
    quick_define_stl(lpdword);

    quick_define_stl(luchar);
    quick_define_stl(lpustr);
    quick_define_stl(lpcustr);

    quick_define_stl(lchar);
    quick_define_stl(lpstr);
    quick_define_stl(lpcstr);

    quick_define_stl(lwchar);
    quick_define_stl(lpwstr);
    quick_define_stl(lpcwstr);

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
