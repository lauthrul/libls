#pragma once
#include <tlhelp32.h>

//////////////////////////////////////////////////////////////////////////
namespace lslib
{
    /// @brief �������
    namespace process
    {
        declare_stl_obj(PROCESSENTRY32);

        /// @brief ���ݽ���ID��ȡ������Ϣ
        /// @param[out] pe ������Ϣ���ο�@link https://docs.microsoft.com/en-us/previous-versions/aa911518(v=msdn.10)?redirectedfrom=MSDN @endlink
        /// @param[in] pid ����ID
        /// @return ���سɹ���ʧ��
        LSLIB_API bool get_process_info_by_id(__out__ PROCESSENTRY32& pe, DWORD pid);

        /// @brief ���ݽ������ֻ�ȡ������Ϣ
        /// @param[out] pe_lst ������Ϣ�б�
        /// @param[in] lpProcessName ��������
        /// @param[in] bCaseSensitive �Ƿ����ִ�Сд
        /// @return ���سɹ���ʧ��
        LSLIB_API bool get_process_info_by_name(__out__ PROCESSENTRY32_list& pe_lst, lpcstr lpProcessName, bool bCaseSensitive = false);

        /// @brief ���ݽ������ֻ�ȡ��һ�����������Ľ���ID
        /// @param lpProcessName ��������
        /// @param bCaseSensitive �Ƿ����ִ�Сд
        /// @return �ɹ����ؽ���ID��ʧ�ܷ���0
        LSLIB_API DWORD get_process_id_by_name(lpcstr lpProcessName, bool bCaseSensitive = false);

        /// @brief ���ݽ���ID��ȡ����·��
        /// @param pid ����ID
        /// @return �ɹ����ؽ���·����ʧ�ܷ��ؿ��ַ���
        LSLIB_API string get_process_path_by_id(DWORD pid);

        /// @brief ���ݽ������ֻ�ȡ��һ�����������Ľ���·��
        /// @param lpProcessName ��������
        /// @param bCaseSensitive �Ƿ����ִ�Сд
        /// @return �ɹ����ؽ���·����ʧ�ܷ��ؿ��ַ���
        LSLIB_API string get_process_path_by_name(lpcstr lpProcessName, bool bCaseSensitive = false);

        /// @brief ��������ID
        /// @param pid ����ID
        /// @return ���سɹ���ʧ��
        LSLIB_API bool kill_process_by_id(DWORD pid);

        /// @brief ����������
        /// @param lpProcessName ��������
        /// @param bCaseSensitive �Ƿ����ִ�Сд
        /// @return ���سɹ���ʧ��
        LSLIB_API bool kill_process_by_name(lpcstr lpProcessName, bool bCaseSensitive = false);

    } // process

} // lslib
