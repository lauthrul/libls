#pragma once
#include <tlhelp32.h>

//////////////////////////////////////////////////////////////////////////
namespace lslib
{
    /// @brief 进程相关
    namespace process
    {
        declare_stl_obj(PROCESSENTRY32);

        /// @brief 根据进程ID获取进程信息
        /// @param[out] pe 进程信息，参考@link https://docs.microsoft.com/en-us/previous-versions/aa911518(v=msdn.10)?redirectedfrom=MSDN @endlink
        /// @param[in] pid 进程ID
        /// @return 返回成功或失败
        LSLIB_API bool get_process_info_by_id(__out__ PROCESSENTRY32& pe, DWORD pid);

        /// @brief 根据进程名字获取进程信息
        /// @param[out] pe_lst 进程信息列表
        /// @param[in] lpProcessName 进程名称
        /// @param[in] bCaseSensitive 是否区分大小写
        /// @return 返回成功或失败
        LSLIB_API bool get_process_info_by_name(__out__ PROCESSENTRY32_list& pe_lst, lpcstr lpProcessName, bool bCaseSensitive = false);

        /// @brief 根据进程名字获取第一个满足条件的进程ID
        /// @param lpProcessName 进程名称
        /// @param bCaseSensitive 是否区分大小写
        /// @return 成功返回进程ID，失败返回0
        LSLIB_API DWORD get_process_id_by_name(lpcstr lpProcessName, bool bCaseSensitive = false);

        /// @brief 根据进程ID获取进程路径
        /// @param pid 进程ID
        /// @return 成功返回进程路径，失败返回空字符串
        LSLIB_API string get_process_path_by_id(DWORD pid);

        /// @brief 根据进程名字获取第一个满足条件的进程路径
        /// @param lpProcessName 进程名称
        /// @param bCaseSensitive 是否区分大小写
        /// @return 成功返回进程路径，失败返回空字符串
        LSLIB_API string get_process_path_by_name(lpcstr lpProcessName, bool bCaseSensitive = false);

        /// @brief 结束进程ID
        /// @param pid 进程ID
        /// @return 返回成功或失败
        LSLIB_API bool kill_process_by_id(DWORD pid);

        /// @brief 结束进程名
        /// @param lpProcessName 进程名称
        /// @param bCaseSensitive 是否区分大小写
        /// @return 返回成功或失败
        LSLIB_API bool kill_process_by_name(lpcstr lpProcessName, bool bCaseSensitive = false);

    } // process

} // lslib
