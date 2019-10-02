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
#define regist_ls_type(type) \
    typedef type l##type; \
    typedef type* lp##type;

    regist_ls_type(void);                       // 1 bytes
    regist_ls_type(bool);                       // 1 bytes
    regist_ls_type(int);                        // 4 bytes
    regist_ls_type(float);                      // 4 bytes
    regist_ls_type(double);                     // 8 bytes

    typedef unsigned char           lbyte;      // 1 bytes
    typedef lbyte*                  lpbyte;

    typedef unsigned int            luint;      // 4 bytes
    typedef luint*                  lpuint;     // 4 bytes

    typedef unsigned short          lword;      // 2 bytes
    typedef lword*                  lpword;

    typedef unsigned long           ldword;     // windows: 4 bytes, linux: 8 bytes
    typedef ldword*                 lpdword;

    typedef unsigned char           luchar;     // 1 bytes
    typedef luchar*                 lpustr;
    typedef const luchar*           lpcustr;

    typedef char                    lchar;      // 1 bytes
    typedef lchar*                  lpstr;
    typedef const lchar*            lpcstr;

    typedef wchar_t                 lwchar;     // 4 bytes
    typedef lwchar*                 lpwstr;
    typedef const lwchar*           lpcwstr;

#define declare_stl_obj(type) \
    typedef vector<type>            type##_array; \
    typedef list<type>              type##_list; \
    typedef set<type>               type##_set;

#define declare_stl_map(key_type, type, name) \
    typedef map<key_type, type>     name;

    declare_stl_obj(lvoid);
    declare_stl_obj(lpvoid);

    declare_stl_obj(lbool);
    declare_stl_obj(lpbool);

    declare_stl_obj(lint);
    declare_stl_obj(lpint);

    declare_stl_obj(lfloat);
    declare_stl_obj(lpfloat);

    declare_stl_obj(ldouble);
    declare_stl_obj(lpdouble);

    declare_stl_obj(luint);
    declare_stl_obj(lpuint);

    declare_stl_obj(lbyte);
    declare_stl_obj(lpbyte);

    declare_stl_obj(lword);
    declare_stl_obj(lpword);

    declare_stl_obj(ldword);
    declare_stl_obj(lpdword);

    declare_stl_obj(luchar);
    declare_stl_obj(lpustr);
    declare_stl_obj(lpcustr);

    declare_stl_obj(lchar);
    declare_stl_obj(lpstr);
    declare_stl_obj(lpcstr);

    declare_stl_obj(lwchar);
    declare_stl_obj(lpwstr);
    declare_stl_obj(lpcwstr);

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
