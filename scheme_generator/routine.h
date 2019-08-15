#pragma once

#include "db/db_wrapper.h"

//////////////////////////////////////////////////////////////////////////
class CRoutine : public CThread
{
public :
    CRoutine();
    virtual ~CRoutine();

public:
    virtual _lpcstr GetName() { return "CRoutine"; };
    virtual void Run();
    virtual void OnExecute();
    virtual int HandleMessage(msgid_t uMsg, wparam_t wParam = 0, lparam_t lParam = 0);

protected:
    void        Routine();
    void        MQTTSchemeNotify(const list<SubScheme>& lstSubSchemeUpdated, _lpcstr issue);

protected:
    map<int, Scheme>                    m_mapSchemes; // <scheme id, Scheme>
    map<int, SubScheme>                 m_mapSubSchemes; // <sub scheme id, SubSCheme>
    MapAllHistoryCode                   m_mapAllHistoryCode;
};
