#pragma once

//////////////////////////////////////////////////////////////////////////
namespace lslib
{
    /// @brief 注册表相关
    namespace registry
    {
        /// @brief 获取注册表STRING键值
        /// @param hKeyRoot 主根键[HKEY_CURRENT_USER | HKEY_LOCAL_MACHINE | HKEY_CLASSES_ROOT | HKEY_USERS | HKEY_CURRENT_CONFIG]
        /// @param lpSubKey 键值路径
        /// @param lpValueName 键值名称
        /// @return 返回获取到的键值，失败返回空字符串
        LSLIB_API string registry_get_str(HKEY hKeyRoot, _lpcstr lpSubKey, _lpcstr lpValueName);

        /// @brief 获取字符串DWORD键值
        /// @param hKeyRoot 主根键[HKEY_CURRENT_USER | HKEY_LOCAL_MACHINE | HKEY_CLASSES_ROOT | HKEY_USERS | HKEY_CURRENT_CONFIG]
        /// @param lpSubKey 键值路径
        /// @param lpValueName 键值名称
        /// @return 返回获取到的键值，失败返回0
        LSLIB_API DWORD registry_get_dword(HKEY hKeyRoot, _lpcstr lpSubKey, _lpcstr lpValueName);

        /// @brief 设置注册表STRING键值
        /// @param hKeyRoot 主根键[HKEY_CURRENT_USER | HKEY_LOCAL_MACHINE | HKEY_CLASSES_ROOT | HKEY_USERS | HKEY_CURRENT_CONFIG]
        /// @param lpSubKey 键值路径
        /// @param lpValueName 键值名称
        /// @param lpValue 键值
        /// @param bCreate 若键值路径不存在是否创建
        /// @return 成功返回0，失败返回错误码
        LSLIB_API long registry_set_str(HKEY hKeyRoot, _lpcstr lpSubKey, _lpcstr lpValueName, _lpcstr lpValue, bool bCreate = true);

        /// @brief 设置注册表DWORD键值
        /// @param hKeyRoot 主根键[HKEY_CURRENT_USER | HKEY_LOCAL_MACHINE | HKEY_CLASSES_ROOT | HKEY_USERS | HKEY_CURRENT_CONFIG]
        /// @param lpSubKey 键值路径
        /// @param lpValueName 键值名称
        /// @param dwValue 键值
        /// @param bCreate 若键值路径不存在是否创建
        /// @return 成功返回0，失败返回错误码
        LSLIB_API long registry_set_dword(HKEY hKeyRoot, _lpcstr lpSubKey, _lpcstr lpValueName, DWORD dwValue, bool bCreate = true);

        /// @brief 删除键值
        /// @param hKeyRoot 主根键[HKEY_CURRENT_USER | HKEY_LOCAL_MACHINE | HKEY_CLASSES_ROOT | HKEY_USERS | HKEY_CURRENT_CONFIG]
        /// @param lpSubKey 键值路径
        /// @param lpValueName 键值名称
        /// @return 成功返回0，失败返回错误码
        LSLIB_API long registry_delete_value(HKEY hKeyRoot, _lpcstr lpSubKey, _lpcstr lpValueName);

        /// @brief 删除键值路径
        /// @param hKeyRoot 主根键[HKEY_CURRENT_USER | HKEY_LOCAL_MACHINE | HKEY_CLASSES_ROOT | HKEY_USERS | HKEY_CURRENT_CONFIG]
        /// @param lpSubKey 键值路径
        /// @return 成功返回0，失败返回错误码
        LSLIB_API long registry_delete_key(HKEY hKeyRoot, _lpcstr lpSubKey);

    } // registry

} // lslib
