#include "stdafx.h"
#include "process.h"
#include "psapi.h"

//////////////////////////////////////////////////////////////////////////
namespace lslib
{
    namespace process
    {
        LSLIB_API bool get_process_info_by_id(__out__ PROCESSENTRY32& pe, DWORD pid)
        {
            HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
            pe.dwSize = sizeof(PROCESSENTRY32);
            if (!Process32First(hSnapShot, &pe))
                return false;

            while (Process32Next(hSnapShot, &pe))
            {
                if (pe.th32ProcessID == pid)
                    return true;
            }
            return false;
        }

        LSLIB_API bool get_process_info_by_name(__out__ PROCESSENTRY32_list& pe_lst, lpcstr lpProcessName, bool bCaseSensitive /*= false*/)
        {
            HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
            PROCESSENTRY32 pe;
            pe.dwSize = sizeof(PROCESSENTRY32);
            if (!Process32First(hSnapShot, &pe))
                return false;

            string strProcessName = lpProcessName;
            if (!bCaseSensitive)
                strProcessName = strtool::lower(strProcessName);
            while (Process32Next(hSnapShot, &pe))
            {
                string scTmp = pe.szExeFile;
                if (!bCaseSensitive)
                    scTmp = strtool::lower(scTmp);
                if (scTmp == strProcessName)
                    pe_lst.push_back(pe);
            }
            return !pe_lst.empty();
        }

        LSLIB_API DWORD get_process_id_by_name(lpcstr lpProcessName, bool bCaseSensitive /*= false*/)
        {
            PROCESSENTRY32_list pe_lst;
            if (get_process_info_by_name(pe_lst, lpProcessName, bCaseSensitive))
                return (*pe_lst.begin()).th32ProcessID;
            return 0;
        }

        LSLIB_API string get_process_path_by_id(DWORD pid)
        {
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, pid);
            TCHAR tszTemp[MAX_PATH + 1] = {0};
            DWORD dwSize = MAX_PATH;
            GetProcessImageFileName(hProcess, tszTemp, dwSize);
            CloseHandle(hProcess);
            return tszTemp;
        }

        LSLIB_API string get_process_path_by_name(lpcstr lpProcessName, bool bCaseSensitive /*= false*/)
        {
            return get_process_path_by_id(get_process_id_by_name(lpProcessName, bCaseSensitive));
        }

        LSLIB_API bool kill_process_by_id(DWORD pid)
        {
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, false, pid);
            bool ret = ::TerminateProcess(hProcess, 0);
            CloseHandle(hProcess);
            return ret;
        }

        LSLIB_API bool kill_process_by_name(lpcstr lpProcessName, bool bCaseSensitive /*= false*/)
        {
            bool ret = false;
            PROCESSENTRY32_list pe_lst;
            if (get_process_info_by_name(pe_lst, lpProcessName, bCaseSensitive))
            {
                ret = true;
                for (PROCESSENTRY32_list::iterator it = pe_lst.begin(); it != pe_lst.end(); it++)
                {
                    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, false, it->th32ProcessID);
                    ret &= ::TerminateProcess(hProcess, 0);
                    CloseHandle(hProcess);
                }
            }
            return  ret;
        }

    } // process

} // lslib
