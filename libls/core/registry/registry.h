#pragma once

//////////////////////////////////////////////////////////////////////////
namespace lslib
{
    // 注册表相关操作
    namespace registry
    {
        LSLIB_API lstring registry_get_str(HKEY hKeyRoot, _lpcstr lpSubKey, _lpcstr lpValueName);

        LSLIB_API DWORD registry_get_dword(HKEY hKeyRoot, _lpcstr lpSubKey, _lpcstr lpValueName);

        // set a value to registry, if lpSubKey not exist, then create it.
        LSLIB_API long registry_set_str(HKEY hKeyRoot, _lpcstr lpSubKey, _lpcstr lpValueName, _lpcstr lpValue);

        // set a value to registry, if lpSubKey not exist, then create it.
        LSLIB_API long registry_set_dword(HKEY hKeyRoot, _lpcstr lpSubKey, _lpcstr lpValueName, DWORD dwValue);

        LSLIB_API long registry_delete_value(HKEY hKeyRoot, _lpcstr lpSubKey, _lpcstr lpValueName);

        LSLIB_API long registry_delete_key(HKEY hKeyRoot, _lpcstr lpSubKey);

    } // registry

} // lslib
