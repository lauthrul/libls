#pragma once
#include <algorithm>
#include <functional>

//////////////////////////////////////////////////////////////////////////
namespace lslib
{
    define_stl_type(string);

    //////////////////////////////////////////////////////////////////////////
    /// @brief string的封装类
    /// @note 谨慎使用
    class LSLIB_API lstring
    {
    public:
        lstring();
        lstring(lpcstr lpstr);
        lstring(lchar c, size_t counts = 1);
        lstring(const string& str);
        ~lstring();

        operator lpcstr();

    public:
        string str;
    };

    //////////////////////////////////////////////////////////////////////////
    /// @brief 字符串操作相关
    namespace strtool
    {
        /// 判断字符串是否为空
        LSLIB_API const bool is_empty(lpcstr str);

        /// 判断字符串是否为空
        LSLIB_API const bool is_empty(const string& str);

        /// @brief 判断字符串是否为空白字符
        /// @details 
        /// 空白字符包括以下几种：
        /// 
        /// - 空格 (' ')
        /// - 制表符 ('\t')
        /// - 回车 ('\r')
        /// - 换行 ('\\n')
        /// - 垂直制表符 ('\v')
        /// - 换页 ('\f')
        LSLIB_API const bool is_space(lpcstr str);

        /// @brief 判断字符串是否为空白字符
        /// @details 空白字符参考 ::is_space 说明
        LSLIB_API const bool is_space(const string& str);

        /// 判断字符串是否为全小写
        LSLIB_API const bool is_lower(lpcstr str);

        /// 判断字符串是否为全小写
        LSLIB_API const bool is_lower(const string& str);

        /// 判断字符串是否为全大写
        LSLIB_API const bool is_upper(lpcstr str);

        /// 判断字符串是否为全大写
        LSLIB_API const bool is_upper(const string& str);

        /// 判断字符串是否为全数字
        LSLIB_API const bool is_digital(lpcstr str);

        /// 判断字符串是否为全数字
        LSLIB_API const bool is_digital(const string& str);

        /// 判断字符串是否为整型（包括负数）
        LSLIB_API const bool is_int(lpcstr str);

        /// 判断字符串是否为整型（包括负数）
        LSLIB_API const bool is_int(const string& str);

        /// 判断字符串是否为浮点数
        LSLIB_API const bool is_float(lpcstr str);

        /// 判断字符串是否为浮点数
        LSLIB_API const bool is_float(const string& str);

        /// @brief 判断字符串是否为布尔型
        /// @param str 待判断的字符串
        /// @param numeric 待判断字符串是否允许为整数型布尔值`0`和`1`
        LSLIB_API const bool is_bool(lpcstr str, bool numeric = true);

        /// @brief 判断字符串是否为布尔型
        /// @param str 待判断的字符串
        /// @param numeric 待判断字符串是否允许为整数型布尔值`0`和`1`
        LSLIB_API const bool is_bool(const string& str, bool numeric = true);

        /// @brief 转换字符串为整型
        /// @details 非整型字符串将转换失败并返回0
        LSLIB_API const int to_int(lpcstr str);

        /// @brief 转换字符串为整型
        /// @details 非整型字符串将转换失败并返回0
        LSLIB_API const int to_int(const string& str);

        /// @brief 转换字符串为64位整型
        /// @details 非整型字符串将转换失败并返回0
        LSLIB_API const lint64 to_int64(lpcstr str);

        /// @brief 转换字符串为64位整型
        /// @details 非整型字符串将转换失败并返回0
        LSLIB_API const lint64 to_int64(const string& str);

        /// @brief 转换字符串为浮点型
        /// @details 非浮点型字符串将转换失败并返回0
        LSLIB_API const double to_float(lpcstr str);

        /// @brief 转换字符串为浮点型
        /// @details 非浮点型字符串将转换失败并返回0
        LSLIB_API const double to_float(const string& str);

        /// @brief 转换字符串为布尔型
        /// @details 非布尔型字符串将转换失败并返回false
        LSLIB_API const bool to_bool(lpcstr str);

        /// @brief 转换字符串为布尔型
        /// @details 非布尔型字符串将转换失败并返回false
        LSLIB_API const bool to_bool(const string& str);

        /// 转换整型为字符串
        LSLIB_API string from_int(int value);

        /// 转换64位整型为字符串
        LSLIB_API string from_int64(lint64 value);

        /// 转换浮点型型为字符串
        LSLIB_API string from_float(double value, int bit);

        /// 转换布尔型为字符串
        LSLIB_API string from_bool(bool value);

        /// 将字符串转换为小写
        LSLIB_API string lower(lpcstr str);

        /// 将字符串转换为小写
        LSLIB_API string lower(const string& str);

        /// 将字符串转换为大写
        LSLIB_API string upper(lpcstr str);

        /// 将字符串转换为大写
        LSLIB_API string upper(const string& str);

        /// @brief 将字符串转换为大写
        /// @details 空白字符参考 ::is_space 说明
        LSLIB_API string trim(lpcstr str);

        /// @brief 去除字符串前后的空白字符
        /// @details 空白字符参考 ::is_space 说明
        LSLIB_API string trim(const string& str);

        /// @brief 去除字符串开头的空白字符
        /// @details 空白字符参考 ::is_space 说明
        LSLIB_API string trim_left(lpcstr str);

        /// @brief 去除字符串开头的空白字符
        /// @details 空白字符参考 ::is_space 说明
        LSLIB_API string trim_left(const string& str);

        /// @brief 去除字符串结尾的空白字符
        /// @details 空白字符参考 ::is_space 说明
        LSLIB_API string trim_right(lpcstr str);

        /// @brief 去除字符串结尾的空白字符
        /// @details 空白字符参考 ::is_space 说明
        LSLIB_API string trim_right(const string& str);

        /// @brief 去除字符串前后指定的字符
        /// @param str 待操作字符串
        /// @param c 待去除的字符
        LSLIB_API string trim(lpcstr str, lchar c);

        /// @brief 去除字符串前后指定的字符
        /// @param str 待操作字符串
        /// @param c 待去除的字符
        LSLIB_API string trim(const string& str, lchar c);

        /// @brief 去除字符串前后指定的字符串
        /// @param str 待操作字符串
        /// @param trm 待去除的字符串
        LSLIB_API string trim(lpcstr str, lpcstr trm);

        /// @brief 去除字符串前后指定的字符串
        /// @param str 待操作字符串
        /// @param trm 待去除的字符串
        LSLIB_API string trim(const string& str, const string& trm);

        /// @brief 去除字符串开头指定的字符
        /// @param str 待操作字符串
        /// @param c 待去除的字符
        LSLIB_API string trim_left(lpcstr str, lchar c);

        /// @brief 去除字符串开头指定的字符
        /// @param str 待操作字符串
        /// @param c 待去除的字符
        LSLIB_API string trim_left(const string& str, lchar c);

        /// @brief 去除字符串开头指定的字符串
        /// @param str 待操作字符串
        /// @param trm 待去除的字符串
        LSLIB_API string trim_left(lpcstr str, lpcstr trm);

        /// @brief 去除字符串开头指定的字符串
        /// @param str 待操作字符串
        /// @param trm 待去除的字符串
        LSLIB_API string trim_left(const string& str, const string& trm);

        /// @brief 去除字符串结尾指定的字符
        /// @param str 待操作字符串
        /// @param c 待去除的字符
        LSLIB_API string trim_right(lpcstr str, lchar c);

        /// @brief 去除字符串结尾指定的字符
        /// @param str 待操作字符串
        /// @param c 待去除的字符
        LSLIB_API string trim_right(const string& str, lchar c);

        /// @brief 去除字符串结尾指定的字符串
        /// @param str 待操作字符串
        /// @param trm 待去除的字符串
        LSLIB_API string trim_right(lpcstr str, lpcstr trm);

        /// @brief 去除字符串结尾指定的字符串
        /// @param str 待操作字符串
        /// @param trm 待去除的字符串
        LSLIB_API string trim_right(const string& str, const string& trm);

        /// @brief 字符串替换
        /// @param str 待操作字符串
        /// @param needle 需要查找的字符
        /// @param replacement 需要替换成的字符
        /// @param index 开始替换的位置索引，默认从第一个字符开始
        /// @param counts 开始替换的位置索引，默认值-1表示全部替换
        /// @return 返回替换后的字符串
        LSLIB_API string replace(lpcstr str, lchar needle, lchar replacement, size_t index = 0, int counts = -1);

        /// @brief 字符串替换
        /// @param str 待操作字符串
        /// @param needle 需要查找的字符
        /// @param replacement 需要替换成的字符
        /// @param index 开始替换的位置索引，默认从第一个字符开始
        /// @param counts 开始替换的位置索引，默认值-1表示全部替换
        /// @return 返回替换后的字符串
        LSLIB_API string replace(const string& str, lchar needle, lchar replacement, size_t index = 0, int counts = -1);

        /// @brief 字符串替换
        /// @param str 待操作字符串
        /// @param needle 需要查找的字符串
        /// @param replacement 需要替换成的字符串
        /// @param index 开始替换的位置索引，默认从第一个字符开始
        /// @param counts 开始替换的位置索引，默认值-1表示全部替换
        /// @return 返回替换后的字符串
        LSLIB_API string replace(lpcstr str, lpcstr needle, lpcstr replacement, size_t index = 0, int counts = -1);

        /// @brief 字符串替换
        /// @param str 待操作字符串
        /// @param needle 需要查找的字符串
        /// @param replacement 需要替换成的字符串
        /// @param index 开始替换的位置索引，默认从第一个字符开始
        /// @param counts 开始替换的位置索引，默认值-1表示全部替换
        /// @return 返回替换后的字符串
        LSLIB_API string replace(const string& str, const string& needle, const string& replacement, size_t index = 0, int counts = -1);

        /// @brief 格式化字符串
        /// @param pfmt 将要格式化成的字符串格式
        /// @param args 可变参列表，与pfmt格式一一对应
        LSLIB_API string format_args(lpcstr pfmt, const va_list& args);

        /// @brief 格式化字符串
        /// @param pfmt 将要格式化成的字符串格式
        /// @param ...  可变参数，与pfmt格式一一对应
        LSLIB_API string format(lpcstr pfmt, ...);

        /// @brief 追加格式化字符串
        /// @param str  需要追加的源字符串
        /// @param pfmt 要追加的字符串格式
        /// @param ...  可变参数，与pfmt格式一一对应
        /// @note 返回值为第一个参数的副本
        LSLIB_API string append_format(__out__ string& str, lpcstr pfmt, ...);

        /// @brief 分割字符串
        /// @param[out] dest        保存分割后的分组数据
        /// @param[in] src          待分割的源字符串
        /// @param[in] patten       分割符
        /// @param[in] allow_empty  分割后是否保存空的分组
        /// @note 返回值分割后的分组长度
        LSLIB_API const int split(__out__ string_array& dest, lpcstr src, lpcstr patten, bool allow_empty = true);

        /// @brief 分割字符串
        /// @param[out] dest        保存分割后的分组数据
        /// @param[in] src          待分割的源字符串
        /// @param[in] patten       分割符
        /// @param[in] allow_empty  分割后是否保存空的分组
        /// @note 返回值分割后的分组长度
        LSLIB_API const int split(__out__ string_list &dest, lpcstr src, lpcstr patten, bool allow_empty = true);

        /// @brief 分割字符串
        /// @param[out] dest        保存分割后的分组数据
        /// @param[in] src          待分割的源字符串
        /// @param[in] patten       分割符
        /// @param[in] allow_empty  分割后是否保存空的分组
        /// @return 返回值分割后的分组长度
        LSLIB_API const int split(__out__ string_array& dest, const string& src, const string& patten, bool allow_empty = true);

        /// @brief 分割字符串
        /// @param[out] dest        保存分割后的分组数据
        /// @param[in] src          待分割的源字符串
        /// @param[in] patten       分割符
        /// @param[in] allow_empty  分割后是否保存空的分组
        /// @return 返回值分割后的分组长度
        LSLIB_API const int split(__out__ string_list& dest, const string& src, const string& patten, bool allow_empty = true);

        /// @brief 将一个16进制字符转为字节
        /// @details 如 'a' -> 0xa
        LSLIB_API lbyte hex_char_to_byte(lchar c);

        /// @brief 将一个字节转为16进制字符
        /// @details 如 0xa -> 'a'
        LSLIB_API lchar byte_to_hex_char(lbyte b);

        /// @brief 将16进制字符串转为字节数组
        /// @details 如 "da3fed134a" -> { 0xda, 0x3f, 0xed, 0x13, 0x4a }
        LSLIB_API lbyte_array hex_str_to_byte_array(lpcstr data);

        /// @brief 将16进制字符串转为字节数组
        /// @details 如 "da3fed134a" -> { 0xda, 0x3f, 0xed, 0x13, 0x4a }
        LSLIB_API lbyte_array hex_str_to_byte_array(const string& data);

        /// @brief 将字节数组转为16进制字符串
        /// @details 如 { 0xda, 0x3f, 0xed, 0x13, 0x4a } -> "da3fed134a"
        LSLIB_API string byte_array_to_hex_str(lbyte_array data);

        /// @brief 将字节数组转为16进制字符串
        /// @details 如 { 0xda, 0x3f, 0xed, 0x13, 0x4a } -> "da3fed134a"
        LSLIB_API string byte_array_to_hex_str(lbyte data[], size_t len);

    }
}
