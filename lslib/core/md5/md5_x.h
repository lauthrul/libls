#pragma once

namespace lslib
{
	namespace md5
	{
		LSLIB_API const lstring md5(_lpcstr str);
		LSLIB_API const lstring file_md5(_lpcstr pfile);
	};
}