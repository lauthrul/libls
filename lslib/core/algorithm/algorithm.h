#pragma once

namespace lslib
{
    namespace md5
    {
        LSLIB_API lstring md5(_lpcstr str, int len);
        LSLIB_API lstring file_md5(_lpcstr pfile);
    };

    namespace sha1
    {
        LSLIB_API lstring sha1(_lpcstr str, int len);
        LSLIB_API lstring file_sha1(_lpcstr pfile);
    }

    namespace base64
    {
        LSLIB_API lstring encode(_lpcstr data, int len, int line_len = -1); // line_len: 64 or 76 or -1 (single line)
        LSLIB_API lstring decode(_lpcstr data, int len, __out int* outlen);
    };
}