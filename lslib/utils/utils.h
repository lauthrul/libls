#pragma once

namespace lslib
{
    namespace utils
    {
		LSLIB_API _lbyte char_to_hex(_lchar c); // 'a' -> 0xa
		LSLIB_API _lchar hex_to_char(_lbyte b); // 0xa -> 'a'
		LSLIB_API _lbyte_array hex_str_to_hex(_lpcstr data);
		LSLIB_API lstring hex_to_hex_str(_lbyte_array data);

    } // utils

} // lslib

