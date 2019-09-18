#pragma once

//////////////////////////////////////////////////////////////////////////
namespace lslib
{
    /// @brief ע������
    namespace registry
    {
        /// @brief ��ȡע���STRING��ֵ
        /// @param hKeyRoot ������[HKEY_CURRENT_USER | HKEY_LOCAL_MACHINE | HKEY_CLASSES_ROOT | HKEY_USERS | HKEY_CURRENT_CONFIG]
        /// @param lpSubKey ��ֵ·��
        /// @param lpValueName ��ֵ����
        /// @return ���ػ�ȡ���ļ�ֵ��ʧ�ܷ��ؿ��ַ���
        LSLIB_API string registry_get_str(HKEY hKeyRoot, _lpcstr lpSubKey, _lpcstr lpValueName);

        /// @brief ��ȡ�ַ���DWORD��ֵ
        /// @param hKeyRoot ������[HKEY_CURRENT_USER | HKEY_LOCAL_MACHINE | HKEY_CLASSES_ROOT | HKEY_USERS | HKEY_CURRENT_CONFIG]
        /// @param lpSubKey ��ֵ·��
        /// @param lpValueName ��ֵ����
        /// @return ���ػ�ȡ���ļ�ֵ��ʧ�ܷ���0
        LSLIB_API DWORD registry_get_dword(HKEY hKeyRoot, _lpcstr lpSubKey, _lpcstr lpValueName);

        /// @brief ����ע���STRING��ֵ
        /// @param hKeyRoot ������[HKEY_CURRENT_USER | HKEY_LOCAL_MACHINE | HKEY_CLASSES_ROOT | HKEY_USERS | HKEY_CURRENT_CONFIG]
        /// @param lpSubKey ��ֵ·��
        /// @param lpValueName ��ֵ����
        /// @param lpValue ��ֵ
        /// @param bCreate ����ֵ·���������Ƿ񴴽�
        /// @return �ɹ�����0��ʧ�ܷ��ش�����
        LSLIB_API long registry_set_str(HKEY hKeyRoot, _lpcstr lpSubKey, _lpcstr lpValueName, _lpcstr lpValue, bool bCreate = true);

        /// @brief ����ע���DWORD��ֵ
        /// @param hKeyRoot ������[HKEY_CURRENT_USER | HKEY_LOCAL_MACHINE | HKEY_CLASSES_ROOT | HKEY_USERS | HKEY_CURRENT_CONFIG]
        /// @param lpSubKey ��ֵ·��
        /// @param lpValueName ��ֵ����
        /// @param dwValue ��ֵ
        /// @param bCreate ����ֵ·���������Ƿ񴴽�
        /// @return �ɹ�����0��ʧ�ܷ��ش�����
        LSLIB_API long registry_set_dword(HKEY hKeyRoot, _lpcstr lpSubKey, _lpcstr lpValueName, DWORD dwValue, bool bCreate = true);

        /// @brief ɾ����ֵ
        /// @param hKeyRoot ������[HKEY_CURRENT_USER | HKEY_LOCAL_MACHINE | HKEY_CLASSES_ROOT | HKEY_USERS | HKEY_CURRENT_CONFIG]
        /// @param lpSubKey ��ֵ·��
        /// @param lpValueName ��ֵ����
        /// @return �ɹ�����0��ʧ�ܷ��ش�����
        LSLIB_API long registry_delete_value(HKEY hKeyRoot, _lpcstr lpSubKey, _lpcstr lpValueName);

        /// @brief ɾ����ֵ·��
        /// @param hKeyRoot ������[HKEY_CURRENT_USER | HKEY_LOCAL_MACHINE | HKEY_CLASSES_ROOT | HKEY_USERS | HKEY_CURRENT_CONFIG]
        /// @param lpSubKey ��ֵ·��
        /// @return �ɹ�����0��ʧ�ܷ��ش�����
        LSLIB_API long registry_delete_key(HKEY hKeyRoot, _lpcstr lpSubKey);

    } // registry

} // lslib
