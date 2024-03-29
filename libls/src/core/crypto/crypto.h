#pragma once

namespace lslib
{
    /// @brief 加解密相关
    namespace crypto
    {
#define DEFAULT_CRYPT_KEY       "l53d70p3"
#define DEFAULT_CRYPT_IV        "12345678"

        // -----------------------------------------------------------------------
        // md5
        // -----------------------------------------------------------------------

        /// @brief 计算数据md5值
        /// @param data 需要进行md5计算的数据内容
        /// @param len  数据长度
        /// @return 返回十六进制md5值字符串
        LSLIB_API string md5(lpbyte data, size_t len);

        // -----------------------------------------------------------------------
        // sha1
        // -----------------------------------------------------------------------

        /// @brief 计算数据sha1值
        /// @param data 需要进行sha1计算的数据内容
        /// @param len  数据长度
        /// @return 返回十六进制sha1值字符串
        LSLIB_API string sha1(lpbyte data, size_t len);

        // -----------------------------------------------------------------------
        // sha224
        // -----------------------------------------------------------------------

        /// @brief 计算数据sha224值
        /// @param data 需要进行sha224计算的数据内容
        /// @param len  数据长度
        /// @return 返回十六进制sha224值字符串
        LSLIB_API string sha224(lpbyte data, size_t len);


        // -----------------------------------------------------------------------
        // sha256
        // -----------------------------------------------------------------------

        /// @brief 计算数据sha256值
        /// @param data 需要进行sha256计算的数据内容
        /// @param len  数据长度
        /// @return 返回十六进制sha256值字符串
        LSLIB_API string sha256(lpbyte data, size_t len);

        // -----------------------------------------------------------------------
        // sha384
        // -----------------------------------------------------------------------

        /// @brief 计算数据sha384值
        /// @param data 需要进行sha384计算的数据内容
        /// @param len  数据长度
        /// @return 返回十六进制sha384值字符串
        LSLIB_API string sha384(lpbyte data, size_t len);

        // -----------------------------------------------------------------------
        // sha512
        // -----------------------------------------------------------------------

        /// @brief 计算数据sha512值
        /// @param data 需要进行sha512计算的数据内容
        /// @param len  数据长度
        /// @return 返回十六进制sha512值字符串
        LSLIB_API string sha512(lpbyte data, size_t len);

        // -----------------------------------------------------------------------
        // base64
        // -----------------------------------------------------------------------

        /// @brief 对数据进行base64编码
        /// @param data 需要进行base64编码的数据内容
        /// @param len  数据长度
        /// @return 返回base64编码后的字符串
        LSLIB_API string base64_encode(lpbyte data, size_t len);

        /// @brief 对数据进行base64解码
        /// @param[in] data     需要进行base64解码的字符串
        /// @param[out] out_len 输出解码后的数据内容长度，为NULL时不输出长度
        /// @return 返回base64解码后的数据内容，以string作为保存容器，非字符串数据内容请配合输出长度out_len使用
        LSLIB_API string base64_decode(lpcstr data, __out__ int* out_len);

        // -----------------------------------------------------------------------
        // common
        // -----------------------------------------------------------------------

        /// 加密密钥长度枚举
        enum crypto_key_bits
        {
            crypto_bit128 = 128,    ///< 128位（16字节）密钥长度
            crypto_bit192 = 192,    ///< 192位（24字节）密钥长度
            crypto_bit256 = 256     ///< 256位（32字节）密钥长度
        };

        /// 加密填充模式枚举
        enum crypto_padding_mode
        {
            crypto_nopadding,          ///< 不填充。仅当块大小为8字节的倍数时使用，否则结果未确定
            crypto_zeropadding,        ///< 填充0。仅当加/解密数据为字符串时使用
            crypto_pkcs5padding,       ///< 填充0-8个字节。块大小为8个字节时使用。当块大小为16个字节时，同crypto_pkcs7padding
            crypto_pkcs7padding        ///< 填充0-16个字节
        };

        /// @brief 对数据进行填充
        /// @param[in, out] data_buf    待填充数据缓冲区
        /// @param[in, out] data_len    数据字节长度
        /// @param[in] block_size       块大小，128,192或256位(16,24或32字节)
        /// @param[in] mode             填充模式
        /// @return 返回填充后数据长度
        /// @note 填充数据后，会重新调整缓冲区长度并返回调整后的长度
        LSLIB_API lpbyte crypto_padding(__inout__ lpbyte& data_buf, __inout__ size_t& data_len, size_t block_size, crypto_padding_mode mode);

        /// @brief 对填充的数据进行还原
        /// @param[in, out] data_buf    待还原数据缓冲区
        /// @param[in, out] data_len    数据字节长度
        /// @param[in] mode             填充模式
        /// @return 返回还原后真实数据长度
        /// @note 还原数据后，会重新调整缓冲区长度并返回调整后的长度
        LSLIB_API lpbyte crypto_unpadding(__inout__ lpbyte& data_buf, __inout__ size_t& data_len, crypto_padding_mode mode);

        // -----------------------------------------------------------------------
        // DES
        // -----------------------------------------------------------------------

        /// @brief DES加密（ECB模式）
        /// @param[in] data     待加密的数据
        /// @param[in] data_len 待加密数据的字节长度
        /// @param[in] key      加密秘钥，必须为64位（8字节）
        /// @param[in] mode     填充模式
        /// @param[out] out_len 输出加密后的字节长度
        /// @return 返回加密后的数据内容，以string作为保存容器，非字符串数据内容请配合输出长度out_len使用
        LSLIB_API string des_encrypt(lpbyte data, size_t data_len, const lchar key[8], crypto_padding_mode mode, __out__ int* out_len);

        /// @brief DES解密（ECB模式）
        /// @param[in] data     待解密的数据
        /// @param[in] data_len 待解密数据的字节长度，须为8的倍数
        /// @param[in] key      解密秘钥，必须为64位（8字节）
        /// @param[in] mode     填充模式
        /// @param[out] out_len 输出解密后的字节长度
        /// @return 返回解密后的数据内容，以string作为保存容器，非字符串数据内容请配合输出长度out_len使用
        LSLIB_API string des_decrypt(lpbyte data, size_t data_len, const lchar key[8], crypto_padding_mode mode, __out__ int* out_len);

        /// @brief DES加密（CBC模式）
        /// @param[in] data     待加密的数据
        /// @param[in] data_len 待加密数据的字节长度
        /// @param[in] key      加密秘钥，必须为64位（8字节）
        /// @param[in] mode     填充模式
        /// @param[in] iv       初始向量，8字节长度
        /// @param[out] out_len 输出加密后的字节长度
        /// @return 返回加密后的数据内容，以string作为保存容器，非字符串数据内容请配合输出长度out_len使用
        LSLIB_API string des_encrypt_cbc(lpbyte data, size_t data_len, const lchar key[8], crypto_padding_mode mode, const lchar iv[8], __out__ int* out_len);

        /// @brief DES解密（CBC模式）
        /// @param[in] data     待解密的数据
        /// @param[in] data_len 待解密数据的字节长度，须为8的倍数
        /// @param[in] key      解密秘钥，必须为64位（8字节）
        /// @param[in] mode     填充模式
        /// @param[in] iv       初始向量，8字节长度
        /// @param[out] out_len 输出解密后的字节长度
        /// @return 返回解密后的数据内容，以string作为保存容器，非字符串数据内容请配合输出长度out_len使用
        LSLIB_API string des_decrypt_cbc(lpbyte data, size_t data_len, const lchar key[8], crypto_padding_mode mode, const lchar iv[8], __out__ int* out_len);

        /// @brief 3-DES加密（ECB模式）
        /// @param[in] data     待加密的数据
        /// @param[in] data_len 待加密数据的字节长度
        /// @param[in] key      加密秘钥，必须为192位（24字节）
        /// @param[in] mode     填充模式
        /// @param[out] out_len 输出加密后的字节长度
        /// @return 返回加密后的数据内容，以string作为保存容器，非字符串数据内容请配合输出长度out_len使用
        LSLIB_API string three_des_encrypt(lpbyte data, size_t data_len, const lchar key[24], crypto_padding_mode mode, __out__ int* out_len);

        /// @brief 3-DES解密（ECB模式）
        /// @param[in] data     待解密的数据
        /// @param[in] data_len 待解密数据的字节长度，须为8的倍数
        /// @param[in] key      加密秘钥，必须为192位（24字节）
        /// @param[in] mode     填充模式
        /// @param[out] out_len 输出解密后的字节长度
        /// @return 返回解密后的数据内容，以string作为保存容器，非字符串数据内容请配合输出长度out_len使用
        LSLIB_API string three_des_decrypt(lpbyte data, size_t data_len, const lchar key[24], crypto_padding_mode mode, __out__ int* out_len);

        /// @brief 3-DES加密（CBC模式）
        /// @param[in] data     待加密的数据
        /// @param[in] data_len 待加密数据的字节长度
        /// @param[in] key      解密秘钥，必须为192位（24字节）
        /// @param[in] mode     填充模式
        /// @param[in] iv       初始向量，8字节长度
        /// @param[out] out_len 输出加密后的字节长度
        /// @return 返回加密后的数据内容，以string作为保存容器，非字符串数据内容请配合输出长度out_len使用
        LSLIB_API string three_des_encrypt_cbc(lpbyte data, size_t data_len, const lchar key[24], crypto_padding_mode mode, const lchar iv[8], __out__ int* out_len);

        /// @brief DES解密（CBC模式）
        /// @param[in] data     待解密的数据
        /// @param[in] data_len 待解密数据的字节长度，须为8的倍数
        /// @param[in] key      解密秘钥，必须为192位（24字节）
        /// @param[in] mode     填充模式
        /// @param[in] iv       初始向量，8字节长度
        /// @param[out] out_len 输出解密后的字节长度
        /// @return 返回解密后的数据内容，以string作为保存容器，非字符串数据内容请配合输出长度out_len使用
        LSLIB_API string three_des_decrypt_cbc(lpbyte data, size_t data_len, const lchar key[24], crypto_padding_mode mode, const lchar iv[8], __out__ int* out_len);


        // -----------------------------------------------------------------------
        // AES
        // -----------------------------------------------------------------------

        /// @brief AES加密（ECB模式）
        /// @param[in] data     待加密的数据
        /// @param[in] data_len 待加密数据的字节长度
        /// @param[in] key      加密秘钥，必须为128,192或256位(16,24或32字节)
        /// @param[in] key_bits 秘钥长度
        /// @param[in] mode     填充模式，仅当数据长度为16的倍数时，才能使用crypto_nopadding，否则结果未确定
        /// @param[out] out_len 输出加密后的字节长度
        /// @return 返回加密后的数据内容，以string作为保存容器，非字符串数据内容请配合输出长度out_len使用
        LSLIB_API string aes_encrypt(lpbyte data, size_t data_len, lpcstr key, crypto_key_bits key_bits, crypto_padding_mode mode, __out__ int* out_len);

        /// @brief AES解密（ECB模式）
        /// @param[in] data     待解密的数据
        /// @param[in] data_len 待解密数据的字节长度，须为16的倍数
        /// @param[in] key      加密秘钥，必须为128,192或256位(16,24或32字节)
        /// @param[in] key_bits 秘钥长度
        /// @param[in] mode     填充模式，仅当数据长度为16的倍数时，才能使用crypto_nopadding，否则结果未确定
        /// @param[out] out_len 输出解密后的字节长度
        /// @return 返回解密后的数据内容，以string作为保存容器，非字符串数据内容请配合输出长度out_len使用
        LSLIB_API string aes_decrypt(lpbyte data, size_t data_len, lpcstr key, crypto_key_bits key_bits, crypto_padding_mode mode, __out__ int* out_len);

        /// @brief AES加密（CBC模式）
        /// @param[in] data     待加密的数据
        /// @param[in] data_len 待加密数据的字节长度
        /// @param[in] key      加密秘钥，必须为128,192或256位(16,24或32字节)
        /// @param[in] key_bits 秘钥长度
        /// @param[in] mode     填充模式，仅当数据长度为16的倍数时，才能使用crypto_nopadding，否则结果未确定
        /// @param[in] iv       初始向量，16字节长度
        /// @param[out] out_len 输出加密后的字节长度
        /// @return 返回加密后的数据内容，以string作为保存容器，非字符串数据内容请配合输出长度out_len使用
        LSLIB_API string aes_encrypt_cbc(lpbyte data, size_t data_len, lpcstr key, crypto_key_bits key_bits, crypto_padding_mode mode, const lchar iv[16], __out__ int* out_len);

        /// @brief AES解密（CBC模式）
        /// @param[in] data     待解密的数据
        /// @param[in] data_len 待解密数据的字节长度，须为16的倍数
        /// @param[in] key      加密秘钥，必须为128,192或256位(16,24或32字节)
        /// @param[in] key_bits 秘钥长度
        /// @param[in] mode     填充模式，仅当数据长度为16的倍数时，才能使用crypto_nopadding，否则结果未确定
        /// @param[in] iv       初始向量，16字节长度
        /// @param[out] out_len 输出解密后的字节长度
        /// @return 返回解密后的数据内容，以string作为保存容器，非字符串数据内容请配合输出长度out_len使用
        LSLIB_API string aes_decrypt_cbc(lpbyte data, size_t data_len, lpcstr key, crypto_key_bits key_bits, crypto_padding_mode mode, const lchar iv[16], __out__ int* out_len);

        // -----------------------------------------------------------------------
        // url encoding
        // -----------------------------------------------------------------------

        /// @brief 对数据进行URL编码
        /// @param[in] data     需要进行URL编码的数据内容
        /// @param[in] len      数据字节长度
        /// @param[out] out_len 输出编码后的字节长度
        /// @return 返回URL编码后的数据
        LSLIB_API string url_encode(lpbyte data, size_t len);

        /// @brief 对数据进行URL解码
        /// @param[in] data     需要进行URL解码的字符串
        /// @param[out] out_len 输出解码后的字节长度
        /// @return 返回URL解码后的数据，以string作为保存容器，非字符串数据内容请配合输出长度out_len使用
        LSLIB_API string url_decode(lpcstr data, __out__ int* out_len);

        // -----------------------------------------------------------------------
        // encoding convert
        // -----------------------------------------------------------------------

#ifdef USE_LIBICONV
        /// @brief 编码转换
        /// @param[in] data         需要进行转码的数据内容
        /// @param[in] len          数据字节长度
        /// @param[in] from_charset 源编码格式，如"utf-8"
        /// @param[in] to_charset   目标编码格式，如"gb2312"
        /// @param[out] out_len     输出转码后的字节长度
        /// @return 返回转码后的字节内容，以string作为保存容器，非字符串数据内容请配合输出长度out_len使用
        LSLIB_API string encoding_convert(lpbyte data, size_t len, lpcstr from_charset, lpcstr to_charset, __out__ int* out_len);

        /// @brief 编码转换
        /// @param data         需要进行转码的字符串
        /// @param from_charset 源编码格式，如"utf-8"
        /// @param to_charset   目标编码格式，如"gb2312"
        /// @return 返回转码后的字符串
        LSLIB_API string encoding_convert(lpcstr data, lpcstr from_charset, lpcstr to_charset);
#endif

        /// @brief 去除UTF-8字符串的bom
        /// @param[in] data         需要处理的字符串内容
        /// @return 返回处理后的字符串
        LSLIB_API string trim_utf8_bom(lpcstr data);

    } // crypto
}
