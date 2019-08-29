#pragma once
#include "stdafx.h"
#include "mmr.h"

namespace lslib
{
    LSLIB_API _lpbyte lsalloc(size_t len)
    {
        _lpbyte buf = (_lpbyte)malloc(len);
        memset(buf, 0, len);
        return buf;
    }

    LSLIB_API void lsfree(_lpbyte data)
    {
        free(data);
    }

}
