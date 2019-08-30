#pragma once

#include "json.h"

namespace lslib
{
    //////////////////////////////////////////////////////////////////////////
    typedef Json::Value         CJsonValue;

    //////////////////////////////////////////////////////////////////////////
    class LSLIB_API  CJsonWrapper
    {
    public:
        static bool             Parse(_lpcstr lpstrJson, __out__ CJsonValue* pRoot);
        static bool             ParseFile(_lpcstr lpstrFile, __out__ CJsonValue* pRoot);
        static CJsonValue*      GetNode(CJsonValue* pParent, _lpcstr lpstrPath); // path separated by "/", like "module/section/key"
        static CJsonValue*      GetChildNode(CJsonValue* pParent, _lpcstr lpKeyOrIndex); // get direct child node by key(objectValue) or index(arrayValue)
        static CJsonValue*      GetSublingChildNode(CJsonValue* pParent, _lpcstr lpKey, int nIndex); // for arrayValue, get direct child node by target key and array index
        static string           Dumps(const CJsonValue& obj, bool bPretty = false);
    };
}
