#include "stdafx.h"
#include "json_x.h"

//////////////////////////////////////////////////////////////////////////
namespace lslib
{
    bool CJsonWrapper::Parse(_lpcstr lpstrJson, __out__ CJsonValue* pRoot)
    {
        if (lpstrJson == NULL)
            return false;
        Json::Reader jReader;
        CJsonValue jRoot;
        if (!jReader.parse(lpstrJson, jRoot, false))
            return false;
        if (jRoot.isNull())
            return false;
        if (pRoot != NULL) *pRoot = jRoot;
        return true;
    }

    bool CJsonWrapper::ParseFile(_lpcstr lpstrFile, __out__ CJsonValue* pRoot)
    {
        _lpbyte buf = os::get_file_buffer(lpstrFile, NULL);
        bool bret = Parse((_lpcstr)buf, pRoot);
        os::release_file_buffer(buf);
        return bret;
    }

    CJsonValue* CJsonWrapper::GetNode(CJsonValue* pParent, _lpcstr lpstrPath)
    {
        if (pParent == NULL) return NULL;

        string_array arrays;
        if (strtool::split(arrays, lpstrPath, "/", false) <= 0)
            strtool::split(arrays, lpstrPath, "\\", false);
        if (arrays.empty()) return pParent;

        CJsonValue* pNode = pParent;
        for (size_t i = 0; i < arrays.size(); i++)
        {
            if (arrays[i].empty()) continue;

            pNode = GetChildNode(pNode, arrays[i].c_str());
            if (pNode == NULL) break;
        }
        return pNode;
    }

    CJsonValue* CJsonWrapper::GetChildNode(CJsonValue* pParent, _lpcstr lpKeyOrIndex)
    {
        if (pParent == NULL) return NULL;

        CJsonValue::iterator it = pParent->begin();
        for (; it != pParent->end(); it++)
        {
            CJsonValue jKey = it.key();
            if (jKey.isString() && jKey.asString() == lpKeyOrIndex)
                return &*it;
            else if (jKey.isInt() && jKey.asInt() == (int)lpKeyOrIndex)
                return &*it;
        }
        return NULL;
    }

    CJsonValue* CJsonWrapper::GetSublingChildNode(CJsonValue* pParent, _lpcstr lpKey, int nIndex)
    {
        if (pParent == NULL) return NULL;

        CJsonValue::iterator it = pParent->begin();
        for (; it != pParent->end(); it++)
        {
            CJsonValue jKey = it.key();
            if (jKey.isInt() && jKey.asInt() == nIndex)
                return GetChildNode(&(*it), lpKey);
        }
        return NULL;
    }

    std::string CJsonWrapper::Dumps(const CJsonValue& obj, bool bPretty /*= false*/)
    {
        if (bPretty)
        {
            Json::StyledWriter jsWriter;
            return jsWriter.write(obj);
        }
        else
        {
            Json::FastWriter jsWriter;
            return jsWriter.write(obj);
        }
    }
} // end of lslib
