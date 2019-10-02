#pragma once
#include "type.h"

namespace lslib
{
    /// @brief �����ڴ�ռ�
    /// @param len ��Ҫ������ڴ�Ŀռ��С
    /// @return �ɹ��������뵽���ڴ�ָ�룬ʧ�ܷ���NULL
    LSLIB_API lpbyte lsalloc(size_t len);

    /// @brief �ͷ��ڴ�ռ�
    /// @param data ��Ҫ�ͷŵ��ڴ�ָ��
    LSLIB_API void lsfree(lpbyte data);
}
