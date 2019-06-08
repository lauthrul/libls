#pragma once

namespace lslib
{
    namespace crypto
    {
        LSLIB_API lstring md5(_lpcstr data, int len);
        LSLIB_API lstring file_md5(_lpcstr pfile);

        LSLIB_API lstring sha1(_lpcstr data, int len);
        LSLIB_API lstring file_sha1(_lpcstr pfile);

        LSLIB_API lstring sha224(_lpcstr data, int len);
        LSLIB_API lstring file_sha224(_lpcstr pfile);

        LSLIB_API lstring sha256(_lpcstr data, int len);
        LSLIB_API lstring file_sha256(_lpcstr pfile);

        LSLIB_API lstring sha384(_lpcstr data, int len);
        LSLIB_API lstring file_sha384(_lpcstr pfile);

        LSLIB_API lstring sha512(_lpcstr data, int len);
        LSLIB_API lstring file_sha512(_lpcstr pfile);

        LSLIB_API lstring base64_encode(_lpcstr data, int len);
        LSLIB_API lstring base64_decode(_lpcstr data, int len, __out int* out_len);

        enum crypto_key_bits { crypto_bit128 = 128, crypto_bit192 = 192, crypto_bit256 = 256 };

        enum crypto_padding_mode
        {
            crypto_nopadding,          // do not padding data, only can be use when data length is a multiple of 16 bytes, otherwise the result will be uncertain.
            crypto_zeropadding,        // padding data with 0, only can be use when encrypt/decrypt a string.
            crypto_pkcs5padding,       // padding data with 0-8 bytes int block size 8, the same as crypto_pkcs7padding in block size 16.
            crypto_pkcs7padding        // padding data with 0-16 bytes int block size 16.
        };

        // return data is byte array (stored in lstring), check the length (out_len) before use.
        LSLIB_API lstring aes_encode(_lpcstr data,                 // data(string or byte array) to be encrypt
                                     int data_len,                 // data length in bytes
                                     _lpcstr key,                  // the key must be length of 128, 192 or 256 bits (16, 24, 32 bytes)
                                     crypto_key_bits key_bits,        // key length in bit
                                     crypto_padding_mode mode,     // padding mode. only data_len is multiple of 16, crypto_nopadding can be set, otherwise the result will be uncertain.
                                     __out int* out_len);          // result length in bytes

        // return data is either string or byte array (stored in lstring), check the length (out_len) before use.
        LSLIB_API lstring aes_decode(_lpcstr data,                 // data(string or byte array) to be decrypt, data length must be multiple of 16.
                                     int data_len,                 // data length in bytes, must be multiple of 16.
                                     _lpcstr key,                  // the key must be length of 128, 192 or 256 bits (16, 24, 32 bytes)
                                     crypto_key_bits key_bits,        // key length in bit
                                     crypto_padding_mode mode,     // padding mode. only data_len is multiple of 16, crypto_nopadding can be set, otherwise the result will be uncertain.
                                     __out int* out_len);          // result length in bytes

        // return data is either string or byte array (stored in lstring), check the length (out_len) before use.
        LSLIB_API lstring aes_encode_cbc(_lpcstr data,             // data(string or byte array) to be encrypt
                                         int data_len,             // data length in bytes
                                         _lpcstr key,              // the key must be length of 128, 192 or 256 bits (16, 24, 32 bytes)
                                         crypto_key_bits key_bits,    // key length in bit
                                         crypto_padding_mode mode, // padding mode. only data_len is multiple of 16, crypto_nopadding can be set, otherwise the result will be uncertain.
                                         _lchar iv[16],            // init vector. must be length of 16 bytes
                                         __out int* out_len);      // result length in bytes

        // return data is either string or byte array (stored in lstring), check the length (out_len) before use.
        LSLIB_API lstring aes_decode_cbc(_lpcstr data,             // data(string or byte array) to be decrypt, data length must be multiple of 16.
                                         int data_len,             // data length in bytes, must be multiple of 16.
                                         _lpcstr key,              // the key must be length of 128, 192 or 256 bits (16, 24, 32 bytes)
                                         crypto_key_bits key_bits,    // key length in bit
                                         crypto_padding_mode mode, // padding mode. only data_len is multiple of 16, crypto_nopadding can be set, otherwise the result will be uncertain.
                                         _lchar iv[16],            // init vector. must be length of 16 bytes
                                         __out int* out_len);      // result length in bytes

        LSLIB_API lstring url_encode(_lpcstr data, int len);
        LSLIB_API lstring url_decode(_lpcstr data, int len);
    } // crypto
}