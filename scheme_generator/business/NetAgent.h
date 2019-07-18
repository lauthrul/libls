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
    virtual const char*             GetName() { return "CNetAgent"; };

    // implement from CTaskAgent
    virtual void                    InitInvoker();

    struct SResultChecker
    {
        __in__ string              strCodeField;
        __in__ int                  nSuccessCode;
        __in__ string              strMsgField;
        __out__ string             strErrMsg;
        __out__ CJsonValue          jRoot;
    };
    virtual int                     CheckResult(int nHttpCode, _lpcstr lpstrResultText, __out__ SResultChecker& checker);

public:
    int                             DoGetLotteryCfg(_ldword pParam, _ldword& pResult);
    int                             DoGetHistoryCode(_ldword pParam, _ldword& pResult);
    int                             DoGetIssueInfo(_ldword pParam, _ldword& pResult);
};
