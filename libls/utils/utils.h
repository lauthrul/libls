#pragma once

namespace lslib
{
    namespace utils
    {
        LSLIB_API _lbyte char_to_hex(_lchar c); // 'a' -> 0xa
        LSLIB_API _lchar hex_to_char(_lbyte b); // 0xa -> 'a'
        LSLIB_API _lbyte_array hex_str_to_byte_array(_lpcstr data); // "da3fed134a" -> { 0xda, 0x3f, 0xed, 0x13, 0x4a }
        LSLIB_API lstring byte_array_to_hex_str(_lbyte_array data); //  { 0xda, 0x3f, 0xed, 0x13, 0x4a } -> "da3fed134a"
        LSLIB_API lstring int_to_str(int value);
        LSLIB_API lstring double_to_str(double value, int bit);
    } // utils

} // lslib

