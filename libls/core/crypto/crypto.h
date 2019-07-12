#pragma once

namespace lslib
{
    namespace crypto
    {
        ///////////////////
        // md5
        ///////////////////

        LSLIB_API lstring md5(_lpbyte data, size_t len);    // 返回十六进制md5值字符串
        LSLIB_API lstring file_md5(_lpcstr pfile);          // 返回十六进制md5值字符串

        ///////////////////
        // sha1
        ///////////////////

        LSLIB_API lstring sha1(_lpbyte data, size_t len);   // 返回十六进制sha1值字符串
        LSLIB_API lstring file_sha1(_lpcstr pfile);         // 返回十六进制sha1值字符串

        ///////////////////
        // sha224
        ///////////////////

        LSLIB_API lstring sha224(_lpbyte data, size_t len); // 返回十六进制sha224值字符串
        LSLIB_API lstring file_sha224(_lpcstr pfile);       // 返回十六进制sha224值字符串

        ///////////////////
        // sha256
        ///////////////////

        LSLIB_API lstring sha256(_lpbyte data, size_t len); // 返回十六进制sha256值字符串
        LSLIB_API lstring file_sha256(_lpcstr pfile);       // 返回十六进制sha256值字符串

        ///////////////////
        // sha384
        ///////////////////

        LSLIB_API lstring sha384(_lpbyte data, size_t len); // 返回十六进制sha384值字符串
        LSLIB_API lstring file_sha384(_lpcstr pfile);       // 返回十六进制sha384值字符串

        ///////////////////
        // sha512
        ///////////////////

        LSLIB_API lstring sha512(_lpbyte data, size_t len); // 返回十六进制sha512值字符串
        LSLIB_API lstring file_sha512(_lpcstr pfile);       // 返回十六进制sha512值字符串

        ///////////////////
        // base64
        ///////////////////

        LSLIB_API lstring base64_encode(_lpbyte data, size_t len); // 返回base64编码后的字符串
        LSLIB_API _lpbyte base64_decode(_lpcstr data, size_t len, __out__ int* out_len); // 返回base64解码后的数据指针，需要调用free()释放。

        enum crypto_key_bits { crypto_bit128 = 128, crypto_bit192 = 192, crypto_bit256 = 256 };

        enum crypto_padding_mode
        {
            crypto_nopadding,          // do not padding data, only can be use when data length is a multiple of 16 bytes, otherwise the result will be uncertain.
            crypto_zeropadding,        // padding data with 0, only can be use when encrypt/decrypt a string.
            crypto_pkcs5padding,       // padding data with 0-8 bytes int block size 8, the same as crypto_pkcs7padding in block size 16.
            crypto_pkcs7padding        // padding data with 0-16 bytes int block size 16.
        };

        ///////////////////
        // DES
        ///////////////////

        // return data is a string or byte array (stored in lstring), check the length (out_len) before use.
        LSLIB_API lstring des_encrypt(_lpcstr data,                 // data(string or byte array) to be encrypt
                                      int data_len,                 // data length in bytes
                                      _lpcstr key,                  // the key must be length of 64 bits (8 bytes)
                                      crypto_padding_mode mode,     // padding mode. only data_len is multiple of 8, crypto_nopadding can be set, otherwise the result will be uncertain.
                                      __out__ int* out_len);          // result length in bytes

        // return data is a string or byte array (stored in lstring), check the length (out_len) before use.
        LSLIB_API lstring des_decrypt(_lpcstr data,                 // data(string or byte array) to be decrypt, data length must be multiple of 8.
                                      int data_len,                 // data length in bytes
                                      _lpcstr key,                  // the key must be length of 64 bits (8 bytes)
                                      crypto_padding_mode mode,     // padding mode. only data_len is multiple of 8, crypto_nopadding can be set, otherwise the result will be uncertain.
                                      __out__ int* out_len);          // result length in bytes

        // return data is a string or byte array (stored in lstring), check the length (out_len) before use.
        LSLIB_API lstring des_encrypt_cbc(_lpcstr data,             // data(string or byte array) to be encrypt
                                          int data_len,             // data length in bytes
                                          _lpcstr key,              // the key must be length of 64 bits (8 bytes)
                                          crypto_padding_mode mode, // padding mode. only data_len is multiple of 8, crypto_nopadding can be set, otherwise the result will be uncertain.
                                          _lchar iv[8],             // init vector. must be length of 8 bytes
                                          __out__ int* out_len);      // result length in bytes

        // return data is a string or byte array (stored in lstring), check the length (out_len) before use.
        LSLIB_API lstring des_decrypt_cbc(_lpcstr data,             // data(string or byte array) to be decrypt, data length must be multiple of 8.
                                          int data_len,             // data length in bytes, must be multiple of 8.
                                          _lpcstr key,              // the key must be length of 64 bits (8 bytes)
                                          crypto_padding_mode mode, // padding mode. only data_len is multiple of 8, crypto_nopadding can be set, otherwise the result will be uncertain.
                                          _lchar iv[8],             // init vector. must be length of 8 bytes
                                          __out__ int* out_len);      // result length in bytes


        // return data is a string or byte array (stored in lstring), check the length (out_len) before use.
        LSLIB_API lstring three_des_encrypt(_lpcstr data,                 // data(string or byte array) to be encrypt
                                            int data_len,                 // data length in bytes
                                            _lpcstr key,                  // the key must be length of 192 bits (24 bytes)
                                            crypto_padding_mode mode,     // padding mode. only data_len is multiple of 8, crypto_nopadding can be set, otherwise the result will be uncertain.
                                            __out__ int* out_len);          // result length in bytes

        // return data is a string or byte array (stored in lstring), check the length (out_len) before use.
        LSLIB_API lstring three_des_decrypt(_lpcstr data,                 // data(string or byte array) to be decrypt, data length must be multiple of 8.
                                            int data_len,                 // data length in bytes
                                            _lpcstr key,                  // the key must be length of 192 bits (24 bytes)
                                            crypto_padding_mode mode,     // padding mode. only data_len is multiple of 8, crypto_nopadding can be set, otherwise the result will be uncertain.
                                            __out__ int* out_len);          // result length in bytes

        // return data is a string or byte array (stored in lstring), check the length (out_len) before use.
        LSLIB_API lstring three_des_encrypt_cbc(_lpcstr data,             // data(string or byte array) to be encrypt
                                                int data_len,             // data length in bytes
                                                _lpcstr key,              // the key must be length of 64 bits (8 bytes)
                                                crypto_padding_mode mode, // padding mode. only data_len is multiple of 8, crypto_nopadding can be set, otherwise the result will be uncertain.
                                                _lchar iv[8],             // init vector. must be length of 8 bytes
                                                __out__ int* out_len);      // result length in bytes

        // return data is a string or byte array (stored in lstring), check the length (out_len) before use.
        LSLIB_API lstring three_des_decrypt_cbc(_lpcstr data,             // data(string or byte array) to be decrypt, data length must be multiple of 8.
                                                int data_len,             // data length in bytes, must be multiple of 8.
                                                _lpcstr key,              // the key must be length of 64 bits (8 bytes)
                                                crypto_padding_mode mode, // padding mode. only data_len is multiple of 8, crypto_nopadding can be set, otherwise the result will be uncertain.
                                                _lchar iv[8],             // init vector. must be length of 8 bytes
                                                __out__ int* out_len);      // result length in bytes


        ///////////////////
        // AES
        ///////////////////

        // return data is a string or byte array (stored in lstring), check the length (out_len) before use.
        LSLIB_API lstring aes_encrypt(_lpcstr data,                 // data(string or byte array) to be encrypt
                                      int data_len,                 // data length in bytes
                                      _lpcstr key,                  // the key must be length of 128, 192 or 256 bits (16, 24, 32 bytes)
                                      crypto_key_bits key_bits,        // key length in bit
                                      crypto_padding_mode mode,     // padding mode. only data_len is multiple of 16, crypto_nopadding can be set, otherwise the result will be uncertain.
                                      __out__ int* out_len);          // result length in bytes

        // return data is a string or byte array (stored in lstring), check the length (out_len) before use.
        LSLIB_API lstring aes_decrypt(_lpcstr data,                 // data(string or byte array) to be decrypt, data length must be multiple of 16.
                                      int data_len,                 // data length in bytes, must be multiple of 16.
                                      _lpcstr key,                  // the key must be length of 128, 192 or 256 bits (16, 24, 32 bytes)
                                      crypto_key_bits key_bits,        // key length in bit
                                      crypto_padding_mode mode,     // padding mode. only data_len is multiple of 16, crypto_nopadding can be set, otherwise the result will be uncertain.
                                      __out__ int* out_len);          // result length in bytes

        // return data is a string or byte array (stored in lstring), check the length (out_len) before use.
        LSLIB_API lstring aes_encrypt_cbc(_lpcstr data,             // data(string or byte array) to be encrypt
                                          int data_len,             // data length in bytes
                                          _lpcstr key,              // the key must be length of 128, 192 or 256 bits (16, 24, 32 bytes)
                                          crypto_key_bits key_bits,    // key length in bit
                                          crypto_padding_mode mode, // padding mode. only data_len is multiple of 16, crypto_nopadding can be set, otherwise the result will be uncertain.
                                          _lchar iv[16],            // init vector. must be length of 16 bytes
                                          __out__ int* out_len);      // result length in bytes

        // return data is a string or byte array (stored in lstring), check the length (out_len) before use.
        LSLIB_API lstring aes_decrypt_cbc(_lpcstr data,             // data(string or byte array) to be decrypt, data length must be multiple of 16.
                                          int data_len,             // data length in bytes, must be multiple of 16.
                                          _lpcstr key,              // the key must be length of 128, 192 or 256 bits (16, 24, 32 bytes)
                                          crypto_key_bits key_bits,    // key length in bit
                                          crypto_padding_mode mode, // padding mode. only data_len is multiple of 16, crypto_nopadding can be set, otherwise the result will be uncertain.
                                          _lchar iv[16],            // init vector. must be length of 16 bytes
                                          __out__ int* out_len);      // result length in bytes

        ///////////////////
        // url encoding
        ///////////////////

        LSLIB_API lstring url_encode(_lpcstr data, int len);
        LSLIB_API lstring url_decode(_lpcstr data, int len);

        ///////////////////
        // encoding convert
        ///////////////////

#ifdef USE_LIBICONV
        // convert string between ut8/gb2312...
        LSLIB_API int encoding_convert(_lpcstr from_charset, _lpcstr to_charset, _lpcstr inbuf, unsigned int inlen, __inout__ _lpstr outbuf, __inout__ unsigned int outlen);
        LSLIB_API lstring encoding_convert(_lpcstr data, _lpcstr from_charset, _lpcstr to_charset);
#endif

    } // crypto
}
