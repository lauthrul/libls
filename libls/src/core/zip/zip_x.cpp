#include "stdafx.h"
#include "zip_x.h"

#ifdef _MSC_VER
#define USE_ZIPUNZIP
#else
#define USE_MINIZ
#endif

#if defined(USE_ZIPUNZIP)
#include "zipunzip/zip.h"
#include "zipunzip/unzip.h"
#elif defined (USE_MINIZ)
#include "miniz/mzip.h"
#endif

namespace lslib
{

    namespace zip
    {

#if defined(USE_ZIPUNZIP) // use zip/unzip lib (only in windows)

        LSLIB_API int zip_file(lpcstr dst_zip, lpcstr src_file, lpstr pwd /*= NULL*/, lpcstr name_in_zip /*= NULL*/)
        {
            if (strtool::is_empty(dst_zip) || strtool::is_empty(src_file))
                return -0xf1;

            HZIP hz = CreateZip(dst_zip, pwd);
            if (hz == NULL ) return -0xf2;

            ZRESULT ret = ZR_OK;
            string strname = name_in_zip;
            if (strname.empty())    strname = os::path_get_name(src_file);
            ret = ZipAdd(hz, strname.c_str(), src_file);
            CloseZip(hz);
            return ret;
        }

        LSLIB_API int zip_folder(lpcstr dst_zip, lpcstr src_dir, lpstr pwd /*= NULL*/, fn_zip_callback cb /*= NULL*/, void* clientp /*= NULL*/)
        {
            if (strtool::is_empty(dst_zip) || strtool::is_empty(src_dir))
                return -0xf1;

            string root_name = os::path_get_name(src_dir) + os::get_slash();

            HZIP hz = CreateZip(dst_zip, pwd);
            if (hz == NULL ) return -0xf2;

            os::enum_file_array arr_files;
            os::enumerate_files(arr_files, src_dir, NULL, NULL, true);
            if (arr_files.size() <= 0)   return -0xf3;

            ZRESULT ret = ZR_OK;
            string src;
            string dst;
            for (size_t i = 0; ret == ZR_OK && i < arr_files.size(); i++)
            {
                src = arr_files[i].fullPath;
                dst = src.substr(src.find(root_name));
                ret = ZipAdd(hz, dst.c_str(), src.c_str());
                if (cb != NULL) ret = cb(clientp, arr_files.size(), i + 1, ret);
            }
            CloseZip(hz);
            return ret;
        }

        LSLIB_API int unzip(lpcstr src_zip, lpcstr dst_dir, lpstr pwd /*= NULL*/, fn_zip_callback cb /*= NULL*/, void* clientp /*= NULL*/)
        {
            if (strtool::is_empty(src_zip) || strtool::is_empty(dst_dir))
                return -0xf1;

            HZIP hz = OpenZip(src_zip, pwd);
            if (NULL == hz) return -0xf2;

            ZRESULT ret = ZR_OK;
            ZIPENTRY ze;
            GetZipItem(hz, -1, &ze);
            int counts = ze.index;
            for (int i = 0; ret == ZR_OK && i < counts; ++i)
            {
                ret = GetZipItem(hz, i, &ze);
                if (ret == ZR_OK)
                {
                    string target = os::path_combine(dst_dir, ze.name);
                    ret = UnzipItem(hz, i, target.c_str());
                }
                if (cb != NULL) ret = cb(clientp, counts, i + 1, ret);
            }
            CloseZip(hz);
            return ret;
        }

#elif defined (USE_MINIZ) // use miniz lib (windows/linux)

        LSLIB_API int zip_file(lpcstr dst_zip, lpcstr src_file, lpstr pwd /*= NULL*/, lpcstr name_in_zip /*= NULL*/)
        {
            if (strtool::is_empty(dst_zip) || strtool::is_empty(src_file))
                return -0xf1;

            struct zip_t* zip = zip_open(dst_zip, ZIP_DEFAULT_COMPRESSION_LEVEL, 'w');
            if (zip == NULL) return -0xf2;

            int ret = 0;
            string strname = name_in_zip;
            if (strname.empty()) strname = os::path_get_name(src_file);
            if (ret == 0) ret = zip_entry_open(zip, strname.c_str());
            if (ret == 0) ret = zip_entry_fwrite(zip, src_file);
            if (ret == 0) ret = zip_entry_close(zip);
            zip_close(zip);
            return ret;
        }

        LSLIB_API int zip_folder(lpcstr dst_zip, lpcstr src_dir, lpstr pwd /*= NULL*/, fn_zip_callback cb /*= NULL*/, void* clientp /*= NULL*/)
        {
            if (strtool::is_empty(dst_zip) || strtool::is_empty(src_dir))
                return -0xf1;

            string root_name = os::path_get_name(src_dir) + os::get_slash();

            struct zip_t* zip = zip_open(dst_zip, ZIP_DEFAULT_COMPRESSION_LEVEL, 'w');
            if (zip == NULL) return -0xf2;

            os::enum_file_array arr_files;
            os::enumerate_files(arr_files, src_dir, NULL, NULL, true);
            if (arr_files.size() <= 0)   return -3;

            int ret = 0;
            string src;
            string dst;
            for (size_t i = 0; ret == 0 && i < arr_files.size(); i++)
            {
                src = arr_files[i].fullPath;
                dst = src.substr(src.find(root_name));
                if (ret == 0) ret = zip_entry_open(zip, dst.c_str());
                if (ret == 0) ret = zip_entry_fwrite(zip, src.c_str());
                if (ret == 0) ret = zip_entry_close(zip);
                if (cb != NULL) ret = cb(clientp, arr_files.size(), i + 1, ret);
            }
            zip_close(zip);
            return ret;
        }

        LSLIB_API int unzip(lpcstr src_zip, lpcstr dst_dir, lpstr pwd /*= NULL*/, fn_zip_callback cb /*= NULL*/, void* clientp /*= NULL*/)
        {
            if (strtool::is_empty(src_zip) || strtool::is_empty(dst_dir))
                return -0x1;

            struct zip_t* zip = zip_open(src_zip, ZIP_DEFAULT_COMPRESSION_LEVEL, 'r');
            if (zip == NULL) return -0x2;

            int ret = 0;
            int counts = zip_total_entries(zip);
            for (int i = 0; ret == 0 && i < counts; ++i)
            {
                ret = zip_entry_openbyindex(zip, i);
                string target = os::path_combine(dst_dir, zip_entry_name(zip));
                os::mkdir(os::path_get_dir(target.c_str()).c_str());
                if (ret == 0) ret = zip_entry_fread(zip, target.c_str());
                if (ret == 0) ret = zip_entry_close(zip);
                if (cb != NULL) ret = cb(clientp, counts, i + 1, ret);
            }
            zip_close(zip);
            return ret;
        }

#endif

    } // md5

} // lslib
