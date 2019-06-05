#include "stdafx.h"
#include "algorithm.h"
#include "md5.h"
#include "sha1.h"
#include "url_encode.h"

namespace lslib
{

    namespace md5
    {
        LSLIB_API lstring md5(_lpcstr str, int len)
        {
            if (is_empty(str) || len <= 0) return "";

            MD5_CTX ctx;
            MD5Init(&ctx);
            MD5Update(&ctx, (_luchar*)str, len);
            MD5Final(&ctx);

            lstring strret;
            for (int i = 0; i < 16; i++)
                strret.append_format("%02x", ctx.digest[i]);
            return strret;
        }

        LSLIB_API lstring file_md5(_lpcstr pfile)
        {
            if (is_empty(pfile)) return "";

            FILE* fp = fopen(pfile, "rb");
            if (fp == NULL) return "";

            fseek(fp, 0, SEEK_END);
            int size = ftell(fp);
            fseek(fp, 0, SEEK_SET);

            _lpstr pbuf = new _lchar[size + 1];
            memset(pbuf, 0, size + 1);
            fread(pbuf, 1, size, fp);
            fclose(fp);

            lstring strret = md5(pbuf, size);
            delete[] pbuf;
            return strret;
        }

    } // md5

    namespace sha1
    {
        LSLIB_API lstring sha1(_lpcstr str, int len)
        {
            if (is_empty(str) || len <= 0) return "";

            unsigned char digest[20] = {0};
            SHA1_CTX ctx;
            SHA1Init(&ctx);
            SHA1Update(&ctx, (_luchar*)str, len);
            SHA1Final(&ctx);

            lstring strret;
            for (int i = 0; i < 20; i++)
                strret.append_format("%02x", ctx.digest[i]);
            return strret;
        }

        LSLIB_API lstring file_sha1(_lpcstr pfile)
        {
            if (is_empty(pfile)) return "";

            FILE* fp = fopen(pfile, "rb");
            if (fp == NULL) return "";

            fseek(fp, 0, SEEK_END);
            int size = ftell(fp);
            fseek(fp, 0, SEEK_SET);

            _lpstr pbuf = new _lchar[size + 1];
            memset(pbuf, 0, size + 1);
            fread(pbuf, 1, size, fp);
            fclose(fp);

            lstring strret = sha1(pbuf, size);
            delete[] pbuf;
            return strret;
        }
    } // sha1

    namespace base64
    {
        lstring base64::encode(_lpcstr data, int len, int line_len /*= -1*/)
        {
            if (is_empty(data) || len <= 0) return "";

            //编码表
            const char enmap[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

            lstring stren;
            unsigned char tmp[4] = { 0 };
            int line_len_cnt = 0;
            for (int i = 0; i < (int)(len / 3); i++)
            {
                tmp[1] = *data++;
                tmp[2] = *data++;
                tmp[3] = *data++;
                stren += enmap[tmp[1] >> 2];
                stren += enmap[((tmp[1] << 4) | (tmp[2] >> 4)) & 0x3F];
                stren += enmap[((tmp[2] << 2) | (tmp[3] >> 6)) & 0x3F];
                stren += enmap[tmp[3] & 0x3F];
                if (line_len > 0)
                {
                    if (line_len_cnt += 4, line_len_cnt == line_len)
                    {
                        stren += "\r\n";
                        line_len_cnt = 0;
                    }
                }
            }

            //对剩余数据进行编码
            int mod = len % 3;
            if (mod == 1)
            {
                tmp[1] = *data++;
                stren += enmap[(tmp[1] & 0xFC) >> 2];
                stren += enmap[((tmp[1] & 0x03) << 4)];
                stren += "==";
            }
            else if (mod == 2)
            {
                tmp[1] = *data++;
                tmp[2] = *data++;
                stren += enmap[(tmp[1] & 0xFC) >> 2];
                stren += enmap[((tmp[1] & 0x03) << 4) | ((tmp[2] & 0xF0) >> 4)];
                stren += enmap[((tmp[2] & 0x0F) << 2)];
                stren += "=";
            }

            return stren;
        }

        lstring base64::decode(_lpcstr data, int len, __out int* outlen)
        {
            if (is_empty(data) || len <= 0) return "";

            //解码表
            const char demap[] =
            {
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                62, // '+'
                0, 0, 0,
                63, // '/'
                52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
                0, 0, 0, 0, 0, 0, 0,
                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
                0, 0, 0, 0, 0, 0,
                26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
                39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
            };

            string strde;
            int value;
            int i = 0;
            int counts = 0;
            while (i < len)
            {
                if (*data != '\r' && *data != '\n')
                {
                    value = demap[*data++] << 18;
                    value += demap[*data++] << 12;
                    strde += (value & 0x00FF0000) >> 16;
                    counts++;
                    if (*data != '=')
                    {
                        value += demap[*data++] << 6;
                        strde += (value & 0x0000FF00) >> 8;
                        counts++;
                        if (*data != '=')
                        {
                            value += demap[*data++];
                            strde += value & 0x000000FF;
                            counts++;
                        }
                    }
                    i += 4;
                }
                else// 回车换行,跳过
                {
                    data++;
                    i++;
                }
            }
            if (outlen != NULL) *outlen = counts;
            return strde;
        }
    } // base64

    namespace url_encode
    {
        LSLIB_API lstring encode(_lpcstr data, int len)
        {
            int out_len = ls_url_encode(data, len, NULL, 0);
            char* pbuf = new char[out_len + 1];
            memset(pbuf, 0, out_len + 1);
            ls_url_encode(data, len, pbuf, out_len);
            lstring str; str.assign(pbuf, out_len);
            delete[] pbuf;
            return str;
        }

        LSLIB_API lstring decode(_lpcstr data, int len)
        {
            char* pbuf = new char[len + 1];
            memset(pbuf, 0, len + 1);
            memcpy(pbuf, data, len);
            int out_len = ls_url_decode(pbuf, len);
            lstring str; str.assign(pbuf, out_len);
            delete[] pbuf;
            return str;
        }

    } // url_encode

} // lslib
