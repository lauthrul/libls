#ifndef XML_H
#define XML_H

#pragma once

#include "tinyxml.h"

namespace lslib
{
    typedef TiXmlElement XmlNode;

    /// @brief XML��ز���
    class LSLIB_API Xml
    {
    public:
        Xml();
        ~Xml();

    public:
        /// �����ַ���
        bool Parse(_lpcstr lpstr);

        /// �����ļ�
        bool ParseFile(_lpcstr lpstrFile);

        /// @brief ��ȡ�ڵ�
        /// @param pParent      ��ʼ��ȡ�ĸ��ڵ�
        /// @param lpstrPath    �ڵ�·������"/"�ָ��
        /// �� @code
        /// GetNode(pRoot, "module/section/key"); // ��ȡpRoot�ڵ��µ�"module/section/key"�ӽڵ�
        /// GetNode(pSection, "key"); // ��ȡpSection�ڵ��µ�"key"�ӽڵ�
        /// @endcode
        /// @return �ɹ����ػ�ȡ�����ӽڵ㣬ʧ�ܷ���NULL
        static XmlNode* GetNode(XmlNode* pParent, _lpcstr lpstrPath); // path separated by "/", like "module/section/key"

        /// @brief ��ȡֱ���ӽڵ�
        /// @param pParent      ��ʼ��ȡ�ĸ��ڵ�
        /// @param lpKeyOrIndex �ӽڵ�keyֵ�����ڵ�ΪobjectValue���ͣ��������ţ����ڵ�ΪarrayValue���ͣ�
        /// @return �ɹ����ػ�ȡ�����ӽڵ㣬ʧ�ܷ���NULL
        static XmlNode* GetChildNode(XmlNode* pParent, _lpcstr lpKeyOrIndex); // get direct child node by key(objectValue) or index(arrayValue)


        /// @brief ��ȡ�ֵ�ֱ���ӽڵ㣨��Ը��ڵ�ΪarrayValue���ͣ�
        /// @param pParent      ��ʼ��ȡ�ĸ��ڵ�
        /// @param nIndex       ��������
        /// @param lpKey        �ӽڵ�keyֵ
        /// @return �ɹ����ػ�ȡ�����ӽڵ㣬ʧ�ܷ���NULL
        static XmlNode* GetSublingChildNode(XmlNode* pParent, int nIndex, _lpcstr lpKey); // for arrayValue, get direct child node by target key from the specified index array node

        /// @brief ��ȡ�ڵ�����
        /// @param node ��Ҫ��ȡ���ԵĽڵ�
        /// @param lpstrAttri ��������
        /// @return ���ػ�ȡ�����ӽڵ㣬ʧ�ܷ��ؿ��ַ���
        static string GetAttribute(const XmlNode& node, _lpcstr lpstrAttri);

        /// @brief ��ȡ�ڵ������ַ����������ӽڵ㣩
        /// @param node ��Ҫ��ȡ�����ַ����Ľڵ�
        static string GetValue(const XmlNode& node);

        /// @brief ��ȡ�ڵ��ַ���
        /// @param node ��Ҫ��ȡ�����ַ����Ľڵ�
        /// @param bPretty �Ƿ���������ʽ��������
        /// @return ���ؽڵ��ַ���
        static string Dumps(const XmlNode& node, bool bPretty = false);

    public:
        TiXmlDocument   m_xmlDoc;
        XmlNode*        m_xmlRoot;
    };
}


#endif //XML_H
