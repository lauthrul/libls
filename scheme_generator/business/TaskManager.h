/************************************************************************/
/* desc: Task Manager                                                   */
/* autor: neco                                                          */
/* date: 2018.02.06                                                     */
/************************************************************************/

#pragma once
#include "TaskDefine.h"
#include "TaskAgent.h"

//////////////////////////////////////////////////////////////////////////
#define DEFAULT_AGENT_COUNT                         3

//////////////////////////////////////////////////////////////////////////
struct STask
{
    int             dwTID;
    CTaskAgent*     pAgent;
    STaskReq*       pReq;
    STaskResp*      pResp;
    bool            bKeepSingle;
    int             nReqID;
    int             nRetryTimes;
    long            lRecycleTime;

    STask()
    { memset(this, 0, sizeof(STask)); }
};

//////////////////////////////////////////////////////////////////////////
class CTaskManager : public CThread
{
public :
    CTaskManager();
    virtual ~CTaskManager();

public:
    virtual _lpcstr                     GetName() { return "CTaskManager"; };

public:
    // common functions
    bool                                RegisterCallBack(CThread* pThread);
    bool                                UnRegisterCallBack(CThread* pThread);

protected:
    virtual void                        InitAgent(int nAgentCounts);
    virtual void                        ReleaseAgent();
    CTaskAgent*                         GetNextAgent();
    virtual int                         Invoke(CTaskAgent* pAgent, int dwTID, STaskReq* pReq = 0, bool bKeepSingle = true);
    bool                                IsTaskExist(STask* pTask);
    virtual bool                        IsSameTask(STask* plhs, STask* prhs) = 0;

    virtual void                        OnExecute();
    virtual void                        DealTask();
    virtual void                        DealRetryTask();
    virtual void                        DealMemClean(bool bForce = false);
    virtual void                        CleanTasks();

    void                                Recycle(STask* pTask);
    void                                Notice(SInvoker* pInvoker, wparam_t wParam = 0, lparam_t lParam = 0);

    virtual int                         HandleCustomMessage(msgid_t uMsg, wparam_t wParam, lparam_t lParam, bool& bHandled);
    virtual int                         HandleMessage(msgid_t uMsg, wparam_t wParam, lparam_t lParam);
    virtual int                         OnHandleMessage(STask* pTask, __inout__ bool& bRecycle, __inout__ bool& bNotice);

protected:
    list<CThread*>                      m_lstCBThreads;             // callback thread handler
    CMutexLock                          m_mtxTask;
    list<STask*>                        m_lstTask;
    list<STask*>                        m_lstRetryTask;
    list<STask*>                        m_lstTrashbin;
    CMutexLock                          m_mtxWorkMapping;
    map<STask*, CTaskAgent*>            m_mapTaskWorkMapping;       // use for same task checking
    CTaskAgent**                        m_pAgents;                  // agent array
    int                                 m_nAgentCounts;
};
