#pragma once

namespace lslib
{
	namespace zip
	{
		typedef int fn_zip_callback(void *clientp, double dltotal, double dlnow);

		LSLIB_API bool zip_file(_lpcstr dst_zip, _lpcstr src_file, _lpstr pwd = NULL, _lpcstr name_in_zip = NULL);

		LSLIB_API bool zip_folder(_lpcstr dst_zip, _lpcstr src_dir, _lpstr pwd = NULL, fn_zip_callback cb = NULL, void *clientp = NULL);

		LSLIB_API bool unzip(_lpcstr src_zip, _lpcstr dst_dir, _lpstr pwd = NULL, fn_zip_callback cb = NULL, void *clientp = NULL);
	};
}