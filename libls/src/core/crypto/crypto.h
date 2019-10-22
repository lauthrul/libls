#pragma once

namespace lslib
{
    /// @brief �ӽ������
    namespace crypto
    {
#define DEFAULT_CRYPT_KEY       "l53d70p3"
#define DEFAULT_CRYPT_IV        "12345678"

        // -----------------------------------------------------------------------
        // md5
        // -----------------------------------------------------------------------

        /// @brief ��������md5ֵ
        /// @param data ��Ҫ����md5�������������
        /// @param len  ���ݳ���
        /// @return ����ʮ������md5ֵ�ַ���
        LSLIB_API string md5(lpbyte data, size_t len);

        // -----------------------------------------------------------------------
        // sha1
        // -----------------------------------------------------------------------

        /// @brief ��������sha1ֵ
        /// @param data ��Ҫ����sha1�������������
        /// @param len  ���ݳ���
        /// @return ����ʮ������sha1ֵ�ַ���
        LSLIB_API string sha1(lpbyte data, size_t len);

        // -----------------------------------------------------------------------
        // sha224
        // -----------------------------------------------------------------------

        /// @brief ��������sha224ֵ
        /// @param data ��Ҫ����sha224�������������
        /// @param len  ���ݳ���
        /// @return ����ʮ������sha224ֵ�ַ���
        LSLIB_API string sha224(lpbyte data, size_t len);


        // -----------------------------------------------------------------------
        // sha256
        // -----------------------------------------------------------------------

        /// @brief ��������sha256ֵ
        /// @param data ��Ҫ����sha256�������������
        /// @param len  ���ݳ���
        /// @return ����ʮ������sha256ֵ�ַ���
        LSLIB_API string sha256(lpbyte data, size_t len);

        // -----------------------------------------------------------------------
        // sha384
        // -----------------------------------------------------------------------

        /// @brief ��������sha384ֵ
        /// @param data ��Ҫ����sha384�������������
        /// @param len  ���ݳ���
        /// @return ����ʮ������sha384ֵ�ַ���
        LSLIB_API string sha384(lpbyte data, size_t len);

        // -----------------------------------------------------------------------
        // sha512
        // -----------------------------------------------------------------------

        /// @brief ��������sha512ֵ
        /// @param data ��Ҫ����sha512�������������
        /// @param len  ���ݳ���
        /// @return ����ʮ������sha512ֵ�ַ���
        LSLIB_API string sha512(lpbyte data, size_t len);

        // -----------------------------------------------------------------------
        // base64
        // -----------------------------------------------------------------------

        /// @brief �����ݽ���base64����
        /// @param data ��Ҫ����base64�������������
        /// @param len  ���ݳ���
        /// @return ����base64�������ַ���
        LSLIB_API string base64_encode(lpbyte data, size_t len, __out__ int* out_len);

        /// @brief �����ݽ���base64����
        /// @param[in] data     ��Ҫ����base64�������������
        /// @param[in] len      ���ݳ���
        /// @param[out] out_len ����������������ݳ��ȣ�ΪNULLʱ���������
        /// @return ����base64�������������ݣ���string��Ϊ�������������ַ�����������������������out_lenʹ��
        LSLIB_API string base64_decode(lpbyte data, size_t len, __out__ int* out_len);

        // -----------------------------------------------------------------------
        // common
        // -----------------------------------------------------------------------

        /// ������Կ����ö��
        enum crypto_key_bits
        {
            crypto_bit128 = 128,    ///< 128λ��16�ֽڣ���Կ����
            crypto_bit192 = 192,    ///< 192λ��24�ֽڣ���Կ����
            crypto_bit256 = 256     ///< 256λ��32�ֽڣ���Կ����
        };

        /// �������ģʽö��
        enum crypto_padding_mode
        {
            crypto_nopadding,          ///< ����䡣�������СΪ8�ֽڵı���ʱʹ�ã�������δȷ��
            crypto_zeropadding,        ///< ���0��������/��������Ϊ�ַ���ʱʹ��
            crypto_pkcs5padding,       ///< ���0-8���ֽڡ����СΪ8���ֽ�ʱʹ�á������СΪ16���ֽ�ʱ��ͬcrypto_pkcs7padding
            crypto_pkcs7padding        ///< ���0-16���ֽ�
        };

        /// @brief �����ݽ������
        /// @param[in, out] data_buf    ��������ݻ�����
        /// @param[in, out] data_len    �����ֽڳ���
        /// @param[in] block_size       ���С��128,192��256λ(16,24��32�ֽ�)
        /// @param[in] mode             ���ģʽ
        /// @return �����������ݳ���
        /// @note ������ݺ󣬻����µ������������Ȳ����ص�����ĳ���
        LSLIB_API lpbyte crypto_padding(__inout__ lpbyte& data_buf, __inout__ size_t& data_len, size_t block_size, crypto_padding_mode mode);

        /// @brief ���������ݽ��л�ԭ
        /// @param[in, out] data_buf    ����ԭ���ݻ�����
        /// @param[in, out] data_len    �����ֽڳ���
        /// @param[in] mode             ���ģʽ
        /// @return ���ػ�ԭ����ʵ���ݳ���
        /// @note ��ԭ���ݺ󣬻����µ������������Ȳ����ص�����ĳ���
        LSLIB_API lpbyte crypto_unpadding(__inout__ lpbyte& data_buf, __inout__ size_t& data_len, crypto_padding_mode mode);

        // -----------------------------------------------------------------------
        // DES
        // -----------------------------------------------------------------------

        /// @brief DES���ܣ�ECBģʽ��
        /// @param[in] data     �����ܵ�����
        /// @param[in] data_len ���������ݵ��ֽڳ���
        /// @param[in] key      ������Կ������Ϊ64λ��8�ֽڣ�
        /// @param[in] mode     ���ģʽ
        /// @param[out] out_len ������ܺ���ֽڳ���
        /// @return ���ؼ��ܺ���������ݣ���string��Ϊ�������������ַ�����������������������out_lenʹ��
        LSLIB_API string des_encrypt(lpbyte data, size_t data_len, const lchar key[8], crypto_padding_mode mode, __out__ int* out_len);

        /// @brief DES���ܣ�ECBģʽ��
        /// @param[in] data     �����ܵ�����
        /// @param[in] data_len ���������ݵ��ֽڳ��ȣ���Ϊ8�ı���
        /// @param[in] key      ������Կ������Ϊ64λ��8�ֽڣ�
        /// @param[in] mode     ���ģʽ
        /// @param[out] out_len ������ܺ���ֽڳ���
        /// @return ���ؽ��ܺ���������ݣ���string��Ϊ�������������ַ�����������������������out_lenʹ��
        LSLIB_API string des_decrypt(lpbyte data, size_t data_len, const lchar key[8], crypto_padding_mode mode, __out__ int* out_len);

        /// @brief DES���ܣ�CBCģʽ��
        /// @param[in] data     �����ܵ�����
        /// @param[in] data_len ���������ݵ��ֽڳ���
        /// @param[in] key      ������Կ������Ϊ64λ��8�ֽڣ�
        /// @param[in] mode     ���ģʽ
        /// @param[in] iv       ��ʼ������8�ֽڳ���
        /// @param[out] out_len ������ܺ���ֽڳ���
        /// @return ���ؼ��ܺ���������ݣ���string��Ϊ�������������ַ�����������������������out_lenʹ��
        LSLIB_API string des_encrypt_cbc(lpbyte data, size_t data_len, const lchar key[8], crypto_padding_mode mode, const lchar iv[8], __out__ int* out_len);

        /// @brief DES���ܣ�CBCģʽ��
        /// @param[in] data     �����ܵ�����
        /// @param[in] data_len ���������ݵ��ֽڳ��ȣ���Ϊ8�ı���
        /// @param[in] key      ������Կ������Ϊ64λ��8�ֽڣ�
        /// @param[in] mode     ���ģʽ
        /// @param[in] iv       ��ʼ������8�ֽڳ���
        /// @param[out] out_len ������ܺ���ֽڳ���
        /// @return ���ؽ��ܺ���������ݣ���string��Ϊ�������������ַ�����������������������out_lenʹ��
        LSLIB_API string des_decrypt_cbc(lpbyte data, size_t data_len, const lchar key[8], crypto_padding_mode mode, const lchar iv[8], __out__ int* out_len);

        /// @brief 3-DES���ܣ�ECBģʽ��
        /// @param[in] data     �����ܵ�����
        /// @param[in] data_len ���������ݵ��ֽڳ���
        /// @param[in] key      ������Կ������Ϊ192λ��24�ֽڣ�
        /// @param[in] mode     ���ģʽ
        /// @param[out] out_len ������ܺ���ֽڳ���
        /// @return ���ؼ��ܺ���������ݣ���string��Ϊ�������������ַ�����������������������out_lenʹ��
        LSLIB_API string three_des_encrypt(lpbyte data, size_t data_len, const lchar key[24], crypto_padding_mode mode, __out__ int* out_len);

        /// @brief 3-DES���ܣ�ECBģʽ��
        /// @param[in] data     �����ܵ�����
        /// @param[in] data_len ���������ݵ��ֽڳ��ȣ���Ϊ8�ı���
        /// @param[in] key      ������Կ������Ϊ192λ��24�ֽڣ�
        /// @param[in] mode     ���ģʽ
        /// @param[out] out_len ������ܺ���ֽڳ���
        /// @return ���ؽ��ܺ���������ݣ���string��Ϊ�������������ַ�����������������������out_lenʹ��
        LSLIB_API string three_des_decrypt(lpbyte data, size_t data_len, const lchar key[24], crypto_padding_mode mode, __out__ int* out_len);

        /// @brief 3-DES���ܣ�CBCģʽ��
        /// @param[in] data     �����ܵ�����
        /// @param[in] data_len ���������ݵ��ֽڳ���
        /// @param[in] key      ������Կ������Ϊ192λ��24�ֽڣ�
        /// @param[in] mode     ���ģʽ
        /// @param[in] iv       ��ʼ������8�ֽڳ���
        /// @param[out] out_len ������ܺ���ֽڳ���
        /// @return ���ؼ��ܺ���������ݣ���string��Ϊ�������������ַ�����������������������out_lenʹ��
        LSLIB_API string three_des_encrypt_cbc(lpbyte data, size_t data_len, const lchar key[24], crypto_padding_mode mode, const lchar iv[8], __out__ int* out_len);

        /// @brief DES���ܣ�CBCģʽ��
        /// @param[in] data     �����ܵ�����
        /// @param[in] data_len ���������ݵ��ֽڳ��ȣ���Ϊ8�ı���
        /// @param[in] key      ������Կ������Ϊ192λ��24�ֽڣ�
        /// @param[in] mode     ���ģʽ
        /// @param[in] iv       ��ʼ������8�ֽڳ���
        /// @param[out] out_len ������ܺ���ֽڳ���
        /// @return ���ؽ��ܺ���������ݣ���string��Ϊ�������������ַ�����������������������out_lenʹ��
        LSLIB_API string three_des_decrypt_cbc(lpbyte data, size_t data_len, const lchar key[24], crypto_padding_mode mode, const lchar iv[8], __out__ int* out_len);


        // -----------------------------------------------------------------------
        // AES
        // -----------------------------------------------------------------------

        /// @brief AES���ܣ�ECBģʽ��
        /// @param[in] data     �����ܵ�����
        /// @param[in] data_len ���������ݵ��ֽڳ���
        /// @param[in] key      ������Կ������Ϊ128,192��256λ(16,24��32�ֽ�)
        /// @param[in] key_bits ��Կ����
        /// @param[in] mode     ���ģʽ���������ݳ���Ϊ16�ı���ʱ������ʹ��crypto_nopadding��������δȷ��
        /// @param[out] out_len ������ܺ���ֽڳ���
        /// @return ���ؼ��ܺ���������ݣ���string��Ϊ�������������ַ�����������������������out_lenʹ��
        LSLIB_API string aes_encrypt(lpbyte data, size_t data_len, lpcstr key, crypto_key_bits key_bits, crypto_padding_mode mode, __out__ int* out_len);

        /// @brief AES���ܣ�ECBģʽ��
        /// @param[in] data     �����ܵ�����
        /// @param[in] data_len ���������ݵ��ֽڳ��ȣ���Ϊ16�ı���
        /// @param[in] key      ������Կ������Ϊ128,192��256λ(16,24��32�ֽ�)
        /// @param[in] key_bits ��Կ����
        /// @param[in] mode     ���ģʽ���������ݳ���Ϊ16�ı���ʱ������ʹ��crypto_nopadding��������δȷ��
        /// @param[out] out_len ������ܺ���ֽڳ���
        /// @return ���ؽ��ܺ���������ݣ���string��Ϊ�������������ַ�����������������������out_lenʹ��
        LSLIB_API string aes_decrypt(lpbyte data, size_t data_len, lpcstr key, crypto_key_bits key_bits, crypto_padding_mode mode, __out__ int* out_len);

        /// @brief AES���ܣ�CBCģʽ��
        /// @param[in] data     �����ܵ�����
        /// @param[in] data_len ���������ݵ��ֽڳ���
        /// @param[in] key      ������Կ������Ϊ128,192��256λ(16,24��32�ֽ�)
        /// @param[in] key_bits ��Կ����
        /// @param[in] mode     ���ģʽ���������ݳ���Ϊ16�ı���ʱ������ʹ��crypto_nopadding��������δȷ��
        /// @param[in] iv       ��ʼ������16�ֽڳ���
        /// @param[out] out_len ������ܺ���ֽڳ���
        /// @return ���ؼ��ܺ���������ݣ���string��Ϊ�������������ַ�����������������������out_lenʹ��
        LSLIB_API string aes_encrypt_cbc(lpbyte data, size_t data_len, lpcstr key, crypto_key_bits key_bits, crypto_padding_mode mode, const lchar iv[16], __out__ int* out_len);

        /// @brief AES���ܣ�CBCģʽ��
        /// @param[in] data     �����ܵ�����
        /// @param[in] data_len ���������ݵ��ֽڳ��ȣ���Ϊ16�ı���
        /// @param[in] key      ������Կ������Ϊ128,192��256λ(16,24��32�ֽ�)
        /// @param[in] key_bits ��Կ����
        /// @param[in] mode     ���ģʽ���������ݳ���Ϊ16�ı���ʱ������ʹ��crypto_nopadding��������δȷ��
        /// @param[in] iv       ��ʼ������16�ֽڳ���
        /// @param[out] out_len ������ܺ���ֽڳ���
        /// @return ���ؽ��ܺ���������ݣ���string��Ϊ�������������ַ�����������������������out_lenʹ��
        LSLIB_API string aes_decrypt_cbc(lpbyte data, size_t data_len, lpcstr key, crypto_key_bits key_bits, crypto_padding_mode mode, const lchar iv[16], __out__ int* out_len);

        // -----------------------------------------------------------------------
        // url encoding
        // -----------------------------------------------------------------------

        /// @brief �����ݽ���URL����
        /// @param[in] data     ��Ҫ����URL�������������
        /// @param[in] len      �����ֽڳ���
        /// @param[out] out_len ����������ֽڳ���
        /// @return ����URL���������ݣ���string��Ϊ�������������ַ�����������������������out_lenʹ��
        LSLIB_API string url_encode(lpbyte data, size_t len, __out__ int* out_len);

        /// @brief �����ݽ���URL����
        /// @param[in] data     ��Ҫ����URL�������������
        /// @param[in] len      �����ֽڳ���
        /// @param[out] out_len ����������ֽڳ���
        /// @return ����URL���������ݣ���string��Ϊ�������������ַ�����������������������out_lenʹ��
        LSLIB_API string url_decode(lpbyte data, size_t len, __out__ int* out_len);

        // -----------------------------------------------------------------------
        // encoding convert
        // -----------------------------------------------------------------------

#ifdef USE_LIBICONV
        /// @brief ����ת��
        /// @param[in] data         ��Ҫ����ת�����������
        /// @param[in] len          �����ֽڳ���
        /// @param[in] from_charset Դ�����ʽ����"utf-8"
        /// @param[in] to_charset   Ŀ������ʽ����"gb2312"
        /// @param[out] out_len     ���ת�����ֽڳ���
        /// @return ����ת�����ֽ����ݣ���string��Ϊ�������������ַ�����������������������out_lenʹ��
        LSLIB_API string encoding_convert(lpcstr data, size_t len, lpcstr from_charset, lpcstr to_charset, __out__ int* out_len);
#endif

    } // crypto
}
