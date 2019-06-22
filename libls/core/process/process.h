#pragma once
#include <tlhelp32.h>

//////////////////////////////////////////////////////////////////////////
namespace lslib
{
    namespace process
    {
        declare_stl_obj(PROCESSENTRY32);

        LSLIB_API bool get_process_info_by_id(__out PROCESSENTRY32& pe, DWORD pid);

        LSLIB_API bool get_process_info_by_name(__out PROCESSENTRY32_list& pe_lst, _lpcstr lpProcessName, bool bCaseSensitive);

        // get the first one if multi processes have the same name
        LSLIB_API DWORD get_process_id_by_name(_lpcstr lpProcessName);

        LSLIB_API lstring get_process_path_by_id(DWORD pid);

        // get the first one if multi processes have the same name
        LSLIB_API lstring get_process_path_by_name(_lpcstr lpProcessName);

        LSLIB_API bool kill_process_by_id(DWORD pid);

        // kill all process which have the same name
        LSLIB_API bool kill_process_by_name(_lpcstr lpProcessName);

    } // process

} // lslib
