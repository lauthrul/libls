#pragma once
#include "type.h"

namespace lslib
{
    LSLIB_API _lpbyte lsalloc(size_t len);
    LSLIB_API void lsfree(_lpbyte data);
}
