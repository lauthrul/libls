/************************************************************************/
/* desc: Business Manager                                               */
/* autor: neco                                                          */
/* date: 2018.02.06                                                     */
/************************************************************************/

#pragma once
#include "TaskManager.h"
#include "NetAgent.h"
#include "NetDefine.h"

//////////////////////////////////////////////////////////////////////////
class CNetManager : public CTaskManager
{
public :
    CNetManager();
    virtual ~CNetManager();

    // common functions
public:
    virtual _lpcstr                     GetName() { return "CNetManager"; };

public:
    int                                 GetLotteryCfgRequest();
    int                                 GetHistoryCodeRequest(_lpcstr lpstrLottery, int nIssues = 30);
    int                                 GetIssueInfoRequest(_lpcstr lpstrLottery);

protected:
    virtual void                        InitNetAgent(int nAgentCounts);
    virtual void                        ReleaseNetAgent();
    virtual bool                        IsSameTask(STask* plhs, STask* prhs);
    virtual int                         OnHandleMessage(STask* pTask, __inout bool& bRecycle, __inout bool& bNotice);
};
