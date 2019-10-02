#pragma once
#include "type.h"

namespace lslib
{
    /// @brief 分配内存空间
    /// @param len 需要分配的内存的空间大小
    /// @return 成功返回申请到的内存指针，失败返回NULL
    LSLIB_API lpbyte lsalloc(size_t len);

    /// @brief 释放内存空间
    /// @param data 需要释放的内存指针
    LSLIB_API void lsfree(lpbyte data);
}
