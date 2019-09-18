#pragma once

namespace lslib
{
    /// ��������
    enum ECfgValueType { CVT_STRING = 0, CVT_INT = 1, CVT_FLOAT = 2, CVT_BLOB = 3 };
    declare_enum_str(ECfgValueType, LSLIB_API);

    /// ���ò�����ش�����
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

    /// ���������ݽṹ
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
    declare_stl_obj(SCfgData);

    /// @brief �����ļ���ز���
    class CCfgHandler
    {
    public:
        /// ��ȡ�ı�����������
        static LSLIB_API string GetCfgText(_lpcstr pModule, _lpcstr pKey, _lpcstr pDefault, _lpcstr pFile);
        
        /// ��ȡ����������
        static LSLIB_API int GetCfgInt(_lpcstr pModule, _lpcstr pKey, int nDefault, _lpcstr pFile);

        /// ��ȡ������������
        static LSLIB_API float GetCfgFloat(_lpcstr pModule, _lpcstr pKey, float fDefault, _lpcstr pFile);

        /// @brief ��ȡ������������
        /// @return �ɹ������������ֽڳ��ȣ�ʧ�ܷ���-1
        /// @note ʹ�ú���Ҫ�ͷŵ�pOut��ռ�ڴ�
        static LSLIB_API int GetCfgBlob(__out__ _lpvoid& pOut, _lpcstr pModule, _lpcstr pKey, _lpcstr pFile);

        /// �����ı�����������
        static LSLIB_API int SetCfg(_lpcstr pModule, _lpcstr pKey, _lpcstr value, _lpcstr pFile);

        /// ��������������
        static LSLIB_API int SetCfg(_lpcstr pModule, _lpcstr pKey, int value, _lpcstr pFile);

        /// ���ø�����������
        static LSLIB_API int SetCfg(_lpcstr pModule, _lpcstr pKey, float value, _lpcstr pFile);

        /// ���ö�����������
        static LSLIB_API int SetCfg(_lpcstr pModule, _lpcstr pKey, _lpvoid pValue, int dwValueLen, _lpcstr pFile);

        /// @brief ���������ļ�
        /// @param[out] lst ���ص��������б�
        /// @param[in] pFile �����ļ�·��
        /// @return �ɹ�����0��ʧ�ܷ��ش����룬�ο�@ref ECfgErr
        static LSLIB_API int LoadCfg(__out__ SCfgData_list& lst, _lpcstr  pFile);
    };
}
