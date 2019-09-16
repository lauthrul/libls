#pragma once

#include "json.h"

namespace lslib
{
    //////////////////////////////////////////////////////////////////////////
    typedef Json::Value         CJsonValue;

    //////////////////////////////////////////////////////////////////////////
    /// @brief json相关操作API
    class LSLIB_API  CJsonWrapper
    {
    public:
        /// 解析字符串
        static bool             Parse(_lpcstr lpstrJson, __out__ CJsonValue* pRoot);

        /// 解析文件
        static bool             ParseFile(_lpcstr lpstrFile, __out__ CJsonValue* pRoot);

        /// @brief 获取节点
        /// @param pParent      开始获取的父节点
        /// @param lpstrPath    节点路径，以"/"分割开来
        /// 如 @code
        /// GetNode(pRoot, "module/section/key"); // 获取pRoot节点下的"module/section/key"子节点
        /// GetNode(pSection, "key"); // 获取pSection节点下的"key"子节点
        /// @endcode
        /// @return 成功返回获取到的子节点，失败返回NULL
        static CJsonValue*      GetNode(CJsonValue* pParent, _lpcstr lpstrPath); // path separated by "/", like "module/section/key"

        /// @brief 获取直接子节点
        /// @param pParent      开始获取的父节点
        /// @param lpKeyOrIndex 子节点key值（父节点为对象类型）或索引号（父节点为数组类型）
        /// @return 成功返回获取到的子节点，失败返回NULL
        static CJsonValue*      GetChildNode(CJsonValue* pParent, _lpcstr lpKeyOrIndex); // get direct child node by key(objectValue) or index(arrayValue)

        /// @brief 获取兄弟直接子节点（针对父节点为数组类型）
        /// @param pParent      开始获取的父节点
        /// @param nIndex       数组索引
        /// @param lpKey        子节点key值
        /// @return 成功返回获取到的子节点，失败返回NULL
        static CJsonValue*      GetSublingChildNode(CJsonValue* pParent, int nIndex, _lpcstr lpKey); // for arrayValue, get direct child node by target key from the specified index array node

        /// @brief 获取节点字符串
        /// @param obj 需要获取内容字符串的节点
        /// @param bPretty 是否美化（格式化）内容
        /// @return 返回节点字符串
        static string           Dumps(const CJsonValue& obj, bool bPretty = false);
    };
}
