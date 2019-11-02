#include "stdafx.h"
#include "crypto.h"
#include "md5.h"
#include "sha1.h"
#include "sha224.h"
#include "sha256.h"
#include "sha384.h"
#include "sha512.h"
#include "base64.h"
#include "aes.h"
#include "url_encode.h"
#include <math.h>
#include "des.h"
#ifdef USE_LIBICONV
#include <iconv.h>
#endif

namespace lslib
{
    namespace crypto
    {
        LSLIB_API string md5(lpbyte data, size_t len)
        {
            MD5_CTX ctx;
            MD5Init(&ctx);
            MD5Update(&ctx, data, len);
            MD5Final(&ctx);
            return strtool::byte_array_to_hex_str(ctx.digest, 16);
        }

        LSLIB_API string sha1(lpbyte data, size_t len)
        {
            SHA1_CTX ctx;
            SHA1Init(&ctx);
            SHA1Update(&ctx, data, len);
            SHA1Final(&ctx);
            return strtool::byte_array_to_hex_str(ctx.digest, 20);
        }

        LSLIB_API string sha224(lpbyte data, size_t len)
        {
            uint8_t hash[32] = { 0 };
            SHA224_Simple(data, len, hash);
            return strtool::byte_array_to_hex_str(hash, 28);
        }

        LSLIB_API string sha256(lpbyte data, size_t len)
        {
            uint8_t hash[32] = {0};
            SHA256_Simple(data, len, hash);
            return strtool::byte_array_to_hex_str(hash, 32);
        }

        LSLIB_API string sha384(lpbyte data, size_t len)
        {
            uint8_t hash[64] = { 0 };
            SHA384_Simple(data, len, hash);
            return strtool::byte_array_to_hex_str(hash, 48);
        }

        LSLIB_API string sha512(lpbyte data, size_t len)
        {
            uint8_t hash[64] = { 0 };
            SHA512_Simple(data, len, hash);
            return strtool::byte_array_to_hex_str(hash, 64);
        }

        LSLIB_API string base64_encode(lpbyte data, size_t len)
        {
            int data_len = estimate_base64_encode_len(len);
            lpbyte pbuf = lsalloc(data_len + 1);
            data_len = ::base64_encode(data, len, (char*)pbuf);

            string strret;
            strret.assign((lpcstr)pbuf, data_len);
            lsfree(pbuf);
            return strret;
        }

        LSLIB_API string base64_decode(lpcstr data, __out__ int* out_len)
        {
            if (strtool::is_empty(data)) return "";

            size_t len = strlen(data);
            int data_len = estimate_base64_decode_len(len);
            lpbyte pbuf = lsalloc(data_len + 1);
            data_len = ::base64_decode(data, len, pbuf);

            string strret;
            strret.assign((lpcstr)pbuf, data_len);
            lsfree(pbuf);
            if (out_len) *out_len = data_len;
            return strret;
        }

        LSLIB_API lpbyte crypto_padding(__inout__ lpbyte& data_buf, __inout__ size_t& data_len, size_t block_size, crypto_padding_mode mode)
        {
            if (data_buf != NULL && data_len > 0)
            {
                int padding_len = block_size - data_len % block_size;
                data_buf = (lpbyte)realloc(data_buf, data_len + padding_len);
                switch (mode)
                {
                    case crypto_zeropadding:
                        memset(data_buf + data_len, 0, padding_len);
                        break;
                    case crypto_pkcs5padding:
                    case crypto_pkcs7padding:
                        memset(data_buf + data_len, padding_len, padding_len);
                        break;
                }
                data_len += padding_len;
            }
            return data_buf;
        }

        LSLIB_API lpbyte crypto_unpadding(__inout__ lpbyte& data_buf, __inout__ size_t& data_len, crypto_padding_mode mode)
        {
            if (data_buf != NULL && data_len > 0)
            {
                switch (mode)
                {
                    case crypto_zeropadding:
                        while (data_buf[--data_len] == 0 && data_len >= 0);
                        ++data_len;
                        break;
                    case crypto_pkcs5padding:
                    case crypto_pkcs7padding:
                        data_len -= data_buf[data_len - 1];
                        break;
                }
                data_buf = (lpbyte)realloc(data_buf, data_len);
            }
            return data_buf;
        }

        LSLIB_API string des_encrypt(lpbyte data, size_t data_len, const lchar key[8], crypto_padding_mode mode, __out__ int* out_len)
        {
            uint8_t key_schedule[16][6] = {0};
            des_key_setup((uint8_t*)key, key_schedule, DES_ENCRYPT);

            lpbyte data_buf = (lpbyte)lsalloc(data_len);
            memcpy(data_buf, data, data_len);
            crypto_padding(data_buf, data_len, DES_BLOCK_SIZE, mode);

            for (int i = 0; i < data_len; i += DES_BLOCK_SIZE)
                ::des_crypt(data_buf + i, data_buf + i, key_schedule);

            string strret;
            strret.assign((lpcstr)data_buf, data_len);
            lsfree(data_buf);
            if (out_len) *out_len = data_len;
            return strret;
        }

        LSLIB_API string des_decrypt(lpbyte data, size_t data_len, const lchar key[8], crypto_padding_mode mode, __out__ int* out_len)
        {
            if (data_len % DES_BLOCK_SIZE != 0) return ""; // decrypt data size must be times of AES_BLOCK_SIZE

            uint8_t key_schedule[16][6] = {0};
            des_key_setup((uint8_t*)key, key_schedule, DES_DECRYPT);

            lpbyte data_buf = lsalloc(data_len);
            memcpy(data_buf, data, data_len);

            for (int i = 0; i < data_len; i += DES_BLOCK_SIZE)
                ::des_crypt(data_buf + i, data_buf + i, key_schedule);

            crypto_unpadding(data_buf, data_len, mode);

            string strret;
            strret.assign((lpcstr)data_buf, data_len);
            lsfree(data_buf);
            if (out_len) *out_len = data_len;
            return strret;
        }

        LSLIB_API string des_encrypt_cbc(lpbyte data, size_t data_len, const lchar key[8], crypto_padding_mode mode, const lchar iv[8], __out__ int* out_len)
        {
            uint8_t key_schedule[16][6] = {0};
            des_key_setup((uint8_t*)key, key_schedule, DES_ENCRYPT);

            lpbyte data_buf = lsalloc(data_len);
            memcpy(data_buf, data, data_len);
            crypto_padding(data_buf, data_len, DES_BLOCK_SIZE, mode);

            ::des_encrypt_cbc(data_buf, data_len, data_buf, key_schedule, (uint8_t*)iv);

            string strret;
            strret.assign((lpcstr)data_buf, data_len);
            lsfree(data_buf);
            if (out_len) *out_len = data_len;
            return strret;
        }

        LSLIB_API string des_decrypt_cbc(lpbyte data, size_t data_len, const lchar key[8], crypto_padding_mode mode, const lchar iv[8], __out__ int* out_len)
        {
            if (data_len % DES_BLOCK_SIZE != 0) return ""; // decrypt data size must be times of AES_BLOCK_SIZE

            uint8_t key_schedule[16][6] = {0};
            des_key_setup((uint8_t*)key, key_schedule, DES_DECRYPT);

            lpbyte data_buf = lsalloc(data_len);
            memcpy(data_buf, data, data_len);

            ::des_decrypt_cbc(data_buf, data_len, data_buf, key_schedule, (uint8_t*)iv);

            crypto_unpadding(data_buf, data_len, mode);

            string strret;
            strret.assign((lpcstr)data_buf, data_len);
            lsfree(data_buf);
            if (out_len) *out_len = data_len;
            return strret;
        }

        LSLIB_API string three_des_encrypt(lpbyte data, size_t data_len, const lchar key[24], crypto_padding_mode mode, __out__ int* out_len)
        {
            uint8_t key_schedule[3][16][6] = {0};
            three_des_key_setup((uint8_t*)key, key_schedule, DES_ENCRYPT);

            lpbyte data_buf = lsalloc(data_len);
            memcpy(data_buf, data, data_len);
            crypto_padding(data_buf, data_len, DES_BLOCK_SIZE, mode);

            for (int i = 0; i < data_len; i += DES_BLOCK_SIZE)
                ::three_des_crypt(data_buf + i, data_buf + i, key_schedule);

            string strret;
            strret.assign((lpcstr)data_buf, data_len);
            lsfree(data_buf);
            if (out_len) *out_len = data_len;
            return strret;
        }

        LSLIB_API string three_des_decrypt(lpbyte data, size_t data_len, const lchar key[24], crypto_padding_mode mode, __out__ int* out_len)
        {
            if (data_len % DES_BLOCK_SIZE != 0) return ""; // decrypt data size must be times of AES_BLOCK_SIZE

            uint8_t key_schedule[3][16][6] = {0};
            three_des_key_setup((uint8_t*)key, key_schedule, DES_DECRYPT);

            lpbyte data_buf = lsalloc(data_len);
            memcpy(data_buf, data, data_len);

            for (int i = 0; i < data_len; i += DES_BLOCK_SIZE)
                ::three_des_crypt(data_buf + i, data_buf + i, key_schedule);

            crypto_unpadding(data_buf, data_len, mode);

            string strret;
            strret.assign((lpcstr)data_buf, data_len);
            lsfree(data_buf);
            if (out_len) *out_len = data_len;
            return strret;
        }

        LSLIB_API string three_des_encrypt_cbc(lpbyte data, size_t data_len, const lchar key[24], crypto_padding_mode mode, const lchar iv[8], __out__ int* out_len)
        {
            uint8_t key_schedule[3][16][6] = {0};
            three_des_key_setup((uint8_t*)key, key_schedule, DES_ENCRYPT);

            lpbyte data_buf = lsalloc(data_len);
            memcpy(data_buf, data, data_len);
            crypto_padding(data_buf, data_len, DES_BLOCK_SIZE, mode);

            ::three_des_encrypt_cbc(data_buf, data_len, data_buf, key_schedule, (uint8_t*)iv);

            string strret;
            strret.assign((lpcstr)data_buf, data_len);
            lsfree(data_buf);
            if (out_len) *out_len = data_len;
            return strret;
        }

        LSLIB_API string three_des_decrypt_cbc(lpbyte data, size_t data_len, const lchar key[24], crypto_padding_mode mode, const lchar iv[8], __out__ int* out_len)
        {
            if (data_len % DES_BLOCK_SIZE != 0) return ""; // decrypt data size must be times of AES_BLOCK_SIZE

            uint8_t key_schedule[3][16][6] = {0};
            three_des_key_setup((uint8_t*)key, key_schedule, DES_DECRYPT);

            lpbyte data_buf = lsalloc(data_len);
            memcpy(data_buf, data, data_len);

            ::three_des_decrypt_cbc(data_buf, data_len, data_buf, key_schedule, (uint8_t*)iv);

            crypto_unpadding(data_buf, data_len, mode);

            string strret;
            strret.assign((lpcstr)data_buf, data_len);
            lsfree(data_buf);
            if (out_len) *out_len = data_len;
            return strret;
        }


        LSLIB_API string aes_encrypt(lpbyte data, size_t data_len, lpcstr key, crypto_key_bits key_bits, crypto_padding_mode mode, __out__ int* out_len)
        {
            uint32_t key_schedule[60] = {0};
            aes_key_setup((uint8_t*)key, key_schedule, key_bits);

            lpbyte data_buf = lsalloc(data_len);
            memcpy(data_buf, data, data_len);
            crypto_padding(data_buf, data_len, AES_BLOCK_SIZE, mode);

            for (int i = 0; i < data_len; i += AES_BLOCK_SIZE)
                ::aes_encrypt(data_buf + i, data_buf + i, key_schedule, key_bits);

            string strret;
            strret.assign((lpcstr)data_buf, data_len);
            lsfree(data_buf);
            if (out_len) *out_len = data_len;
            return strret;
        }

        LSLIB_API string aes_decrypt(lpbyte data, size_t data_len, lpcstr key, crypto_key_bits key_bits, crypto_padding_mode mode, __out__ int* out_len)
        {
            if (data_len % AES_BLOCK_SIZE != 0) return ""; // decrypt data size must be times of AES_BLOCK_SIZE

            uint32_t key_schedule[60] = {0};
            aes_key_setup((uint8_t*)key, key_schedule, key_bits);

            lpbyte data_buf = lsalloc(data_len);
            memcpy(data_buf, data, data_len);

            for (int i = 0; i < data_len; i += AES_BLOCK_SIZE)
                ::aes_decrypt(data_buf + i, data_buf + i, key_schedule, key_bits);

            crypto_unpadding(data_buf, data_len, mode);

            string strret;
            strret.assign((lpcstr)data_buf, data_len);
            lsfree(data_buf);
            if (out_len) *out_len = data_len;
            return strret;
        }

        LSLIB_API string aes_encrypt_cbc(lpbyte data, size_t data_len, lpcstr key, crypto_key_bits key_bits, crypto_padding_mode mode, const lchar iv[16], __out__ int* out_len)
        {
            uint32_t key_schedule[60] = {0};
            aes_key_setup((uint8_t*)key, key_schedule, key_bits);

            lpbyte data_buf = lsalloc(data_len);
            memcpy(data_buf, data, data_len);
            crypto_padding(data_buf, data_len, AES_BLOCK_SIZE, mode);

            ::aes_encrypt_cbc(data_buf, data_len, data_buf, key_schedule, key_bits, (uint8_t*)iv);

            string strret;
            strret.assign((lpcstr)data_buf, data_len);
            lsfree(data_buf);
            if (out_len) *out_len = data_len;
            return strret;
        }

        LSLIB_API string aes_decrypt_cbc(lpbyte data, size_t data_len, lpcstr key, crypto_key_bits key_bits, crypto_padding_mode mode, const lchar iv[16], __out__ int* out_len)
        {
            if (data_len % AES_BLOCK_SIZE != 0) return ""; // decrypt data size must be times of AES_BLOCK_SIZE

            uint32_t key_schedule[60] = {0};
            aes_key_setup((uint8_t*)key, key_schedule, key_bits);

            lpbyte data_buf = lsalloc(data_len);
            memcpy(data_buf, data, data_len);

            ::aes_decrypt_cbc(data_buf, data_len, data_buf, key_schedule, key_bits, (uint8_t*)iv);

            crypto_unpadding(data_buf, data_len, mode);

            string strret;
            strret.assign((lpcstr)data_buf, data_len);
            lsfree(data_buf);
            if (out_len) *out_len = data_len;
            return strret;
        }

        LSLIB_API string url_encode(lpbyte data, size_t len)
        {
            int data_len = ::url_encode((lpcstr)data, len, NULL, 0);
            lpbyte pbuf = lsalloc(data_len + 1);
            ::url_encode((lpcstr)data, len, (lpstr)pbuf, data_len);
            string str; str.assign((lpcstr)pbuf, data_len);
            lsfree(pbuf);
            return str;
        }

        LSLIB_API string url_decode(lpcstr data, __out__ int* out_len)
        {
            if (strtool::is_empty(data)) return "";

            size_t len = strlen(data);
            lpbyte pbuf = lsalloc(len + 1);
            memcpy(pbuf, data, len);
            int data_len = ::url_decode((lpstr)pbuf, len);
            string str; str.assign((lpcstr)pbuf, data_len);
            lsfree(pbuf);
            if (out_len) *out_len = data_len;
            return str;
        }

#ifdef USE_LIBICONV
        LSLIB_API int encoding_convert(lpcstr from_charset, lpcstr to_charset, lpcstr inbuf, size_t inlen, __inout__ lpstr outbuf, __inout__ size_t& outlen)
        {
            iconv_t cd;
            const char** pin = &inbuf;
            char** pout = &outbuf;
            cd = iconv_open(to_charset, from_charset);
            if ((int)cd <= 0) return -1;
            memset(outbuf, 0, outlen);
            int ret = iconv(cd, pin, &inlen, pout, &outlen);
            iconv_close(cd);
            return ret;
        }

        LSLIB_API string encoding_convert(lpbyte data, size_t len, lpcstr from_charset, lpcstr to_charset, __out__ int* out_len)
        {
            size_t buff_size = 4 * len;
            lpbyte buf = lsalloc(buff_size);
            int ret = encoding_convert(from_charset, to_charset, (lpcstr)data, len, (lpstr)buf, buff_size);
            string strret; strret = (lpcstr)buf;
            if (out_len) *out_len = strret.length();
            lsfree(buf);
            return strret;
        }

        LSLIB_API string encoding_convert(lpcstr data, lpcstr from_charset, lpcstr to_charset)
        {
            if (strtool::is_empty(data)) return "";
            return encoding_convert((lpbyte)data, strlen(data), from_charset, to_charset, NULL);
        }

#endif

    } // crypto

} // lslib
