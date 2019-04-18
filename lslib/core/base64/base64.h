#pragma once

namespace lslib
{

	class LSLIB_API base64
	{
	public:
		static lstring encode(_lpcstr data, int len);
		static lstring decode(_lpcstr data, int len, __out int* outlen);
	};

} // lslib
