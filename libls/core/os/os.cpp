#include <stdlib.h>
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
#if defined _MSC_VER
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

        LSLIB_API lstring path_get_dir(_lpcstr path)
        {
            if (is_empty(path)) return "";

            lstring str = path_pretty(path);
            while (*str.rbegin() == get_slash())
                str.erase((str.rbegin() + 1).base());
            size_t pos = str.find_last_of(get_slash());
            if (pos == lstring::npos) return "";
            return str.substr(0, pos);
        }

        LSLIB_API lstring path_get_name(_lpcstr path)
        {
            if (is_empty(path)) return "";

            lstring str = path_pretty(path);
            while (*str.rbegin() == get_slash())
                str.erase((str.rbegin() + 1).base());
            size_t pos = str.find_last_of(get_slash());
            return str.substr(pos + 1);
        }

        LSLIB_API lstring path_get_filename(_lpcstr path)
        {
            if (is_empty(path)) return "";

            lstring str = path_get_name(path);
            size_t pos = str.find_last_of('.');
            return str.substr(0, pos);
        }

        LSLIB_API lstring path_get_ext(_lpcstr path)
        {
            if (is_empty(path)) return "";

            lstring str = path_get_name(path);
            size_t pos = str.find_last_of('.');
            if (pos == lstring::npos) return "";
            return str.substr(pos + 1);
        }

        LSLIB_API lstring path_pretty(_lpcstr path)
        {
            if (is_empty(path)) return "";

            lstring str = path;
            str.to_lower();
            str.trim();
            str.replace('/', get_slash());
            str.replace('\\', get_slash());
            _lchar buf[MAX_PATH] = { 0 };
            strncpy(buf, str.c_str(), min(MAX_PATH, str.length()));
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

        LSLIB_API lstring path_combine(_lpcstr path, _lpcstr join)
        {
            if (is_empty(path)) return "";
#ifdef _MSC_VER
            _lchar szbuf[MAX_PATH + 1] =
            {   0};
            PathCombine(szbuf, path_pretty(path), path_pretty(join));
            return szbuf;
#else
            return path_pretty(lstring(path) + "/" + join);
#endif
        }

        LSLIB_API lstring path_make_absolute(_lpcstr path)
        {
            if (is_empty(path)) return "";

            lstring str = path_pretty(path);

            _lchar resolved_path[MAX_PATH + 1] = { 0 };
#ifdef _WIN32
            _fullpath(resolved_path, str.data(), MAX_PATH);
#else
            realpath(str.data(), resolved_path);
#endif
            return resolved_path;
        }

        LSLIB_API const bool is_exist(_lpcstr path)
        {
            if (is_empty(path)) return false;
#ifdef _MSC_VER
            return PathFileExists(path);
#else
            return access(path, 0) == 0;
#endif
        }

        LSLIB_API const bool is_file(_lpcstr path)
        {
            if (is_empty(path)) return false;
            struct stat s;
            lstring str = path_pretty(path);
            str.trim(get_slash());
            if (stat(str, &s) == 0) return s.st_mode & S_IFREG;
            return false;
        }

        LSLIB_API const bool is_dir(_lpcstr path)
        {
            if (is_empty(path)) return false;
            struct stat s;
            lstring str = path_pretty(path);
            str.trim(get_slash());
            if (stat(str, &s) == 0) return s.st_mode & S_IFDIR;
            return false;
        }

        LSLIB_API const int copy(_lpcstr path, _lpcstr target)
        {
#ifdef _MSC_VER
            TCHAR szPath[MAX_PATH + 2] =
            {   0};
            int nLen = min(_tcsclen(path), MAX_PATH);
            memcpy(szPath, path, nLen);

            TCHAR szTarget[MAX_PATH + 2] =
            {   0};
            nLen = min(_tcsclen(target), MAX_PATH);
            memcpy(szTarget, target, nLen);

            SHFILEOPSTRUCT fop =
            {   0};
            fop.wFunc = FO_COPY;
            fop.pFrom = szPath;
            fop.pTo = szTarget;
            fop.fFlags = FOF_NO_UI;
            return SHFileOperation( &fop );
#else
#endif
        }

        LSLIB_API const int move(_lpcstr path, _lpcstr target)
        {
#ifdef _MSC_VER
            TCHAR szPath[MAX_PATH + 2] =
            {   0};
            int nLen = min(_tcsclen(path), MAX_PATH);
            memcpy(szPath, path, nLen);

            TCHAR szTarget[MAX_PATH + 2] =
            {   0};
            nLen = min(_tcsclen(target), MAX_PATH);
            memcpy(szTarget, target, nLen);

            SHFILEOPSTRUCT fop =
            {   0};
            fop.wFunc = FO_MOVE;
            fop.pFrom = szPath;
            fop.pTo = szTarget;
            fop.fFlags = FOF_NO_UI;
            return SHFileOperation( &fop );
#else
#endif
        }

        LSLIB_API const bool rename(_lpcstr path, _lpcstr target)
        {
            return ::rename(path, target);
#ifdef _MSC_VER
            TCHAR szPath[MAX_PATH + 2] =
            {   0};
            int nLen = min(_tcsclen(path), MAX_PATH);
            memcpy(szPath, path, nLen);

            TCHAR szTarget[MAX_PATH + 2] =
            {   0};
            nLen = min(_tcsclen(target), MAX_PATH);
            memcpy(szTarget, target, nLen);

            SHFILEOPSTRUCT fop =
            {   0};
            fop.wFunc = FO_RENAME;
            fop.pFrom = szPath;
            fop.pTo = szTarget;
            fop.fFlags = FOF_NO_UI;
            return SHFileOperation( &fop );
#else
#endif
        }

        LSLIB_API const int mkdir(_lpcstr path)
        {
            if (is_empty(path)) return false;
#ifdef _MSC_VER
            lstring str = path_make_absolute(path);
            return SHCreateDirectoryEx(NULL, str.c_str(), NULL);
#else
#endif
        }

        LSLIB_API const int rm(_lpcstr path)
        {
            if (is_empty(path)) return false;

#ifdef _MSC_VER
            TCHAR szPath[MAX_PATH + 2] =
            {   0};
            int nLen = min(_tcsclen(path), MAX_PATH);
            memcpy(szPath, path, nLen);

            SHFILEOPSTRUCT fop =
            {   0};
            fop.wFunc = FO_DELETE;
            fop.pFrom = szPath; // must be double-null terminated string
            fop.fFlags = FOF_NO_UI;
            return SHFileOperation(&fop);
#endif
        }

        LSLIB_API lstring get_module_file_path()
        {
#ifdef _WIN32
            return _pgmptr;
#else
            char arg1[20];
            char exepath[200] =
            {   0};

            sprintf(arg1, "/proc/%d/exe", getpid());
            readlink(arg1, exepath, 1024);
            return string(exepath);
#endif
        }

        LSLIB_API lstring get_module_path()
        {
            lstring str = get_module_file_path();
            return path_get_dir(str) + get_slash();
        }

        LSLIB_API lstring get_module_name()
        {
            lstring str = get_module_file_path();
            return path_get_filename(str);
        }

#ifdef _MSC_VER

        LSLIB_API lstring get_special_folder_path(int csidl, bool bcreate /*= false*/)
        {
            _lchar sz[MAX_PATH + 1] =
            {   0};
            if (SHGetSpecialFolderPath(NULL, sz, csidl, bcreate))
                return lstring(sz) + get_slash();
            else return "";
        }

        LSLIB_API lstring get_app_data_path()
        {
            return get_special_folder_path(CSIDL_COMMON_APPDATA, true);
        }

        LSLIB_API lstring get_module_app_data_path(bool bcreate /*= false*/)
        {
            lstring str = get_app_data_path();
            if (!str.empty())
            {
                str += path_get_filename(get_module_name()) + get_slash();
                if (bcreate) mkdir(str);
            }
            return str;
        }

        LSLIB_API lstring get_desktop_path()
        {
            return get_special_folder_path(CSIDL_DESKTOP, true);
        }

        LSLIB_API lstring get_quick_launch_path()
        {
            lstring str = get_special_folder_path(CSIDL_APPDATA);
            if (!str.empty())
                str += "Microsoft\\Internet Explorer\\Quick Launch\\User Pinned\\TaskBar\\";
            return str;
        }

        LSLIB_API lstring get_program_files_path()
        {
            return get_special_folder_path(CSIDL_PROGRAM_FILES, true);
        }

        LSLIB_API void open_dir(_lpcstr path, _lpcstr file)
        {
            if (is_empty(path)) return;

            lstring str = path_pretty(path);
            if (!is_empty(file))
                str = lstring("/e,/select, ") + path_combine(path, file);
            ShellExecute(NULL, _T("open"), _T("explorer.exe"), str, NULL, SW_SHOWNORMAL);
        }

        LSLIB_API bool open_file_select_dialog(__out__ lstring_array& arr_files, _lpcstr title, _lpcstr filter, bool multi, HWND owner)
        {
#define nBuffSize (MAX_PATH*100)
            _lchar szFileName[nBuffSize] =
            {   ""};

            OPENFILENAME ofn =
            {   0};
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

                lstring strPath = p;
                p += strPath.length() + 1;

                while (*p != NULL)
                {
                    lstring strName = p;
                    arr_files.push_back(strPath + _T("\\") + strName);
                    p += strName.length() + 1;
                }
                return true;
            }
            return false;
        }

        LSLIB_API bool open_folder_select_dialog(__out__ lstring& target, _lpcstr title, HWND owner)
        {
            BROWSEINFO bInfo =
            {   0};
            bInfo.hwndOwner = owner;
            bInfo.lpszTitle = title;
            bInfo.ulFlags = BIF_RETURNONLYFSDIRS
                            | BIF_USENEWUI /*����һ���༭�� �û������ֶ���д·�� �Ի�����Ե�����С֮���*/
                            | BIF_UAHINT /*��TIPS��ʾ*/
                            | BIF_NONEWFOLDERBUTTON /*�����½��ļ��а�ť*/;
            _lchar szPathName[MAX_PATH] =
            {   0};
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

        LSLIB_API lstring get_product_version(_lpcstr path)
        {
            lstring strFilePath = path;

            if (is_empty(path))
            {
                TCHAR szAppPath[MAX_PATH] =
                {   0};
                ::GetModuleFileName(NULL, szAppPath, MAX_PATH);
                strFilePath = szAppPath;
            }

            lstring strVersion = "";
            VS_FIXEDFILEINFO* vsInfo;
            int ma, mj, r, b;
            UINT i = ::GetFileVersionInfoSize(strFilePath, 0);
            char* szBuf = new char[i];

            if (::GetFileVersionInfo(strFilePath, 0, i, szBuf) == TRUE)
            {
                if (::VerQueryValue(szBuf, _T("\\"), (LPVOID*)&vsInfo, &i))
                {
                    ma = vsInfo->dwProductVersionMS >> 16;
                    mj = vsInfo->dwProductVersionMS & 0xffff;
                    r = vsInfo->dwProductVersionLS >> 16;
                    b = vsInfo->dwProductVersionLS & 0xffff;
                    strVersion.format(_T("%d.%d.%d.%d"), ma, mj, r, b);
                }
            }
            delete[] szBuf;

            return strVersion;
        }

#endif

        LSLIB_API bool enumerate_files(__out__ enum_file_array& array_files, _lpcstr path, _lpcstr extention, _lpcstr filter, bool recurse/* = false*/)
        {
#ifdef _MSC_VER
            if (is_empty(path)) return false;

            lstring strpath = path_pretty(path);
            if (strpath.find_last_of('*') == lstring::npos) // û��ͨ���������Ŀ¼����
                strpath += get_slash() + "*.*";

            lstring str;
            lstring_array arr_exts;
            if (!is_empty(extention))
            {
                _lpcstr p = extention;
                while (*p != NULL)
                {
                    str = p;
                    p += str.length() + 1;
                    if (str == "*.*") continue;
                    arr_exts.push_back(str);
                }
            }

            lstring_array arr_filters;
            if (!is_empty(filter))
            {
                _lpcstr p = filter;
                while (*p != NULL)
                {
                    str = p;
                    p += str.length() + 1;
                    arr_filters.push_back(str);
                }
            }

            WIN32_FIND_DATA fd;
            HANDLE hFile = FindFirstFile(strpath, &fd);
            if (hFile == INVALID_HANDLE_VALUE) return FALSE;

            enum_file file_info;
            bool bfilter = true;
            do
            {
                file_info.filePath = path_get_dir(strpath);
                if (file_info.filePath.empty())
                    file_info.filePath = ".";
                if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) // ���ΪĿ¼
                {
                    if (_tcscmp(fd.cFileName, _T(".")) == 0 || _tcscmp(fd.cFileName, _T("..")) == 0)
                        continue;

                    if (recurse)
                        enumerate_files(array_files, file_info.filePath + get_slash() + fd.cFileName, extention, filter, recurse);
                }
                else
                {
                    file_info.fileName = fd.cFileName;
                    file_info.fullPath = file_info.filePath + get_slash() + file_info.fileName;
                    file_info.size = fd.nFileSizeLow - fd.nFileSizeHigh;
                    file_info.name = path_get_filename(file_info.fileName);
                    file_info.extName = path_get_ext(file_info.fileName);

                    SYSTEMTIME stUTC, stLocal;

                    FileTimeToSystemTime(&(fd.ftCreationTime), &stUTC);
                    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
                    file_info.attr.createTime.format("%04d-%02d-%02d %02d:%02d:%02d", stLocal.wYear, stLocal.wMonth, stLocal.wDay, stLocal.wHour, stLocal.wMinute, stLocal.wSecond);

                    FileTimeToSystemTime(&(fd.ftLastWriteTime), &stUTC);
                    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
                    file_info.attr.writeTime.format("%04d-%02d-%02d %02d:%02d:%02d", stLocal.wYear, stLocal.wMonth, stLocal.wDay, stLocal.wHour, stLocal.wMinute, stLocal.wSecond);

                    FileTimeToSystemTime(&(fd.ftLastAccessTime), &stUTC);
                    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
                    file_info.attr.accessTime.format("%04d-%02d-%02d %02d:%02d:%02d", stLocal.wYear, stLocal.wMonth, stLocal.wDay, stLocal.wHour, stLocal.wMinute, stLocal.wSecond);

                    bfilter = true;
                    if (!arr_exts.empty())
                    {
                        size_t i = 0;
                        for (; i < arr_exts.size(); i++)
                        {
                            if (arr_exts[i] == lstring("*.") + file_info.extName)
                            {
                                bfilter = true;
                                break;
                            }
                        }
                        if (i == arr_exts.size())
                            bfilter = false;
                    }
                    if (!arr_filters.empty())
                    {
                        size_t i = 0;
                        for (; i < arr_filters.size(); i++)
                        {
                            if (file_info.fileName.find(arr_filters[i]) != lstring::npos)
                            {
                                bfilter &= true;
                                break;
                            }
                        }
                        if (i == arr_filters.size())
                            bfilter &= false;
                    }
                    if (bfilter) array_files.push_back(file_info);
                }
            }
            while (::FindNextFile(hFile, &fd));
#else
#endif
            return true;
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
            attr.createTime =  tm.GetDateTimeStr();
            tm.SetDataTime(st.st_mtime);
            attr.writeTime =  tm.GetDateTimeStr();
            tm.SetDataTime(st.st_atime);
            attr.accessTime =  tm.GetDateTimeStr();
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