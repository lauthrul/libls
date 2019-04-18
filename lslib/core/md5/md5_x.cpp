#include "stdafx.h"
#include "md5.h"
#include "md5_x.h"

namespace lslib
{

	namespace md5
	{
		LSLIB_API const lstring md5(_lpcstr str)
		{
			MD5_CTX mdctx;
			MD5Init(&mdctx);
			MD5Update(&mdctx, (_luchar*)str, strlen(str));
			MD5Final(&mdctx);

			lstring strret;
			for (int i = 0; i < 16; i++)
				strret.append_format("%02x", mdctx.digest[i]);
			return strret;
		}

		LSLIB_API const lstring file_md5(_lpcstr pfile)
		{
			if (is_empty(pfile)) return "";

			FILE *fp = fopen(pfile, "r");
			if (fp == NULL) return "";

			fseek(fp, 0, SEEK_END);
			int size = ftell(fp);
			fseek(fp, 0, SEEK_SET);

			_lpstr pbuf = new _lchar[size + 1];
			memset(pbuf, 0, size + 1);
			fread(pbuf, 1, size, fp);
			fclose(fp);

			lstring strret = md5(pbuf);
			delete[] pbuf;
			return strret;
		}

	} // md5

} // lslib
