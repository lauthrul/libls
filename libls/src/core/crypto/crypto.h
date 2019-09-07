#pragma once

namespace lslib
{
    /// @brief �ӽ������API
    namespace crypto
    {
        // -----------------------------------------------------------------------
        // md5
        // -----------------------------------------------------------------------

        /// @brief ��������md5ֵ
        /// @param data ��Ҫ����md5�������������
        /// @param len  ���ݳ���
        /// @return ����ʮ������md5ֵ�ַ���
        LSLIB_API string md5(_lpbyte data, size_t len);

        /// @brief �����ļ�md5ֵ
        /// @param pfile ��Ҫ����md5������ļ�·��
        /// @return ����ʮ������md5ֵ�ַ���
        LSLIB_API string file_md5(_lpcstr pfile);

        // -----------------------------------------------------------------------
        // sha1
        // -----------------------------------------------------------------------

        /// @brief ��������sha1ֵ
        /// @param data ��Ҫ����sha1�������������
        /// @param len  ���ݳ���
        /// @return ����ʮ������sha1ֵ�ַ���
        LSLIB_API string sha1(_lpbyte data, size_t len);

        /// @brief �����ļ�sha1ֵ
        /// @param pfile ��Ҫ����sha1������ļ�·��
        /// @return ����ʮ������sha1ֵ�ַ���
        LSLIB_API string file_sha1(_lpcstr pfile);

        // -----------------------------------------------------------------------
        // sha224
        // -----------------------------------------------------------------------

        /// @brief ��������sha224ֵ
        /// @param data ��Ҫ����sha224�������������
        /// @param len  ���ݳ���
        /// @return ����ʮ������sha224ֵ�ַ���
        LSLIB_API string sha224(_lpbyte data, size_t len);

        /// @brief �����ļ�sha224ֵ
        /// @param pfile ��Ҫ����sha224������ļ�·��
        /// @return ����ʮ������sha224ֵ�ַ���
        LSLIB_API string file_sha224(_lpcstr pfile);

        // -----------------------------------------------------------------------
        // sha256
        // -----------------------------------------------------------------------

        /// @brief ��������sha256ֵ
        /// @param data ��Ҫ����sha256�������������
        /// @param len  ���ݳ���
        /// @return ����ʮ������sha256ֵ�ַ���
        LSLIB_API string sha256(_lpbyte data, size_t len);

        /// @brief �����ļ�sha256ֵ
        /// @param pfile ��Ҫ����sha256������ļ�·��
        /// @return ����ʮ������sha256ֵ�ַ���
        LSLIB_API string file_sha256(_lpcstr pfile);

        // -----------------------------------------------------------------------
        // sha384
        // -----------------------------------------------------------------------

        /// @brief ��������sha384ֵ
        /// @param data ��Ҫ����sha384�������������
        /// @param len  ���ݳ���
        /// @return ����ʮ������sha384ֵ�ַ���
        LSLIB_API string sha384(_lpbyte data, size_t len);

        /// @brief �����ļ�sha384ֵ
        /// @param pfile ��Ҫ����sha384������ļ�·��
        /// @return ����ʮ������sha384ֵ�ַ���
        LSLIB_API string file_sha384(_lpcstr pfile);

        // -----------------------------------------------------------------------
        // sha512
        // -----------------------------------------------------------------------

        /// @brief ��������sha512ֵ
        /// @param data ��Ҫ����sha512�������������
        /// @param len  ���ݳ���
        /// @return ����ʮ������sha512ֵ�ַ���
        LSLIB_API string sha512(_lpbyte data, size_t len);

        /// @brief �����ļ�sha512ֵ
        /// @param pfile ��Ҫ����sha512������ļ�·��
        /// @return ����ʮ������sha512ֵ�ַ���
        LSLIB_API string file_sha512(_lpcstr pfile);

        // -----------------------------------------------------------------------
        // base64
        // -----------------------------------------------------------------------

        /// @brief �����ݽ���base64����
        /// @param data ��Ҫ����base64�������������
        /// @param len  ���ݳ���
        /// @return ����base64�������ַ���
        LSLIB_API string base64_encode(_lpbyte data, size_t len);

        /// @brief �����ݽ���base64����
        /// @param[in] data     ��Ҫ����base64�������������
        /// @param[in] len      ���ݳ���
        /// @param[out] out_len ����������������ݳ��ȣ�ΪNULLʱ���������
        /// @return ����base64��������������
        /// @post ���ؽ����Ҫ����free()�ͷ�
        LSLIB_API _lpbyte base64_decode(_lpcstr data, size_t len, __out__ int* out_len);

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
        LSLIB_API _lpbyte crypto_padding(__inout__ _lpbyte& data_buf, __inout__ int& data_len, int block_size, crypto_padding_mode mode);

        /// @brief ���������ݽ��л�ԭ
        /// @param[in, out] data_buf    ����ԭ���ݻ�����
        /// @param[in, out] data_len    �����ֽڳ���
        /// @param[in] mode             ���ģʽ
        /// @return ���ػ�ԭ����ʵ���ݳ���
        /// @note ��ԭ���ݺ󣬻����µ������������Ȳ����ص�����ĳ���
        LSLIB_API _lpbyte crypto_unpadding(__inout__ _lpbyte& data_buf, __inout__ int& data_len, crypto_padding_mode mode);

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
        LSLIB_API string des_encrypt(_lpcstr data, int data_len, _lpcstr key, crypto_padding_mode mode, __out__ int* out_len);

        /// @brief DES���ܣ�ECBģʽ��
        /// @param[in] data     �����ܵ�����
        /// @param[in] data_len ���������ݵ��ֽڳ��ȣ���Ϊ8�ı���
        /// @param[in] key      ������Կ������Ϊ64λ��8�ֽڣ�
        /// @param[in] mode     ���ģʽ
        /// @param[out] out_len ������ܺ���ֽڳ���
        /// @return ���ؽ��ܺ���������ݣ���string��Ϊ�������������ַ�����������������������out_lenʹ��
        LSLIB_API string des_decrypt(_lpcstr data, int data_len, _lpcstr key, crypto_padding_mode mode, __out__ int* out_len);

        /// @brief DES���ܣ�CBCģʽ��
        /// @param[in] data     �����ܵ�����
        /// @param[in] data_len ���������ݵ��ֽڳ���
        /// @param[in] key      ������Կ������Ϊ64λ��8�ֽڣ�
        /// @param[in] mode     ���ģʽ
        /// @param[in] iv       ��ʼ������8�ֽڳ���
        /// @param[out] out_len ������ܺ���ֽڳ���
        /// @return ���ؼ��ܺ���������ݣ���string��Ϊ�������������ַ�����������������������out_lenʹ��
        LSLIB_API string des_encrypt_cbc(_lpcstr data, int data_len, _lpcstr key, crypto_padding_mode mode, _lchar iv[8], __out__ int* out_len);

        /// @brief DES���ܣ�CBCģʽ��
        /// @param[in] data     �����ܵ�����
        /// @param[in] data_len ���������ݵ��ֽڳ��ȣ���Ϊ8�ı���
        /// @param[in] key      ������Կ������Ϊ64λ��8�ֽڣ�
        /// @param[in] mode     ���ģʽ
        /// @param[in] iv       ��ʼ������8�ֽڳ���
        /// @param[out] out_len ������ܺ���ֽڳ���
        /// @return ���ؽ��ܺ���������ݣ���string��Ϊ�������������ַ�����������������������out_lenʹ��
        LSLIB_API string des_decrypt_cbc(_lpcstr data, int data_len, _lpcstr key, crypto_padding_mode mode, _lchar iv[8], __out__ int* out_len);

        /// @brief 3-DES���ܣ�ECBģʽ��
        /// @param[in] data     �����ܵ�����
        /// @param[in] data_len ���������ݵ��ֽڳ���
        /// @param[in] key      ������Կ������Ϊ192λ��24�ֽڣ�
        /// @param[in] mode     ���ģʽ
        /// @param[out] out_len ������ܺ���ֽڳ���
        /// @return ���ؼ��ܺ���������ݣ���string��Ϊ�������������ַ�����������������������out_lenʹ��
        LSLIB_API string three_des_encrypt(_lpcstr data, int data_len, _lpcstr key, crypto_padding_mode mode, __out__ int* out_len);

        /// @brief 3-DES���ܣ�ECBģʽ��
        /// @param[in] data     �����ܵ�����
        /// @param[in] data_len ���������ݵ��ֽڳ��ȣ���Ϊ8�ı���
        /// @param[in] key      ������Կ������Ϊ192λ��24�ֽڣ�
        /// @param[in] mode     ���ģʽ
        /// @param[out] out_len ������ܺ���ֽڳ���
        /// @return ���ؽ��ܺ���������ݣ���string��Ϊ�������������ַ�����������������������out_lenʹ��
        LSLIB_API string three_des_decrypt(_lpcstr data, int data_len, _lpcstr key, crypto_padding_mode mode, __out__ int* out_len);

        /// @brief 3-DES���ܣ�CBCģʽ��
        /// @param[in] data     �����ܵ�����
        /// @param[in] data_len ���������ݵ��ֽڳ���
        /// @param[in] key      ������Կ������Ϊ64λ��8�ֽڣ�
        /// @param[in] mode     ���ģʽ
        /// @param[in] iv       ��ʼ������8�ֽڳ���
        /// @param[out] out_len ������ܺ���ֽڳ���
        /// @return ���ؼ��ܺ���������ݣ���string��Ϊ�������������ַ�����������������������out_lenʹ��
        LSLIB_API string three_des_encrypt_cbc(_lpcstr data, int data_len, _lpcstr key, crypto_padding_mode mode, _lchar iv[8], __out__ int* out_len);

        /// @brief DES���ܣ�CBCģʽ��
        /// @param[in] data     �����ܵ�����
        /// @param[in] data_len ���������ݵ��ֽڳ��ȣ���Ϊ8�ı���
        /// @param[in] key      ������Կ������Ϊ64λ��8�ֽڣ�
        /// @param[in] mode     ���ģʽ
        /// @param[in] iv       ��ʼ������8�ֽڳ���
        /// @param[out] out_len ������ܺ���ֽڳ���
        /// @return ���ؽ��ܺ���������ݣ���string��Ϊ�������������ַ�����������������������out_lenʹ��
        LSLIB_API string three_des_decrypt_cbc(_lpcstr data, int data_len, _lpcstr key, crypto_padding_mode mode, _lchar iv[8], __out__ int* out_len);


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
        LSLIB_API string aes_encrypt(_lpcstr data, int data_len, _lpcstr key, crypto_key_bits key_bits, crypto_padding_mode mode, __out__ int* out_len);

        /// @brief AES���ܣ�ECBģʽ��
        /// @param[in] data     �����ܵ�����
        /// @param[in] data_len ���������ݵ��ֽڳ��ȣ���Ϊ16�ı���
        /// @param[in] key      ������Կ������Ϊ128,192��256λ(16,24��32�ֽ�)
        /// @param[in] key_bits ��Կ����
        /// @param[in] mode     ���ģʽ���������ݳ���Ϊ16�ı���ʱ������ʹ��crypto_nopadding��������δȷ��
        /// @param[out] out_len ������ܺ���ֽڳ���
        /// @return ���ؽ��ܺ���������ݣ���string��Ϊ�������������ַ�����������������������out_lenʹ��
        LSLIB_API string aes_decrypt(_lpcstr data, int data_len, _lpcstr key, crypto_key_bits key_bits, crypto_padding_mode mode, __out__ int* out_len);

        /// @brief AES���ܣ�CBCģʽ��
        /// @param[in] data     �����ܵ�����
        /// @param[in] data_len ���������ݵ��ֽڳ���
        /// @param[in] key      ������Կ������Ϊ128,192��256λ(16,24��32�ֽ�)
        /// @param[in] key_bits ��Կ����
        /// @param[in] mode     ���ģʽ���������ݳ���Ϊ16�ı���ʱ������ʹ��crypto_nopadding��������δȷ��
        /// @param[in] iv       ��ʼ������16�ֽڳ���
        /// @param[out] out_len ������ܺ���ֽڳ���
        /// @return ���ؼ��ܺ���������ݣ���string��Ϊ�������������ַ�����������������������out_lenʹ��
        LSLIB_API string aes_encrypt_cbc(_lpcstr data, int data_len, _lpcstr key, crypto_key_bits key_bits, crypto_padding_mode mode, _lchar iv[16], __out__ int* out_len);

        /// @brief AES���ܣ�CBCģʽ��
        /// @param[in] data     �����ܵ�����
        /// @param[in] data_len ���������ݵ��ֽڳ��ȣ���Ϊ16�ı���
        /// @param[in] key      ������Կ������Ϊ128,192��256λ(16,24��32�ֽ�)
        /// @param[in] key_bits ��Կ����
        /// @param[in] mode     ���ģʽ���������ݳ���Ϊ16�ı���ʱ������ʹ��crypto_nopadding��������δȷ��
        /// @param[in] iv       ��ʼ������16�ֽڳ���
        /// @param[out] out_len ������ܺ���ֽڳ���
        /// @return ���ؽ��ܺ���������ݣ���string��Ϊ�������������ַ�����������������������out_lenʹ��
        LSLIB_API string aes_decrypt_cbc(_lpcstr data, int data_len, _lpcstr key, crypto_key_bits key_bits, crypto_padding_mode mode, _lchar iv[16], __out__ int* out_len);

        // -----------------------------------------------------------------------
        // url encoding
        // -----------------------------------------------------------------------

        /// @brief �����ݽ���URL����
        /// @param[in] data     ��Ҫ����URL�������������
        /// @param[in] len      �����ֽڳ���
        /// @param[out] out_len ����������ֽڳ���
        /// @return ����URL���������ݣ���string��Ϊ�������������ַ�����������������������out_lenʹ��
        LSLIB_API string url_encode(_lpcstr data, int len, __out__ int* out_len = NULL);

        /// @brief �����ݽ���URL����
        /// @param[in] data     ��Ҫ����URL�������������
        /// @param[in] len      �����ֽڳ���
        /// @param[out] out_len ����������ֽڳ���
        /// @return ����URL���������ݣ���string��Ϊ�������������ַ�����������������������out_lenʹ��
        LSLIB_API string url_decode(_lpcstr data, int len, __out__ int* out_len = NULL);

        // -----------------------------------------------------------------------
        // encoding convert
        // -----------------------------------------------------------------------

#ifdef USE_LIBICONV
        /// @brief ����ת��
        /// @param[in] from_charset Դ�����ʽ����"utf-8"
        /// @param[in] to_charset   Ŀ������ʽ����"gb2312"
        /// @param[in] inbuf        ��ת�������
        /// @param[in] inlen        ���ݳ���
        /// @param[in, out] outbuf  ���������
        /// @param[in, out] outlen  �������������
        /// @return �ɹ�����ת�볤�ȣ�ʧ�ܷ��ظ���
        LSLIB_API int encoding_convert(_lpcstr from_charset, _lpcstr to_charset, _lpcstr inbuf, size_t inlen, __inout__ _lpstr outbuf, __inout__ size_t outlen);

        /// @brief �ַ�������ת��
        /// @param data         ��ת����ַ���
        /// @param from_charset Դ�����ʽ����"utf-8"
        /// @param to_charset   Ŀ������ʽ����"gb2312"
        /// @return �ɹ�����ת�볤�ȣ�ʧ�ܷ��ظ���
        LSLIB_API string encoding_convert(_lpcstr data, _lpcstr from_charset, _lpcstr to_charset);
#endif

    } // crypto
}
