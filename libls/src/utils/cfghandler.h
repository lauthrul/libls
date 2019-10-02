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
    define_stl_type(SCfgData);

    /// @brief �����ļ���ز���
    class CCfgHandler
    {
    public:
        /// ��ȡ�ı�����������
        static LSLIB_API string GetCfgText(lpcstr pModule, lpcstr pKey, lpcstr pDefault, lpcstr pFile);
        
        /// ��ȡ����������
        static LSLIB_API int GetCfgInt(lpcstr pModule, lpcstr pKey, int nDefault, lpcstr pFile);

        /// ��ȡ������������
        static LSLIB_API float GetCfgFloat(lpcstr pModule, lpcstr pKey, float fDefault, lpcstr pFile);

        /// @brief ��ȡ������������
        /// @return �ɹ������������ֽڳ��ȣ�ʧ�ܷ���-1
        /// @note ʹ�ú���Ҫ�ͷŵ�pOut��ռ�ڴ�
        static LSLIB_API int GetCfgBlob(__out__ lpvoid& pOut, lpcstr pModule, lpcstr pKey, lpcstr pFile);

        /// �����ı�����������
        static LSLIB_API int SetCfg(lpcstr pModule, lpcstr pKey, lpcstr value, lpcstr pFile);

        /// ��������������
        static LSLIB_API int SetCfg(lpcstr pModule, lpcstr pKey, int value, lpcstr pFile);

        /// ���ø�����������
        static LSLIB_API int SetCfg(lpcstr pModule, lpcstr pKey, float value, lpcstr pFile);

        /// ���ö�����������
        static LSLIB_API int SetCfg(lpcstr pModule, lpcstr pKey, lpvoid pValue, int dwValueLen, lpcstr pFile);

        /// @brief ���������ļ�
        /// @param[out] lst ���ص��������б�
        /// @param[in] pFile �����ļ�·��
        /// @return �ɹ�����0��ʧ�ܷ��ش����룬�ο�@ref ECfgErr
        static LSLIB_API int LoadCfg(__out__ SCfgData_list& lst, lpcstr  pFile);
    };
}
