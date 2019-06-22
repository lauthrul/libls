#include "stdafx.h"
#include "zip.h"
//#include "unzip.h"
#include "zip_x.h"

namespace lslib
{

    namespace zip
    {

        LSLIB_API bool zip_file(_lpcstr dst_zip, _lpcstr src_file, _lpstr pwd /*= NULL*/, _lpcstr name_in_zip /*= NULL*/)
        {
            if (is_empty(dst_zip) || is_empty(src_file))
                return false;

//            HZIP hz = CreateZip(dst_zip, pwd);
//            if (hz == NULL ) return false;
//
//            if (name_in_zip != NULL)    ZipAdd(hz, name_in_zip, src_file);
//            else                        ZipAdd(hz, os::path_get_name(src_file), src_file);
//            CloseZip(hz);
            return true;
        }

        LSLIB_API bool zip_folder(_lpcstr dst_zip, _lpcstr src_dir, _lpstr pwd /*= NULL*/, fn_zip_callback cb /*= NULL*/, void* clientp /*= NULL*/)
        {
            if (is_empty(dst_zip) || is_empty(src_dir))
                return false;

//            lstring root_name = os::path_get_name(src_dir) + os::get_slash();
//
//            HZIP hz = CreateZip(dst_zip, pwd);
//            if (hz == NULL ) return false;
//
//            os::enum_file_array arr_files;
//            os::enumerate_files(arr_files, src_dir, NULL, NULL, true);
//            if (arr_files.size() <= 0)   return false;
//
//            ZRESULT ret = ZR_OK;
//            lstring src;
//            lstring dst;
//            for (size_t i = 0; i < arr_files.size(); i++)
//            {
//                dst = arr_files[i].fullPath;
//                src = dst.substr(dst.find(root_name));
//                ret |= ZipAdd(hz, src, dst);
//                if (cb != NULL) cb(clientp, arr_files.size(), i + 1);
//            }
//            CloseZip(hz);
//            return (ret == ZR_OK);
            return true;
        }

        LSLIB_API bool unzip(_lpcstr src_zip, _lpcstr dst_dir, _lpstr pwd /*= NULL*/, fn_zip_callback cb /*= NULL*/, void* clientp /*= NULL*/)
        {
            bool ret = true;
//            HZIP hz = OpenZip(src_zip, pwd);
//            if (NULL == hz) return false;
//
//            ZIPENTRY zet;
//            lstring target = lstring(dst_dir) + os::get_slash();
//
//            ZIPENTRY ze;
//            GetZipItem(hz, -1, &zet);
//            for (int zi = 0; ret && zi < zet.index; ++zi)
//            {
//                GetZipItem(hz, zi, &ze);
//                if (0 != UnzipItem(hz, zi, (target + ze.name).c_str()))
//                    ret = false;
//
//                if (cb != NULL) cb(clientp, zet.index, zi);
//            }
//            CloseZip(hz);
            return ret;
        }

    } // md5

} // lslib
