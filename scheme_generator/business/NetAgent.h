#pragma once
#include "TaskAgent.h"

//////////////////////////////////////////////////////////////////////////
class CNetManager;
class CNetAgent : public CTaskAgent
{
public:
    CNetAgent(CNetManager* pManager);
    virtual ~CNetAgent();

public:
    // implement from CThread
    virtual LPCTSTR                 GetName() { return "CNetAgent"; };

    // implement from CTaskAgent
    virtual void                    InitInvoker();

    struct SResultChecker
    {
        __in lstring                strCodeField;
        __in int                    nSuccessCode;
        __in lstring                strMsgField;
        __out lstring               strErrMsg;
        __out CJsonValue            jRoot;
    };
    virtual int                     CheckResult(int nHttpCode, _lpcstr lpstrResultText, __out SResultChecker& checker);

public:
    int                             DoGetLotteryCfg(_ldword pParam, _ldword& pResult);
    int                             DoGetHistoryCode(_ldword pParam, _ldword& pResult);
    int                             DoGetIssueInfo(_ldword pParam, _ldword& pResult);
};
