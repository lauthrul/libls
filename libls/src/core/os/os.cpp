#include <dirent.h>
#include "stdafx.h"
#include "os.h"

#include <sys/stat.h>

#if defined _MSC_VER
#include <ShlObj.h>
#include <ShellAPI.h>
#include <CommDlg.h>
#else
#include <unistd.h>
#endif

//////////////////////////////////////////////////////////////////////////
namespace lslib
{
    namespace os
    {

        static _lchar s_slashs[] = { '\\', '/' };

        LSLIB_API const _lchar get_slash()
        {
#ifdef _MSC_VER
            return s_slashs[0];
#else
            return s_slashs[1];
#endif
        }

        LSLIB_API const bool is_slash(_lchar c)
        {
            for (size_t i = 0; i < sizeof(s_slashs); i++)
            {
                if (c == s_slashs[i]) return true;
            }
            return false;
        }

        LSLIB_API string path_get_dir(_lpcstr path)
        {
            if (strtool::is_empty(path)) return "";

            string str = path_pretty(path);
            while (*str.rbegin() == get_slash())
                str.erase((str.rbegin() + 1).base());
            size_t pos = str.find_last_of(get_slash());
            if (pos == string::npos) return "";
            return str.substr(0, pos);
        }

        LSLIB_API string path_get_name(_lpcstr path)
        {
            if (strtool::is_empty(path)) return "";

            string str = path_pretty(path);
            while (*str.rbegin() == get_slash())
                str.erase((str.rbegin() + 1).base());
            size_t pos = str.find_last_of(get_slash());
            return str.substr(pos + 1);
        }

        LSLIB_API string path_get_filename(_lpcstr path)
        {
            if (strtool::is_empty(path)) return "";

            string str = path_get_name(path);
            size_t pos = str.find_last_of('.');
            return str.substr(0, pos);
        }

        LSLIB_API string path_get_ext(_lpcstr path)
        {
            if (strtool::is_empty(path)) return "";

            string str = path_get_name(path);
            size_t pos = str.find_last_of('.');
            if (pos == string::npos) return "";
            return str.substr(pos + 1);
        }

        LSLIB_API string path_pretty(_lpcstr path)
        {
            if (strtool::is_empty(path)) return "";

            string str = strtool::trim(path);
            for (size_t i = 0; i < sizeof(s_slashs); i++)
                str = strtool::replace(str, s_slashs[i], get_slash());
            _lchar buf[MAX_PATH] = { 0 };
            strncpy(buf, str.c_str(), MIN(MAX_PATH, str.length()));
#ifdef _MSC_VER
            PathMakePretty(buf);
#else
#endif
            _lpcstr p = buf;
            str.clear();
            while (*p != 0)
            {
                if (!is_slash(*p)) str += *p++;
                else
                {
                    if (!is_slash(*(p + 1))) str += *p++;
                    else p++;
                }
            }
            return str;
        }

        LSLIB_API string path_combine(_lpcstr path, _lpcstr join)
        {
#ifdef _MSC_VER
            _lchar szbuf[MAX_PATH + 1] = { 0 };
            PathCombine(szbuf, path_pretty(path).c_str(), path_pretty(join).c_str());
            return szbuf;
#else
            if (strtool::is_empty(path) || is_absolute(join))
                return path_pretty(join);

            string strpath = string(path) + get_slash() + join;
            return path_pretty(strpath.c_str());
#endif
        }

        LSLIB_API string path_make_absolute(_lpcstr path)
        {
            if (strtool::is_empty(path)) return "";

            string str = path_pretty(path);

            _lchar resolved_path[MAX_PATH + 1] = { 0 };
#ifdef _WIN32
            _fullpath(resolved_path, str.data(), MAX_PATH);
#else
            realpath(str.data(), resolved_path);
#endif
            return path_pretty(resolved_path);
        }

        LSLIB_API const bool is_exist(_lpcstr path)
        {
            if (strtool::is_empty(path)) return false;
#ifdef _MSC_VER
            return PathFileExists(path);
#else
            return access(path, 0) == 0;
#endif
        }

        LSLIB_API const bool is_file(_lpcstr path)
        {
            if (strtool::is_empty(path)) return false;
            struct stat s;
            if (stat(path, &s) == 0) return s.st_mode & S_IFREG;
            return false;
        }

        LSLIB_API const bool is_dir(_lpcstr path)
        {
            if (strtool::is_empty(path)) return false;
            string str = path_pretty(path);
            str = strtool::trim_right(str, get_slash());
            struct stat s;
            if (stat(str.c_str(), &s) == 0) return s.st_mode & S_IFDIR;
            return false;
        }

        LSLIB_API const bool is_absolute(_lpcstr path)
        {
            if (strtool::is_empty(path)) return false;

            string str = path_pretty(path);
            str = strtool::lower(str);
            if (str[0] == get_slash()) return true;
            else if (str[0] == '.') return false;
#ifdef _WIN32
            if (str.length() >= 2 && (str[0] > 'a' && str[0] <= 'z') &&  str[1] == ':')
                return true;
#endif
            return false;
        }

        int copy_single_file(_lpcstr path, _lpcstr target)
        {
            FILE* in, *out;
            if ( (in = fopen(path, "rb")) == NULL) return -1;
            if ( (out = fopen(target, "wb+")) == NULL) return -1;
            char buf[1024] = {0};
            int ret = 0, len = 0;
            while ( (len = fread(buf, 1, 1024, in)) > 0 )
                ret += fwrite(buf, 1, len, out);
            fclose(in);
            fclose(out);
            return ret;
        }

        LSLIB_API const int copy(_lpcstr path, _lpcstr target)
        {
#ifdef _MSC_VER
            TCHAR szPath[MAX_PATH + 2] = { 0 };
            int nLen = MIN(_tcsclen(path), MAX_PATH);
            memcpy(szPath, path, nLen);

            TCHAR szTarget[MAX_PATH + 2] = { 0 };
            nLen = MIN(_tcsclen(target), MAX_PATH);
            memcpy(szTarget, target, nLen);

            SHFILEOPSTRUCT fop = { 0 };
            fop.wFunc = FO_COPY;
            fop.pFrom = szPath;
            fop.pTo = szTarget;
            fop.fFlags = FOF_NO_UI;
            return SHFileOperation( &fop );
#else

            if (is_file(path))
            {
                if (is_dir(target))
                    return copy_single_file(path, path_combine(target, path_get_name(path).c_str()).c_str()) > 0;
                else
                    return copy_single_file(path, target) > 0;
            }
            else if (is_dir(path))
            {
                DIR* d = opendir(path);
                if (d == NULL) return -1;

                int ret = 0;
                if (!is_exist(target)) ret = mkdir(target);
                if (ret < 0) return ret;

                string strpath, strtarget;
                struct dirent* p;
                while (ret == 0 && (p = readdir(d)) != NULL)
                {
                    if (strcmp(p->d_name, ".") == 0 || strcmp(p->d_name, "..") == 0)
                        continue;
                    strpath = path_combine(path, p->d_name);
                    strtarget = path_combine(target, p->d_name);
                    copy(strpath.c_str(), strtarget.c_str());
                }
                closedir(d);
                return ret;
            }
#endif
        }

        LSLIB_API const int move(_lpcstr path, _lpcstr target)
        {
            if (strtool::is_empty(path) || strtool::is_empty(target)) return -1;
#ifdef _MSC_VER
            TCHAR szPath[MAX_PATH + 2] = { 0 };
            int nLen = MIN(_tcsclen(path), MAX_PATH);
            memcpy(szPath, path, nLen);

            TCHAR szTarget[MAX_PATH + 2] = { 0 };
            nLen = MIN(_tcsclen(target), MAX_PATH);
            memcpy(szTarget, target, nLen);

            SHFILEOPSTRUCT fop = { 0 };
            fop.wFunc = FO_MOVE;
            fop.pFrom = szPath;
            fop.pTo = szTarget;
            fop.fFlags = FOF_NO_UI;
            return SHFileOperation( &fop );
#else
            return ::rename(path, target);
#endif
        }

        LSLIB_API const int rename(_lpcstr path, _lpcstr target)
        {
            if (strtool::is_empty(path) || strtool::is_empty(target)) return -1;
#ifdef _MSC_VER
            TCHAR szPath[MAX_PATH + 2] = { 0 };
            int nLen = MIN(_tcsclen(path), MAX_PATH);
            memcpy(szPath, path, nLen);

            TCHAR szTarget[MAX_PATH + 2] = { 0 };
            nLen = MIN(_tcsclen(target), MAX_PATH);
            memcpy(szTarget, target, nLen);

            SHFILEOPSTRUCT fop = { 0 };
            fop.wFunc = FO_RENAME;
            fop.pFrom = szPath;
            fop.pTo = szTarget;
            fop.fFlags = FOF_NO_UI;
            return SHFileOperation( &fop );
#else
            return ::rename(path, target);
#endif
        }

        LSLIB_API const int mkdir(_lpcstr path)
        {
            if (strtool::is_empty(path)) return false;
#ifdef _MSC_VER
            string str = path_make_absolute(path);
            return SHCreateDirectoryEx(NULL, str.c_str(), NULL);
#else


#ifdef WIN32
#define MKDIR(path, mode) ::mkdir(path)
#else
#define MKDIR(path, mode) ::mkdir(path, mode)
#endif

            int ret = 0;
            _lchar slash = get_slash();
            string str = path_pretty(path);
            string strpath;
            {
                size_t idx = 0;
                if (is_absolute(str.c_str()))
                {
                    if (str[0] == slash)    idx = str.find(slash, 1); // linux absolute path
                    else                    idx = str.find(slash, 3); // windows absolute path
                }
                else idx = str.find(slash);

                if (idx != string::npos)
                {
                    strpath = str.substr(0, idx + 1);
                    str = str.substr(idx + 1);
                }
                else
                {
                    strpath = str + slash;
                    str.clear();
                }
            }
            if (!is_exist(strpath.c_str()))
                ret |= MKDIR(strpath.c_str(), 0755);

            string_array arr;
            strtool::split(arr, str, string(1, slash), false);
            for (size_t i = 0; i < arr.size(); i++)
            {
                strpath += arr[i] + slash;
                if (!is_exist(strpath.c_str()))
                    ret |= MKDIR(strpath.c_str(), 0755);
            }
            return ret;
#endif
        }

        LSLIB_API const int rm(_lpcstr path)
        {
            if (strtool::is_empty(path)) return -1;

#ifdef _MSC_VER
            TCHAR szPath[MAX_PATH + 2] = { 0 };
            int nLen = MIN(_tcsclen(path), MAX_PATH);
            memcpy(szPath, path, nLen);

            SHFILEOPSTRUCT fop = { 0 };
            fop.wFunc = FO_DELETE;
            fop.pFrom = szPath; // must be double-null terminated string
            fop.fFlags = FOF_NO_UI;
            return SHFileOperation(&fop);
#else
            if (is_file(path)) return unlink(path);
            else if (is_dir(path))
            {
                DIR* d = opendir(path);
                struct dirent* p;
                int ret = 0;
                string strpath(path);
                while (ret == 0 && (p = readdir(d)) != NULL)
                {
                    if (strcmp(p->d_name, ".") == 0 || strcmp(p->d_name, "..") == 0)
                        continue;
                    strpath = path_combine(path, p->d_name);
                    ret = rm(strpath.c_str());
                }
                closedir(d);
                if (ret == 0) return rmdir(path);
            }
#endif
        }

        LSLIB_API string get_module_file_path()
        {
#ifdef _WIN32
            return _pgmptr;
#else
            char arg1[20];
            char exepath[200] = { 0 };

            sprintf(arg1, "/proc/%d/exe", getpid());
            readlink(arg1, exepath, 1024);
            return string(exepath);
#endif
        }

        LSLIB_API string get_module_path()
        {
            string str = get_module_file_path();
            return path_get_dir(str.c_str()) + get_slash();
        }

        LSLIB_API string get_module_name()
        {
            string str = get_module_file_path();
            return path_get_filename(str.c_str());
        }

#ifdef _MSC_VER

        LSLIB_API string get_special_folder_path(int csidl, bool bcreate /*= false*/)
        {
            _lchar sz[MAX_PATH + 1] = { 0 };
            if (SHGetSpecialFolderPath(NULL, sz, csidl, bcreate))
                return string(sz) + get_slash();
            else return "";
        }

        LSLIB_API string get_app_data_path()
        {
            return get_special_folder_path(CSIDL_COMMON_APPDATA, true);
        }

        LSLIB_API string get_module_app_data_path(bool bcreate /*= false*/)
        {
            string str = get_app_data_path();
            if (!str.empty())
            {
                str += path_get_filename(get_module_name().c_str()) + get_slash();
                if (bcreate) mkdir(str.c_str());
            }
            return str;
        }

        LSLIB_API string get_desktop_path()
        {
            return get_special_folder_path(CSIDL_DESKTOP, true);
        }

        LSLIB_API string get_quick_launch_path()
        {
            string str = get_special_folder_path(CSIDL_APPDATA);
            if (!str.empty())
                str += "Microsoft\\Internet Explorer\\Quick Launch\\User Pinned\\TaskBar\\";
            return str;
        }

        LSLIB_API string get_program_files_path()
        {
            return get_special_folder_path(CSIDL_PROGRAM_FILES, true);
        }

        LSLIB_API void open_dir(_lpcstr path, _lpcstr file)
        {
            if (strtool::is_empty(path)) return;

            string str = path_pretty(path);
            if (!strtool::is_empty(file))
                str = string("/e,/select, ") + path_combine(path, file);
            ShellExecute(NULL, _T("open"), _T("explorer.exe"), str.c_str(), NULL, SW_SHOWNORMAL);
        }

        LSLIB_API bool open_file_select_dialog(__out__ string_array& arr_files, _lpcstr title, _lpcstr filter, bool multi, HWND owner)
        {
#define nBuffSize (MAX_PATH*100)
            _lchar szFileName[nBuffSize] = {   ""};

            OPENFILENAME ofn =
            { 0 };
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.hwndOwner = owner;
            ofn.hInstance = NULL;
            ofn.lpstrFilter = filter;
            ofn.lpstrCustomFilter = NULL;
            ofn.nMaxCustFilter = 0L;
            ofn.nFilterIndex = 1L;
            ofn.lpstrFile = szFileName;
            ofn.nMaxFile = nBuffSize;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;
            ofn.lpstrInitialDir = NULL;
            ofn.lpstrTitle = title;
            ofn.nFileOffset = 0;
            ofn.nFileExtension = 0;
            ofn.lpstrDefExt = _T("*.*");
            ofn.lCustData = 0;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_READONLY | OFN_EXPLORER | (multi ? OFN_ALLOWMULTISELECT : 0);

            if (::GetOpenFileName(&ofn))
            {
                // For Explorer-style dialog boxes, the directory and file name strings are NULL separated, with an extra NULL character after the last file name.
                _lpcstr p = szFileName;

                string strPath = p;
                p += strPath.length() + 1;

                while (*p != NULL)
                {
                    string strName = p;
                    arr_files.push_back(strPath + _T("\\") + strName);
                    p += strName.length() + 1;
                }
                return true;
            }
            return false;
        }

        LSLIB_API bool open_folder_select_dialog(__out__ string& target, _lpcstr title, HWND owner)
        {
            BROWSEINFO bInfo = { 0 };
            bInfo.hwndOwner = owner;
            bInfo.lpszTitle = title;
            bInfo.ulFlags = BIF_RETURNONLYFSDIRS
                            | BIF_USENEWUI /*包含一个编辑框 用户可以手动填写路径 对话框可以调整大小之类的*/
                            | BIF_UAHINT /*带TIPS提示*/
                            | BIF_NONEWFOLDERBUTTON /*不带新建文件夹按钮*/;
            _lchar szPathName[MAX_PATH] = { 0 };
            LPITEMIDLIST lpDlist = SHBrowseForFolder(&bInfo);
            if (lpDlist != NULL)
            {
                SHGetPathFromIDList(lpDlist, szPathName);
                target = szPathName;
                return true;
            }
            return false;
        }

        enum_str_begin(os_type)
        enum_str_item(os_unknown)
        enum_str_item(os_win_nt_4_0)
        enum_str_item(os_win_95)
        enum_str_item(os_win_98)
        enum_str_item(os_win_me)
        enum_str_item(os_win_2000)
        enum_str_item(os_win_xp)
        enum_str_item(os_win_xp_64)
        enum_str_item(os_win_server_2003)
        enum_str_item(os_win_home_server)
        enum_str_item(os_win_server_2003_r2)
        enum_str_item(os_win_vista)
        enum_str_item(os_win_server_2008)
        enum_str_item(os_win_server_2008_r2)
        enum_str_item(os_win_7)
        enum_str_item(os_win_server_2012)
        enum_str_item(os_win_8)
        enum_str_item(os_win_server_2012_r2)
        enum_str_item(os_win_8_1)
        enum_str_item(os_win_server_2016)
        enum_str_item(os_win_10)
        enum_str_end(os_type)

        LSLIB_API os_type get_os_type()
        {
            /* https://docs.microsoft.com/en-us/windows/desktop/api/winnt/ns-winnt-_osversioninfoexa
             Operating system                    Version number  dwMajorVersion  dwMinorVersion  Other
             --------------------------------------------------------------------------------------------------------------------------------------
             Windows?10                          10.0*           10              0               OSVERSIONINFOEX.wProductType == VER_NT_WORKSTATION
             Windows Server?2016                 10.0*           10              0               OSVERSIONINFOEX.wProductType != VER_NT_WORKSTATION
             Windows?8.1                         6.3*            6               3               OSVERSIONINFOEX.wProductType == VER_NT_WORKSTATION
             Windows Server?2012?R2              6.3*            6               3               OSVERSIONINFOEX.wProductType != VER_NT_WORKSTATION
             Windows?8                           6.2             6               2               OSVERSIONINFOEX.wProductType == VER_NT_WORKSTATION
             Windows Server?2012                 6.2             6               2               OSVERSIONINFOEX.wProductType != VER_NT_WORKSTATION
             Windows?7                           6.1             6               1               OSVERSIONINFOEX.wProductType == VER_NT_WORKSTATION
             Windows Server?2008?R2              6.1             6               1               OSVERSIONINFOEX.wProductType != VER_NT_WORKSTATION
             Windows Server?2008                 6               6               0               OSVERSIONINFOEX.wProductType != VER_NT_WORKSTATION
             Windows?Vista                       6               6               0               OSVERSIONINFOEX.wProductType == VER_NT_WORKSTATION
             Windows Server?2003?R2              5.2             5               2               GetSystemMetrics(SM_SERVERR2) != 0
             Windows Home Server                 5.2             5               2               OSVERSIONINFOEX.wSuiteMask & VER_SUITE_WH_SERVER
             Windows Server?2003                 5.2             5               2               GetSystemMetrics(SM_SERVERR2) == 0
             Windows?XP Professional x64 Edition 5.2             5               2               (OSVERSIONINFOEX.wProductType == VER_NT_WORKSTATION) && (SYSTEM_INFO.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)
             Windows?XP                          5.1             5               1               Not applicable
             Windows?2000                        5               5               0               Not applicable
             */
            os_type ostype = os_unknown;

            SYSTEM_INFO info;
            GetSystemInfo(&info);

            OSVERSIONINFOEX os;
            os.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
            if (!GetVersionEx((OSVERSIONINFO*)&os))
                return ostype;

#define compare_ver(major, minor, other)\
    (os.dwMajorVersion == (major) && os.dwMinorVersion == (minor) && (other))

            if (compare_ver(4, 0, os.dwPlatformId == VER_PLATFORM_WIN32_NT)) return os_win_nt_4_0;
            else if (compare_ver(4, 0, os.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)) return os_win_95;
            else if (compare_ver(4, 10, true)) return os_win_98;
            else if (compare_ver(4, 90, true)) return os_win_me;
            else if (compare_ver(5, 0, true)) return os_win_2000;
            else if (compare_ver(5, 1, true)) return os_win_xp;
            else if (compare_ver(5, 2, (os.wProductType == VER_NT_WORKSTATION)
                                 && (info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)))
                return os_win_xp_64;
            else if (compare_ver(5, 2, GetSystemMetrics(SM_SERVERR2) == 0)) return os_win_server_2003;
            else if (compare_ver(5, 2, os.wSuiteMask & 0x00008000/*VER_SUITE_WH_SERVER*/)) return os_win_home_server;
            else if (compare_ver(5, 2, GetSystemMetrics(SM_SERVERR2) != 0)) return os_win_server_2003_r2;
            else if (compare_ver(6, 0, os.wProductType == VER_NT_WORKSTATION)) return os_win_vista;
            else if (compare_ver(6, 0, os.wProductType != VER_NT_WORKSTATION)) return os_win_server_2008;
            else if (compare_ver(6, 1, os.wProductType != VER_NT_WORKSTATION)) return os_win_server_2008_r2;
            else if (compare_ver(6, 1, os.wProductType == VER_NT_WORKSTATION)) return os_win_7;
            else if (compare_ver(6, 2, os.wProductType != VER_NT_WORKSTATION)) return os_win_server_2012;
            else if (compare_ver(6, 2, os.wProductType == VER_NT_WORKSTATION)) return os_win_8;
            else if (compare_ver(6, 3, os.wProductType != VER_NT_WORKSTATION)) return os_win_server_2012_r2;
            else if (compare_ver(6, 3, os.wProductType == VER_NT_WORKSTATION)) return os_win_8_1;
            else if (compare_ver(10, 0, os.wProductType != VER_NT_WORKSTATION)) return os_win_server_2016;
            else if (compare_ver(10, 0, os.wProductType == VER_NT_WORKSTATION)) return os_win_10;
            return os_unknown;
        }

        LSLIB_API string get_product_version(_lpcstr path)
        {
            string strFilePath = path;

            if (strtool::is_empty(path))
            {
                TCHAR szAppPath[MAX_PATH] = { 0 };
                ::GetModuleFileName(NULL, szAppPath, MAX_PATH);
                strFilePath = szAppPath;
            }

            string strVersion = "";
            VS_FIXEDFILEINFO* vsInfo;
            int ma, mj, r, b;
            UINT i = ::GetFileVersionInfoSize(strFilePath.c_str(), 0);
            char* szBuf = new char[i];

            if (::GetFileVersionInfo(strFilePath.c_str(), 0, i, szBuf) == TRUE)
            {
                if (::VerQueryValue(szBuf, _T("\\"), (LPVOID*)&vsInfo, &i))
                {
                    ma = vsInfo->dwProductVersionMS >> 16;
                    mj = vsInfo->dwProductVersionMS & 0xffff;
                    r = vsInfo->dwProductVersionLS >> 16;
                    b = vsInfo->dwProductVersionLS & 0xffff;
                    strVersion = strtool::format(_T("%d.%d.%d.%d"), ma, mj, r, b);
                }
            }
            delete[] szBuf;

            return strVersion;
        }

#endif

        LSLIB_API void enumerate_files(__out__ enum_file_array& array_files, _lpcstr path, _lpcstr extention, _lpcstr filter, bool recurse/* = false*/)
        {
            if (strtool::is_empty(path)) return;
            if (!is_dir(path)) return;

            string str;
            string_array arr_exts;
            string_array arr_filters;
            strtool::split(arr_exts, extention, ";", false);
            strtool::split(arr_filters, filter, ";", false);

            bool bfilter = true;
            enum_file file_info;

#define check(file_info, bfilter) \
    {\
        if (!arr_exts.empty())\
        {\
            size_t i = 0;\
            for (; i < arr_exts.size(); i++)\
            {\
                if (arr_exts[i] == "*.*" || arr_exts[i] == string("*.") + file_info.extName)\
                {\
                    bfilter = true;\
                    break;\
                }\
            }\
            if (i == arr_exts.size())\
                bfilter = false;\
        }\
        if (!arr_filters.empty())\
        {\
            size_t i = 0;\
            for (; i < arr_filters.size(); i++)\
            {\
                if (file_info.fullPath.find(arr_filters[i]) != string::npos)\
                {\
                    bfilter &= true;\
                    break;\
                }\
            }\
            if (i == arr_filters.size())\
                bfilter &= false;\
        }\
    }

#ifdef _MSC_VER

            WIN32_FIND_DATA fd;
            HANDLE hFile = FindFirstFile(path_combine(path, "*.*").c_str(), &fd);
            if (hFile == INVALID_HANDLE_VALUE) return;

            do
            {
                file_info.filePath = path;
                if (file_info.filePath.empty())
                    file_info.filePath = ".";
                if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) // 如果为目录
                {
                    if (_tcscmp(fd.cFileName, _T(".")) == 0 || _tcscmp(fd.cFileName, _T("..")) == 0)
                        continue;
                    if (recurse)
                        enumerate_files(array_files, path_combine(path, fd.cFileName).c_str(), extention, filter, recurse);
                }
                else
                {
                    file_info.fileName = fd.cFileName;
                    file_info.fullPath = path_combine(file_info.filePath.c_str(), file_info.fileName.c_str());
                    file_info.size = fd.nFileSizeLow - fd.nFileSizeHigh;
                    file_info.name = path_get_filename(file_info.fileName.c_str());
                    file_info.extName = path_get_ext(file_info.fileName.c_str());

                    check(file_info, bfilter);
                    if (bfilter)
                    {
                        SYSTEMTIME stUTC, stLocal;

#define get_time(out, time)\
    FileTimeToSystemTime(&(time), &stUTC);\
    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);\
    out = strtool::format("%04d-%02d-%02d %02d:%02d:%02d", stLocal.wYear, stLocal.wMonth, stLocal.wDay, stLocal.wHour, stLocal.wMinute, stLocal.wSecond);

                        get_time(file_info.attr.createTime, fd.ftCreationTime);
                        get_time(file_info.attr.writeTime, fd.ftLastWriteTime);
                        get_time(file_info.attr.accessTime, fd.ftLastAccessTime);

                        array_files.push_back(file_info);
                    }
                }
            }
            while (::FindNextFile(hFile, &fd));
            FindClose(hFile);

#else
            DIR* d = opendir(path);
            struct dirent* p;
            string curpath;
            while ((p = readdir(d)) != NULL)
            {
                if (strcmp(p->d_name, ".") == 0 || strcmp(p->d_name, "..") == 0)
                    continue;
                curpath = path_combine(path, p->d_name);
                struct stat statbuf;
                if (stat(curpath.c_str(), &statbuf) == 0)
                {
                    if (S_ISDIR(statbuf.st_mode))
                    {
                        if (recurse)
                            enumerate_files(array_files, curpath.c_str(),  extention,  filter, recurse);
                    }
                    else
                    {
                        file_info.filePath = path_get_dir(curpath.c_str());
                        file_info.fileName = path_get_name(curpath.c_str());
                        file_info.fullPath = curpath;
                        file_info.name = path_get_filename(file_info.fileName.c_str());
                        file_info.extName = path_get_ext(file_info.fileName.c_str());

                        check(file_info, bfilter);
                        if (bfilter)
                        {
                            file_info.size = get_file_size(curpath.c_str());
                            file_info.attr = get_file_attr(curpath.c_str());
                            array_files.push_back(file_info);
                        }
                    }
                }
            }
            closedir(d);
#endif
        }

        LSLIB_API _ldword get_file_size(_lpcstr file)
        {
            _ldword ret = 0;
            FILE* fp = fopen(file, "rb");
            if (fp)
            {
                fseek(fp, 0, SEEK_END);
                ret = ftell(fp);
                fclose(fp);
            }
            return ret;
        }

        LSLIB_API file_attr get_file_attr(_lpcstr file)
        {
            file_attr attr;
            struct stat st;
            stat(file, &st);
            Time tm;
            tm.SetDataTime(st.st_ctime);
            attr.createTime = tm.GetDateTimeStr();
            tm.SetDataTime(st.st_mtime);
            attr.writeTime = tm.GetDateTimeStr();
            tm.SetDataTime(st.st_atime);
            attr.accessTime = tm.GetDateTimeStr();
            return attr;
        }

        LSLIB_API _lpbyte get_file_buffer(_lpcstr file, __out__ _lpdword outsize)
        {
            _ldword size = get_file_size(file);
            if (size <= 0) return NULL;

            _lpbyte buf = new byte[size + 1];
            memset(buf, 0, size + 1);
            FILE* fp = fopen(file, "rb");
            if (fp)
            {
                size = fread(buf, 1, size, fp);
                fclose(fp);
            }
            if (outsize != NULL) *outsize = size;
            return buf;
        }

        LSLIB_API bool release_file_buffer(_lpbyte data)
        {
            if (data == NULL) return false;
            delete[] data;
            return true;
        }

        LSLIB_API _ldword save_buffer_to_file(_lpbyte data, _ldword size, _lpcstr file, int flag)
        {
            _ldword write_size = 0;
            FILE* fp = NULL;
            if (flag == 0) fp = fopen(file, "wb+");
            else fp = fopen(file, "ab+");
            if (fp)
            {
                write_size = fwrite(data, 1, size, fp);
                fclose(fp);
            }
            return write_size;
        }
    } // os
} // lslib
