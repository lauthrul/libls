#pragma once

namespace lslib
{
    /// 配置类型
    enum ECfgValueType { CVT_STRING = 0, CVT_INT = 1, CVT_FLOAT = 2, CVT_BLOB = 3 };
    declare_enum_str(ECfgValueType, LSLIB_API);

    /// 配置操作相关错误码
    enum ECfgErr
    {
        INVALID_PARAM = -100,
        OPEN_FILE_FAIL,
        READ_FILE_FAIL,
        WRITE_FILE_FAIL,
        MODULE_NOT_EXIST,
        KEY_NOT_EXIST,
        INVALID_MODULE,
        INVALID_KEY,
        CCFGHANDLER_ERROR,
        FILE_NOT_EXIST,
    };

    /// 配置项数据结构
    struct SCfgData
    {
        string          strModule;
        string          strKey;
        ECfgValueType   eValueType;
        string          v_str;
        int             v_int;
        float           v_float;
        string          v_blob;
        int             v_bloblen;
    };
    quick_define_stl(SCfgData);

    /// @brief 配置文件相关操作
    class CCfgHandler
    {
    public:
        /// 获取文本类型配置项
        static LSLIB_API string GetCfgText(lpcstr pModule, lpcstr pKey, lpcstr pDefault, lpcstr pFile);
        
        /// 获取整型配置项
        static LSLIB_API int GetCfgInt(lpcstr pModule, lpcstr pKey, int nDefault, lpcstr pFile);

        /// 获取浮点数配置项
        static LSLIB_API float GetCfgFloat(lpcstr pModule, lpcstr pKey, float fDefault, lpcstr pFile);

        /// @brief 获取二进制配置项
        /// @return 成功返回配置项字节长度，失败返回-1
        /// @note 使用后需要释放掉pOut所占内存
        static LSLIB_API int GetCfgBlob(__out__ lpvoid& pOut, lpcstr pModule, lpcstr pKey, lpcstr pFile);

        /// 设置文本类型配置项
        static LSLIB_API int SetCfg(lpcstr pModule, lpcstr pKey, lpcstr value, lpcstr pFile);

        /// 设置整型配置项
        static LSLIB_API int SetCfg(lpcstr pModule, lpcstr pKey, int value, lpcstr pFile);

        /// 设置浮点型配置项
        static LSLIB_API int SetCfg(lpcstr pModule, lpcstr pKey, float value, lpcstr pFile);

        /// 设置二进制配置项
        static LSLIB_API int SetCfg(lpcstr pModule, lpcstr pKey, lpvoid pValue, int dwValueLen, lpcstr pFile);

        /// @brief 加载配置文件
        /// @param[out] lst 加载的配置项列表
        /// @param[in] pFile 配置文件路径
        /// @return 成功返回0，失败返回错误码，参考@ref ECfgErr
        static LSLIB_API int LoadCfg(__out__ SCfgData_list& lst, lpcstr  pFile);
    };
}
