#pragma once

namespace lslib
{
    // config value type
    enum ECfgValueType { CVT_STRING = 0, CVT_INT = 1, CVT_FLOAT = 2 };
    declare_enum_str(ECfgValueType, LSLIB_API);

    // config api error codes
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

    // config item structure
    struct SCfgData
    {
        string          strModule;
        string          strKey;
        ECfgValueType   eValueType;
        string          v_str;
        int             v_int;
        float           v_float;
    };
    declare_stl_obj(SCfgData);

    class CCfgHandler
    {
    public:
        static LSLIB_API string GetCfgText(_lpcstr pModule, _lpcstr pKey, _lpcstr pDefault, _lpcstr pFile);
        static LSLIB_API int GetCfgInt(_lpcstr pModule, _lpcstr pKey, int nDefault, _lpcstr pFile);
        static LSLIB_API float GetCfgFloat(_lpcstr pModule, _lpcstr pKey, float fDefault, _lpcstr pFile);

        static LSLIB_API int SetCfg(_lpcstr pModule, _lpcstr pKey, int nValueType, _lpvoid pValue, _lint dwValueLen, _lpcstr pFile);
        static LSLIB_API int SetCfg(_lpcstr pModule, _lpcstr pKey, _lpcstr value, _lpcstr pFile);
        static LSLIB_API int SetCfg(_lpcstr pModule, _lpcstr pKey, int value, _lpcstr pFile);
        static LSLIB_API int SetCfg(_lpcstr pModule, _lpcstr pKey, float value, _lpcstr pFile);

        static LSLIB_API int LoadCfg(__out__ SCfgData_list& lst, _lpcstr  pFile);
    };
}
