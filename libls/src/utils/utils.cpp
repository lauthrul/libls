// lslib.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "utils.h"

#ifdef _MSC_VER
#else
#include <uuid/uuid.h>
#endif

namespace lslib
{
    namespace utils
    {
        string gen_uuid()
        {
            string str;

#ifdef _MSC_VER
            UUID guid;
            lpustr buf;

            UuidCreate(&guid);
            UuidToString(&guid, &buf);

            str = (lpcstr)buf;
            str = strtool::replace(str, "-", "");
            str = strtool::lower(str);

            RpcStringFree(&buf);
#else
            uuid_t uu;
            uuid_generate(uu);
            return strtool::byte_array_to_hex_str(uu, 16);
#endif // _MSC_VER

            return str;
        }

    } // utils

} // lslib
