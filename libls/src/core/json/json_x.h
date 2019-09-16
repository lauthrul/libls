#pragma once

#include "json.h"

namespace lslib
{
    //////////////////////////////////////////////////////////////////////////
    typedef Json::Value         CJsonValue;

    //////////////////////////////////////////////////////////////////////////
    /// @brief json��ز���API
    class LSLIB_API  CJsonWrapper
    {
    public:
        /// �����ַ���
        static bool             Parse(_lpcstr lpstrJson, __out__ CJsonValue* pRoot);

        /// �����ļ�
        static bool             ParseFile(_lpcstr lpstrFile, __out__ CJsonValue* pRoot);

        /// @brief ��ȡ�ڵ�
        /// @param pParent      ��ʼ��ȡ�ĸ��ڵ�
        /// @param lpstrPath    �ڵ�·������"/"�ָ��
        /// �� @code
        /// GetNode(pRoot, "module/section/key"); // ��ȡpRoot�ڵ��µ�"module/section/key"�ӽڵ�
        /// GetNode(pSection, "key"); // ��ȡpSection�ڵ��µ�"key"�ӽڵ�
        /// @endcode
        /// @return �ɹ����ػ�ȡ�����ӽڵ㣬ʧ�ܷ���NULL
        static CJsonValue*      GetNode(CJsonValue* pParent, _lpcstr lpstrPath); // path separated by "/", like "module/section/key"

        /// @brief ��ȡֱ���ӽڵ�
        /// @param pParent      ��ʼ��ȡ�ĸ��ڵ�
        /// @param lpKeyOrIndex �ӽڵ�keyֵ�����ڵ�Ϊ�������ͣ��������ţ����ڵ�Ϊ�������ͣ�
        /// @return �ɹ����ػ�ȡ�����ӽڵ㣬ʧ�ܷ���NULL
        static CJsonValue*      GetChildNode(CJsonValue* pParent, _lpcstr lpKeyOrIndex); // get direct child node by key(objectValue) or index(arrayValue)

        /// @brief ��ȡ�ֵ�ֱ���ӽڵ㣨��Ը��ڵ�Ϊ�������ͣ�
        /// @param pParent      ��ʼ��ȡ�ĸ��ڵ�
        /// @param nIndex       ��������
        /// @param lpKey        �ӽڵ�keyֵ
        /// @return �ɹ����ػ�ȡ�����ӽڵ㣬ʧ�ܷ���NULL
        static CJsonValue*      GetSublingChildNode(CJsonValue* pParent, int nIndex, _lpcstr lpKey); // for arrayValue, get direct child node by target key from the specified index array node

        /// @brief ��ȡ�ڵ��ַ���
        /// @param obj ��Ҫ��ȡ�����ַ����Ľڵ�
        /// @param bPretty �Ƿ���������ʽ��������
        /// @return ���ؽڵ��ַ���
        static string           Dumps(const CJsonValue& obj, bool bPretty = false);
    };
}
