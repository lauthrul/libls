#pragma once

namespace lslib
{
    extern const int FLAG_MODULE_BEGIN;
    extern const int FLAG_ITEM_BEGIN;
    extern const int FLAG_ITEM_END;
    extern const int FLAG_MODULE_END;
    extern const int FLAG_LENGTH;

    enum EValueType { VAT_STRING = 0, VAT_INT = 1, VAT_FLOAT = 2 };
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

    class CCfgHandler
    {
    public:
        static LSLIB_API string GetCfgText(_lpcstr pModule, _lpcstr pKey, _lpcstr pDefault, _lpcstr pFile);
        static LSLIB_API int GetCfgInt(_lpcstr pModule, _lpcstr pKey, int nDefault, _lpcstr pFile);
        static LSLIB_API float GetCfgFloat(_lpcstr pModule, _lpcstr pKey, float fDefault, _lpcstr pFile);

        static LSLIB_API int SetCfg(_lpcstr pModule, _lpcstr pKey, int nValueType, _lpvoid pValue, _ldword dwValueLen, _lpcstr pFile);
        static LSLIB_API int SetCfg(_lpcstr pModule, _lpcstr pKey, _lpcstr value, _lpcstr pFile);
        static LSLIB_API int SetCfg(_lpcstr pModule, _lpcstr pKey, int value, _lpcstr pFile);
        static LSLIB_API int SetCfg(_lpcstr pModule, _lpcstr pKey, float value, _lpcstr pFile);

    public:
        struct tagPointer
        {
            _lpustr pModuleStart;
            _lpustr pKeyStart;
            _lpustr pKeyEnd;
            _lpustr pModuleEnd;

            tagPointer() { memset(this, 0, sizeof(tagPointer)); }
        };
        static int GetHandler(_lpcstr pFile, FILE*& hFile, _lpustr* pOutBuf, _lpdword dwBufLen = NULL);
        static bool CloseHandler(FILE* hFile, _lpustr pBuffer);
        static int GetPointer(_lpcstr pModule, _lpcstr pKey, _lpustr pBuffer, _ldword dwBufLen, tagPointer& pt);

    private:
        static CMutexLock m_mtxLock;
    };
}
