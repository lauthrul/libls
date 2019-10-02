#include "stdafx.h"
#include "xml.h"

namespace lslib
{

    Xml::Xml() : m_xmlRoot(NULL)
    {
    }

    Xml::~Xml()
    {
    }

    bool Xml::Parse(lpcstr lpstr, TiXmlEncoding encoding /*= TIXML_DEFAULT_ENCODING*/)
    {
        m_xmlDoc.Parse(lpstr, 0, encoding);
        if (m_xmlDoc.ErrorId() != 0) return false;
        m_xmlRoot = m_xmlDoc.RootElement();
        return true;
    }

    bool Xml::ParseFile(lpcstr lpstrFile, TiXmlEncoding encoding /*= TIXML_DEFAULT_ENCODING*/)
    {
        m_xmlDoc.LoadFile(lpstrFile, encoding);
        if (m_xmlDoc.ErrorId() != 0) return false;
        m_xmlRoot = m_xmlDoc.RootElement();
        return true;
    }

    XmlNode* Xml::GetNode(XmlNode* pParent, lpcstr lpstrPath)
    {
        if (pParent == NULL) return NULL;

        string_array arr;
        string strPath = strtool::replace(lpstrPath, '\\', '/');
        strtool::split(arr, strPath, "/");
        if (arr.empty()) return pParent;

        XmlNode* pNode = pParent;
        for (size_t i = 0; i < arr.size(); i++)
        {
            if (arr[i].empty()) continue;

            pNode = GetChildNode(pNode, arr[i].c_str());
            if (pNode == NULL) break;
        }
        return pNode;
    }

    XmlNode* Xml::GetChildNode(XmlNode* pParent, lpcstr lpKeyOrIndex)
    {
        if (pParent == NULL) return NULL;

        int nIndex = 0;
        XmlNode* pNode = pParent->FirstChildElement();
        for (; pNode != NULL; pNode = pNode->NextSiblingElement(), nIndex++)
        {
            if (pNode->Value() == lpKeyOrIndex || nIndex == *(int*)&lpKeyOrIndex)
                return pNode;
        }
        return NULL;
    }

    XmlNode* Xml::GetSublingChildNode(XmlNode* pParent, int nIndex, lpcstr lpKey)
    {
        if (pParent == NULL) return NULL;

        int idx = 0;
        XmlNode* pNode = pParent->FirstChildElement();
        for (; pNode != NULL; pNode = pNode->NextSiblingElement(), idx++)
        {
            if (nIndex == idx) return GetChildNode(pNode, lpKey);
        }
        return NULL;
    }

    string Xml::GetAttribute(const XmlNode& node, lpcstr lpstrAttri)
    {
        const char* attr = node.Attribute(lpstrAttri);
        return attr != NULL ? attr : "";
    }

    string Xml::GetValue(const XmlNode& node)
    {
        const XmlNode* pNode =  node.FirstChildElement();
        if (pNode == NULL) return node.Value();
        else return Dumps(*pNode);
    }

    string Xml::Dumps(const XmlNode& node, bool bPretty /*= false*/)
    {
        return "";
    }
}
