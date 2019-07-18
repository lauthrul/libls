#pragma once
#include <algorithm>
#include <functional>

//////////////////////////////////////////////////////////////////////////
namespace lslib
{
    declare_stl_obj(string);

//     operator _lpcstr(const string& str)
//     {
//         return str.c_str();
//     }

    namespace strtool
    {
        LSLIB_API const bool is_empty(_lpcstr str);
        LSLIB_API const bool is_space(_lpcstr str);
        LSLIB_API const bool is_lower(_lpcstr str);
        LSLIB_API const bool is_upper(_lpcstr str);
        LSLIB_API const bool is_digital(_lpcstr str);
        LSLIB_API const bool is_int(_lpcstr str);
        LSLIB_API const bool is_float(_lpcstr str);
        LSLIB_API const bool is_bool(_lpcstr str, bool numeric = false);
        LSLIB_API const int to_int(_lpcstr str);
        LSLIB_API const double to_float(_lpcstr str);
        LSLIB_API const bool to_bool(_lpcstr str);
        LSLIB_API string from_int(int value);
        LSLIB_API string from_float(double value, int bit);
        LSLIB_API string from_bool(bool value);
        LSLIB_API string lower(_lpcstr str);
        LSLIB_API string upper(_lpcstr str);
        LSLIB_API string trim(_lpcstr str);
        LSLIB_API string trim_left(_lpcstr str);
        LSLIB_API string trim_right(_lpcstr str);
        LSLIB_API string trim(_lpcstr str, _lchar c);
        LSLIB_API string trim_left(_lpcstr str, _lchar c);
        LSLIB_API string trim_right(_lpcstr str, _lchar c);
        LSLIB_API string replace(_lpcstr str, _lchar needle, _lchar replacement, size_t index = 0, int counts = -1);
        LSLIB_API string replace(_lpcstr str, _lpcstr needle, _lpcstr replacement, size_t index = 0, int counts = -1);
        LSLIB_API string format(_lpcstr pfmt, ...);
        LSLIB_API void append_format(__out__ string& str, _lpcstr pfmt, ...);
        LSLIB_API const int split(__out__ string_array& dest, _lpcstr src, _lpcstr patten, bool allow_empty = true);

        LSLIB_API _lbyte char_to_hex(_lchar c); // 'a' -> 0xa
        LSLIB_API _lchar hex_to_char(_lbyte b); // 0xa -> 'a'
        LSLIB_API _lbyte_array hex_str_to_byte_array(_lpcstr data); // "da3fed134a" -> { 0xda, 0x3f, 0xed, 0x13, 0x4a }
        LSLIB_API string byte_array_to_hex_str(_lbyte_array data); //  { 0xda, 0x3f, 0xed, 0x13, 0x4a } -> "da3fed134a"
    }
}
