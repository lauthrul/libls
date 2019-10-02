#pragma once
#include "stdafx.h"
#include "mmr.h"

namespace lslib
{
    LSLIB_API lpbyte lsalloc(size_t len)
    {
        lpbyte buf = (lpbyte)malloc(len);
        if (buf != NULL) memset(buf, 0, len);
        return buf;
    }

    LSLIB_API void lsfree(lpbyte data)
    {
        free(data);
    }

}
