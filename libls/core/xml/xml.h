#ifndef XML_H
#define XML_H

#pragma once

#include "tinyxml.h"

namespace lslib
{
    typedef TiXmlElement XmlNode;

    class LSLIB_API Xml
    {
        public:
            Xml();
            ~Xml();

        public:
            bool Parse(_lpcstr lpstr);
            bool ParseFile(_lpcstr lpstrFile);

            static XmlNode* GetNode(XmlNode* pParent, _lpcstr lpstrPath); // path separated by "/", like "module/section/key"
            static XmlNode* GetChildNode(XmlNode* pParent, _lpcstr lpKeyOrIndex); // get direct child node by key(objectValue) or index(arrayValue)
            static XmlNode* GetSublingChildNode(XmlNode* pParent, int nIndex, _lpcstr lpKey); // for arrayValue, get direct child node by target key from the specified index array node
            static lstring GetAttribute(const XmlNode& node, _lpcstr lpstrAttri);
            static lstring GetValue(const XmlNode& node);
            static lstring Dumps(const XmlNode& node, bool bPretty = false);

        public:
            TiXmlDocument   m_xmlDoc;
            XmlNode*        m_xmlRoot;
    };
}


#endif //XML_H
