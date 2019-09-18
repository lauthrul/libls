#pragma once
#include <algorithm>
#include <functional>

//////////////////////////////////////////////////////////////////////////
namespace lslib
{
    declare_stl_obj(string);

    //////////////////////////////////////////////////////////////////////////
    /// @brief string�ķ�װ��
    /// @note ����ʹ��
    class LSLIB_API lstring
    {
    public:
        lstring();
        lstring(_lpcstr lpstr);
        lstring(_lchar c, size_t counts = 1);
        lstring(const string& str);
        ~lstring();

        operator _lpcstr();

    public:
        string str;
    };

    //////////////////////////////////////////////////////////////////////////
    /// @brief �ַ����������
    namespace strtool
    {
        /// �ж��ַ����Ƿ�Ϊ��
        LSLIB_API const bool is_empty(_lpcstr str);

        /// �ж��ַ����Ƿ�Ϊ��
        LSLIB_API const bool is_empty(const string& str);

        /// @brief �ж��ַ����Ƿ�Ϊ�հ��ַ�
        /// @details 
        /// �հ��ַ��������¼��֣�
        /// 
        /// - �ո� (' ')
        /// - �Ʊ�� ('\t')
        /// - �س� ('\r')
        /// - ���� ('\\n')
        /// - ��ֱ�Ʊ�� ('\v')
        /// - ��ҳ ('\f')
        LSLIB_API const bool is_space(_lpcstr str);

        /// @brief �ж��ַ����Ƿ�Ϊ�հ��ַ�
        /// @details �հ��ַ��ο� ::is_space ˵��
        LSLIB_API const bool is_space(const string& str);

        /// �ж��ַ����Ƿ�ΪȫСд
        LSLIB_API const bool is_lower(_lpcstr str);

        /// �ж��ַ����Ƿ�ΪȫСд
        LSLIB_API const bool is_lower(const string& str);

        /// �ж��ַ����Ƿ�Ϊȫ��д
        LSLIB_API const bool is_upper(_lpcstr str);

        /// �ж��ַ����Ƿ�Ϊȫ��д
        LSLIB_API const bool is_upper(const string& str);

        /// �ж��ַ����Ƿ�Ϊȫ����
        LSLIB_API const bool is_digital(_lpcstr str);

        /// �ж��ַ����Ƿ�Ϊȫ����
        LSLIB_API const bool is_digital(const string& str);

        /// �ж��ַ����Ƿ�Ϊ���ͣ�����������
        LSLIB_API const bool is_int(_lpcstr str);

        /// �ж��ַ����Ƿ�Ϊ���ͣ�����������
        LSLIB_API const bool is_int(const string& str);

        /// �ж��ַ����Ƿ�Ϊ������
        LSLIB_API const bool is_float(_lpcstr str);

        /// �ж��ַ����Ƿ�Ϊ������
        LSLIB_API const bool is_float(const string& str);

        /// @brief �ж��ַ����Ƿ�Ϊ������
        /// @param str ���жϵ��ַ���
        /// @param numeric ���ж��ַ����Ƿ�Ϊ�����Ͳ���ֵ����`0`��`1`
        LSLIB_API const bool is_bool(_lpcstr str, bool numeric = false);

        /// @brief �ж��ַ����Ƿ�Ϊ������
        /// @param str ���жϵ��ַ���
        /// @param numeric ���ж��ַ����Ƿ�Ϊ�����Ͳ���ֵ����`0`��`1`
        LSLIB_API const bool is_bool(const string& str, bool numeric = false);

        /// @brief ת���ַ���Ϊ����
        /// @details �������ַ�����ת��ʧ�ܲ�����0
        LSLIB_API const int to_int(_lpcstr str);

        /// @brief ת���ַ���Ϊ����
        /// @details �������ַ�����ת��ʧ�ܲ�����0
        LSLIB_API const int to_int(const string& str);

        /// @brief ת���ַ���Ϊ������
        /// @details �Ǹ������ַ�����ת��ʧ�ܲ�����0
        LSLIB_API const double to_float(_lpcstr str);

        /// @brief ת���ַ���Ϊ������
        /// @details �Ǹ������ַ�����ת��ʧ�ܲ�����0
        LSLIB_API const double to_float(const string& str);

        /// @brief ת���ַ���Ϊ������
        /// @details �ǲ������ַ�����ת��ʧ�ܲ�����false
        LSLIB_API const bool to_bool(_lpcstr str);

        /// @brief ת���ַ���Ϊ������
        /// @details �ǲ������ַ�����ת��ʧ�ܲ�����false
        LSLIB_API const bool to_bool(const string& str);

        /// ת������Ϊ�ַ���
        LSLIB_API string from_int(int value);

        /// ת����������Ϊ�ַ���
        LSLIB_API string from_float(double value, int bit);

        /// ת��������Ϊ�ַ���
        LSLIB_API string from_bool(bool value);

        /// ���ַ���ת��ΪСд
        LSLIB_API string lower(_lpcstr str);

        /// ���ַ���ת��ΪСд
        LSLIB_API string lower(const string& str);

        /// ���ַ���ת��Ϊ��д
        LSLIB_API string upper(_lpcstr str);

        /// ���ַ���ת��Ϊ��д
        LSLIB_API string upper(const string& str);

        /// @brief ���ַ���ת��Ϊ��д
        /// @details �հ��ַ��ο� ::is_space ˵��
        LSLIB_API string trim(_lpcstr str);

        /// @brief ȥ���ַ���ǰ��Ŀհ��ַ�
        /// @details �հ��ַ��ο� ::is_space ˵��
        LSLIB_API string trim(const string& str);

        /// @brief ȥ���ַ�����ͷ�Ŀհ��ַ�
        /// @details �հ��ַ��ο� ::is_space ˵��
        LSLIB_API string trim_left(_lpcstr str);

        /// @brief ȥ���ַ�����ͷ�Ŀհ��ַ�
        /// @details �հ��ַ��ο� ::is_space ˵��
        LSLIB_API string trim_left(const string& str);

        /// @brief ȥ���ַ�����β�Ŀհ��ַ�
        /// @details �հ��ַ��ο� ::is_space ˵��
        LSLIB_API string trim_right(_lpcstr str);

        /// @brief ȥ���ַ�����β�Ŀհ��ַ�
        /// @details �հ��ַ��ο� ::is_space ˵��
        LSLIB_API string trim_right(const string& str);

        /// @brief ȥ���ַ���ǰ��ָ�����ַ�
        /// @param str �������ַ���
        /// @param c ��ȥ�����ַ�
        LSLIB_API string trim(_lpcstr str, _lchar c);

        /// @brief ȥ���ַ���ǰ��ָ�����ַ�
        /// @param str �������ַ���
        /// @param c ��ȥ�����ַ�
        LSLIB_API string trim(const string& str, _lchar c);

        /// @brief ȥ���ַ�����ͷָ�����ַ�
        /// @param str �������ַ���
        /// @param c ��ȥ�����ַ�
        LSLIB_API string trim_left(_lpcstr str, _lchar c);

        /// @brief ȥ���ַ�����ͷָ�����ַ�
        /// @param str �������ַ���
        /// @param c ��ȥ�����ַ�
        LSLIB_API string trim_left(const string& str, _lchar c);

        /// @brief ȥ���ַ�����βָ�����ַ�
        /// @param str �������ַ���
        /// @param c ��ȥ�����ַ�
        LSLIB_API string trim_right(_lpcstr str, _lchar c);

        /// @brief ȥ���ַ�����βָ�����ַ�
        /// @param str �������ַ���
        /// @param c ��ȥ�����ַ�
        LSLIB_API string trim_right(const string& str, _lchar c);

        /// @brief �ַ����滻
        /// @param str �������ַ���
        /// @param needle ��Ҫ���ҵ��ַ�
        /// @param replacement ��Ҫ�滻�ɵ��ַ�
        /// @param index ��ʼ�滻��λ��������Ĭ�ϴӵ�һ���ַ���ʼ
        /// @param counts ��ʼ�滻��λ��������Ĭ��ֵ-1��ʾȫ���滻
        /// @return �����滻����ַ���
        LSLIB_API string replace(_lpcstr str, _lchar needle, _lchar replacement, size_t index = 0, int counts = -1);

        /// @brief �ַ����滻
        /// @param str �������ַ���
        /// @param needle ��Ҫ���ҵ��ַ�
        /// @param replacement ��Ҫ�滻�ɵ��ַ�
        /// @param index ��ʼ�滻��λ��������Ĭ�ϴӵ�һ���ַ���ʼ
        /// @param counts ��ʼ�滻��λ��������Ĭ��ֵ-1��ʾȫ���滻
        /// @return �����滻����ַ���
        LSLIB_API string replace(const string& str, _lchar needle, _lchar replacement, size_t index = 0, int counts = -1);

        /// @brief �ַ����滻
        /// @param str �������ַ���
        /// @param needle ��Ҫ���ҵ��ַ���
        /// @param replacement ��Ҫ�滻�ɵ��ַ���
        /// @param index ��ʼ�滻��λ��������Ĭ�ϴӵ�һ���ַ���ʼ
        /// @param counts ��ʼ�滻��λ��������Ĭ��ֵ-1��ʾȫ���滻
        /// @return �����滻����ַ���
        LSLIB_API string replace(_lpcstr str, _lpcstr needle, _lpcstr replacement, size_t index = 0, int counts = -1);

        /// @brief �ַ����滻
        /// @param str �������ַ���
        /// @param needle ��Ҫ���ҵ��ַ���
        /// @param replacement ��Ҫ�滻�ɵ��ַ���
        /// @param index ��ʼ�滻��λ��������Ĭ�ϴӵ�һ���ַ���ʼ
        /// @param counts ��ʼ�滻��λ��������Ĭ��ֵ-1��ʾȫ���滻
        /// @return �����滻����ַ���
        LSLIB_API string replace(const string& str, const string& needle, const string& replacement, size_t index = 0, int counts = -1);

        /// @brief ��ʽ���ַ���
        /// @param pfmt ��Ҫ��ʽ���ɵ��ַ�����ʽ
        /// @param args �ɱ���б���pfmt��ʽһһ��Ӧ
        LSLIB_API string format(_lpcstr pfmt, const va_list& args);

        /// @brief ��ʽ���ַ���
        /// @param pfmt ��Ҫ��ʽ���ɵ��ַ�����ʽ
        /// @param ...  �ɱ��������pfmt��ʽһһ��Ӧ
        LSLIB_API string format(_lpcstr pfmt, ...);

        /// @brief ׷�Ӹ�ʽ���ַ���
        /// @param str  ��Ҫ׷�ӵ�Դ�ַ���
        /// @param pfmt Ҫ׷�ӵ��ַ�����ʽ
        /// @param ...  �ɱ��������pfmt��ʽһһ��Ӧ
        /// @note ����ֵΪ��һ�������ĸ���
        LSLIB_API string append_format(__out__ string& str, _lpcstr pfmt, ...);

        /// @brief �ָ��ַ���
        /// @param[out] dest        ����ָ��ķ�������
        /// @param[in] src          ���ָ��Դ�ַ���
        /// @param[in] patten       �ָ��
        /// @param[in] allow_empty  �ָ���Ƿ񱣴�յķ���
        /// @note ����ֵ�ָ��ķ��鳤��
        LSLIB_API const int split(__out__ string_array& dest, _lpcstr src, _lpcstr patten, bool allow_empty = true);

        /// @brief �ָ��ַ���
        /// @param[out] dest        ����ָ��ķ�������
        /// @param[in] src          ���ָ��Դ�ַ���
        /// @param[in] patten       �ָ��
        /// @param[in] allow_empty  �ָ���Ƿ񱣴�յķ���
        /// @return ����ֵ�ָ��ķ��鳤��
        LSLIB_API const int split(__out__ string_array& dest, const string& src, const string& patten, bool allow_empty = true);

        /// @brief ��һ��16�����ַ�תΪ�ֽ�
        /// @details �� 'a' -> 0xa
        LSLIB_API _lbyte hex_char_to_byte(_lchar c);

        /// @brief ��һ���ֽ�תΪ16�����ַ�
        /// @details �� 0xa -> 'a'
        LSLIB_API _lchar byte_to_hex_char(_lbyte b);

        /// @brief ��16�����ַ���תΪ�ֽ�����
        /// @details �� "da3fed134a" -> { 0xda, 0x3f, 0xed, 0x13, 0x4a }
        LSLIB_API _lbyte_array hex_str_to_byte_array(_lpcstr data);

        /// @brief ��16�����ַ���תΪ�ֽ�����
        /// @details �� "da3fed134a" -> { 0xda, 0x3f, 0xed, 0x13, 0x4a }
        LSLIB_API _lbyte_array hex_str_to_byte_array(const string& data);

        /// @brief ���ֽ�����תΪ16�����ַ���
        /// @details �� { 0xda, 0x3f, 0xed, 0x13, 0x4a } -> "da3fed134a"
        LSLIB_API string byte_array_to_hex_str(_lbyte_array data);

        /// @brief ���ֽ�����תΪ16�����ַ���
        /// @details �� { 0xda, 0x3f, 0xed, 0x13, 0x4a } -> "da3fed134a"
        LSLIB_API string byte_array_to_hex_str(_lbyte data[], int len);

    }
}
