#pragma once

namespace lslib
{
    namespace zip
    {
        // if encounter error, when this call back return value is 0 , then continue, otherwise stop zip/unzip progress.
        typedef int fn_zip_callback(void* clientp, int dltotal, int dlnow, int error);

        // under linux, miniz lib is used by default, which means password is not supported. so @pwd param is ignored.

        LSLIB_API int zip_file(_lpcstr dst_zip, _lpcstr src_file, _lpstr pwd = NULL, _lpcstr name_in_zip = NULL);

        LSLIB_API int zip_folder(_lpcstr dst_zip, _lpcstr src_dir, _lpstr pwd = NULL, fn_zip_callback cb = NULL, void* clientp = NULL);

        LSLIB_API int unzip(_lpcstr src_zip, _lpcstr dst_dir, _lpstr pwd = NULL, fn_zip_callback cb = NULL, void* clientp = NULL);
    };
}