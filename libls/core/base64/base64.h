#pragma once

namespace lslib
{

    namespace base64
    {
        LSLIB_API lstring encode(_lpcstr data, int len);
        LSLIB_API lstring decode(_lpcstr data, int len, __out__ int* outlen);
    };

} // lslib
