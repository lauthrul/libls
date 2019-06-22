// lslib.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "utils.h"

namespace lslib
{
    namespace utils
    {
        LSLIB_API _lbyte char_to_hex(_lchar c)
        {
            if (c >= '0' && c <= '9') return c - '0';
            else if (c >= 'a' && c <= 'f') return 10 + c - 'a';
            else if (c >= 'A' && c <= 'F') return 10 + c - 'A';
            else return 0xff;
        }

        LSLIB_API _lchar hex_to_char(_lbyte b)
        {
            if (b >= 0x0 && b <= 0x9) return b + '0';
            else if (b >= 0xa && b <= 0xf) return b - 10 + 'a';
            else return 0;
        }

        LSLIB_API _lbyte_array hex_str_to_byte_array(_lpcstr data)
        {
            assert(!is_empty(data));

            _lbyte_array arr;
            _lpcstr p = data;
            while (*p != 0)
            {
                _lbyte high = char_to_hex(*p++);
                _lbyte low = char_to_hex(*p++);
                if (high != 0xff && low != 0xff) arr.push_back((high << 4) | low);
            }
            return arr;
        }

        LSLIB_API lstring byte_array_to_hex_str(_lbyte_array data)
        {
            lstring str;
            for (size_t i = 0; i < data.size(); i++)
            {
                str += hex_to_char((data[i] & 0xf0) >> 4);
                str += hex_to_char(data[i] & 0x0f);
            }
            return str;
        }

    } // utils

} // lslib
