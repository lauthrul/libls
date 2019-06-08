#include "stdafx.h"
#include "crypto.h"
#include "md5.h"
#include "sha1.h"
#include "sha256.h"
#include "base64.h"
#include "aes.h"
#include "url_encode.h"
#include <math.h>

namespace lslib
{
    namespace crypto
    {
        LSLIB_API lstring md5(_lpcstr data, int len)
        {
            MD5_CTX ctx;
            MD5Init(&ctx);
            MD5Update(&ctx, (_lpcustr)data, len);
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

        LSLIB_API lstring sha1(_lpcstr data, int len)
        {
            SHA1_CTX ctx;
            SHA1Init(&ctx);
            SHA1Update(&ctx, (_lpcustr)data, len);
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

        LSLIB_API lstring sha224(_lpcstr data, int len)
        {
            return "";
        }

        LSLIB_API lstring file_sha224(_lpcstr pfile)
        {
            return "";
        }

        LSLIB_API lstring sha256(_lpcstr data, int len)
        {
            uint8_t hash[SHA256_BYTES] = {0};
            ::sha256(data, len, hash);
            lstring strret;
            for (int i = 0; i < SHA256_BYTES; i++)
                strret.append_format("%02x", hash[i]);
            return strret;
        }

        LSLIB_API lstring file_sha256(_lpcstr pfile)
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

            lstring strret = sha256(pbuf, size);
            delete[] pbuf;
            return strret;
        }

        LSLIB_API lstring sha384(_lpcstr data, int len)
        {
            return "";
        }

        LSLIB_API lstring file_sha384(_lpcstr pfile)
        {
            return "";
        }

        LSLIB_API lstring sha512(_lpcstr data, int len)
        {
            return "";
        }

        LSLIB_API lstring file_sha512(_lpcstr pfile)
        {
            return "";
        }

        LSLIB_API lstring base64_encode(_lpcstr data, int len)
        {
            int outlen = estimate_base64_encode_len(len);
            _lpstr pbuf = new _lchar[outlen + 1];
            memset(pbuf, 0, outlen + 1);
            outlen = ::base64_encode((_lpcustr)data, len, pbuf);
            lstring strrect;
            if (outlen > 0) strrect.assign(pbuf, outlen);
            return strrect;
        }

        LSLIB_API lstring base64_decode(_lpcstr data, int len, __out int* out_len)
        {
            int estimated_out_len = estimate_base64_decode_len(len);
            _lpustr pbuf = new _luchar[estimated_out_len + 1];
            memset(pbuf, 0, estimated_out_len + 1);
            estimated_out_len = ::base64_decode(data, len, pbuf);
            lstring strrect;
            if (estimated_out_len > 0)
            {
                strrect.assign((_lpcstr)pbuf, estimated_out_len);
                if (out_len != NULL) *out_len = estimated_out_len;
            }
            return strrect;
        }

        LSLIB_API lstring aes_encode(_lpcstr data, int data_len, _lpcstr key, aes_key_bits key_bits, aes_padding_mode mode, __out int* out_len)
        {
            uint32_t key_schedule[60] = {0};
            uint8_t enc_buf[AES_BLOCK_SIZE] = {0};

            aes_key_setup((uint8_t*)key, key_schedule, key_bits);

            int padding_len = AES_BLOCK_SIZE - data_len % AES_BLOCK_SIZE;
            uint8_t* data_buf = new uint8_t[data_len + padding_len];
            memcpy(data_buf, data, data_len);

            switch (mode)
            {
                case aes_zeropadding:
                    memset(data_buf + data_len, 0, padding_len);
                    break;
                case aes_pkcs5padding:
                case aes_pkcs7padding:
                    memset(data_buf + data_len, padding_len, padding_len);
                    break;
            }

            lstring strret;
            int outlen = 0;
            for (int i = 0; i < data_len; i += AES_BLOCK_SIZE)
            {
                ::aes_encrypt(data_buf + i, enc_buf, key_schedule, key_bits);
                strret.append((_lpcstr)enc_buf, AES_BLOCK_SIZE);
                outlen += AES_BLOCK_SIZE;
            }

            delete[] data_buf;
            if (out_len) *out_len = outlen;
            return strret;
        }

        LSLIB_API lstring aes_decode(_lpcstr data, int data_len, _lpcstr key, aes_key_bits key_bits, aes_padding_mode mode, __out int* out_len)
        {
            if (data_len % AES_BLOCK_SIZE != 0) return ""; // decrypt data size must be times of AES_BLOCK_SIZE

            uint32_t key_schedule[60] = {0};
            uint8_t enc_buf[AES_BLOCK_SIZE] = {0};

            aes_key_setup((uint8_t*)key, key_schedule, key_bits);

            lstring strret;
            int outlen = 0;
            for (int i = 0; i < data_len; i += AES_BLOCK_SIZE)
            {
                ::aes_decrypt((uint8_t*)(data + i), enc_buf, key_schedule, key_bits);
                strret.append((_lpcstr)enc_buf, AES_BLOCK_SIZE);
                outlen += AES_BLOCK_SIZE;
            }

            switch (mode)
            {
            case aes_zeropadding:
                break;
            case aes_pkcs5padding:
            case aes_pkcs7padding:
                if (outlen > 0) outlen -= strret.at(outlen - 1);
                break;
            }

            strret.resize(outlen);
            if (out_len) *out_len = outlen;
            return strret;
        }

        LSLIB_API lstring aes_encode_cbc(_lpcstr data, int data_len, _lpcstr key, aes_key_bits key_bits, aes_padding_mode mode, _lchar iv[16], __out int* out_len)
        {
            uint32_t key_schedule[60] = {0};
            aes_key_setup((uint8_t*)key, key_schedule, key_bits);

            int padding_len = AES_BLOCK_SIZE - data_len % AES_BLOCK_SIZE;
            uint8_t* data_buf = new uint8_t[data_len + padding_len];
            memcpy(data_buf, data, data_len);

            switch (mode)
            {
            case aes_zeropadding:
                memset(data_buf + data_len, 0, padding_len);
                break;
            case aes_pkcs5padding:
            case aes_pkcs7padding:
                memset(data_buf + data_len, padding_len, padding_len);
                break;
            }

            ::aes_encrypt_cbc(data_buf, data_len + padding_len, data_buf, key_schedule, key_bits, (uint8_t*)iv);

            lstring strret;
            strret.assign((_lpcstr)data_buf, data_len + padding_len);
            if (out_len) *out_len = data_len + padding_len;
            delete[] data_buf;
            return strret;
        }

        LSLIB_API lstring aes_decode_cbc(_lpcstr data, int data_len, _lpcstr key, aes_key_bits key_bits, aes_padding_mode mode, _lchar iv[16], __out int* out_len)
        {
            if (data_len % AES_BLOCK_SIZE != 0) return ""; // decrypt data size must be times of AES_BLOCK_SIZE

            uint32_t key_schedule[60] = {0};
            aes_key_setup((uint8_t*)key, key_schedule, key_bits);

            uint8_t* data_buf = new uint8_t[data_len];
            memcpy(data_buf, data, data_len);

            ::aes_decrypt_cbc(data_buf, data_len, data_buf, key_schedule, key_bits, (uint8_t*)iv);

            int outlen = data_len;
            switch (mode)
            {
            case aes_zeropadding:
                break;
            case aes_pkcs5padding:
            case aes_pkcs7padding:
                outlen -= data_buf[outlen - 1];
                break;
            }

            lstring strret;
            strret.assign((_lpcstr)data_buf, outlen);
            if (out_len) *out_len = outlen;
            delete[] data_buf;
            return strret;
        }

        LSLIB_API lstring url_encode(_lpcstr data, int len)
        {
            int out_len = ::url_encode(data, len, NULL, 0);
            char* pbuf = new char[out_len + 1];
            memset(pbuf, 0, out_len + 1);
            ::url_encode(data, len, pbuf, out_len);
            lstring str; str.assign(pbuf, out_len);
            delete[] pbuf;
            return str;
        }

        LSLIB_API lstring url_decode(_lpcstr data, int len)
        {
            char* pbuf = new char[len + 1];
            memset(pbuf, 0, len + 1);
            memcpy(pbuf, data, len);
            int out_len = ::url_decode(pbuf, len);
            lstring str; str.assign(pbuf, out_len);
            delete[] pbuf;
            return str;
        }

    } // crypto

} // lslib
