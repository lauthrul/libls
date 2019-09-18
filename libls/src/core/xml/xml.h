#ifndef XML_H
#define XML_H

#pragma once

#include "tinyxml.h"

namespace lslib
{
    typedef TiXmlElement XmlNode;

    /// @brief XML相关操作
    class LSLIB_API Xml
    {
    public:
        Xml();
        ~Xml();

    public:
        /// 解析字符串
        bool Parse(_lpcstr lpstr);

        /// 解析文件
        bool ParseFile(_lpcstr lpstrFile);

        /// @brief 获取节点
        /// @param pParent      开始获取的父节点
        /// @param lpstrPath    节点路径，以"/"分割开来
        /// 如 @code
        /// GetNode(pRoot, "module/section/key"); // 获取pRoot节点下的"module/section/key"子节点
        /// GetNode(pSection, "key"); // 获取pSection节点下的"key"子节点
        /// @endcode
        /// @return 成功返回获取到的子节点，失败返回NULL
        static XmlNode* GetNode(XmlNode* pParent, _lpcstr lpstrPath); // path separated by "/", like "module/section/key"

        /// @brief 获取直接子节点
        /// @param pParent      开始获取的父节点
        /// @param lpKeyOrIndex 子节点key值（父节点为objectValue类型）或索引号（父节点为arrayValue类型）
        /// @return 成功返回获取到的子节点，失败返回NULL
        static XmlNode* GetChildNode(XmlNode* pParent, _lpcstr lpKeyOrIndex); // get direct child node by key(objectValue) or index(arrayValue)


        /// @brief 获取兄弟直接子节点（针对父节点为arrayValue类型）
        /// @param pParent      开始获取的父节点
        /// @param nIndex       数组索引
        /// @param lpKey        子节点key值
        /// @return 成功返回获取到的子节点，失败返回NULL
        static XmlNode* GetSublingChildNode(XmlNode* pParent, int nIndex, _lpcstr lpKey); // for arrayValue, get direct child node by target key from the specified index array node

        /// @brief 获取节点属性
        /// @param node 需要获取属性的节点
        /// @param lpstrAttri 属性名称
        /// @return 返回获取到的子节点，失败返回空字符串
        static string GetAttribute(const XmlNode& node, _lpcstr lpstrAttri);

        /// @brief 获取节点内容字符串（包含子节点）
        /// @param node 需要获取内容字符串的节点
        static string GetValue(const XmlNode& node);

        /// @brief 获取节点字符串
        /// @param node 需要获取内容字符串的节点
        /// @param bPretty 是否美化（格式化）内容
        /// @return 返回节点字符串
        static string Dumps(const XmlNode& node, bool bPretty = false);

    public:
        TiXmlDocument   m_xmlDoc;
        XmlNode*        m_xmlRoot;
    };
}


#endif //XML_H
