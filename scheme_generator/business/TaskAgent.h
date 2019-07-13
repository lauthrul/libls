#pragma once

//////////////////////////////////////////////////////////////////////////
struct SInvoker
{
    int         nID;
    lstring     strTaskName;
    lstring     strNotifyMsg;
    PTR_THREAD_IDLETHREAD_TASKHANDLER       ptrHandler;

    SInvoker() {}
    SInvoker(int tid, _lpcstr notifymsg, _lpcstr name, PTR_THREAD_IDLETHREAD_TASKHANDLER handler)
        : nID(tid), strNotifyMsg(notifymsg), strTaskName(name), ptrHandler(handler)
    {}
};

//////////////////////////////////////////////////////////////////////////
class CTaskManager;
class CTaskAgent : public CIdleThread
{
public:
    CTaskAgent(CTaskManager* pManager);
    virtual ~CTaskAgent();

public:
    // implement from CThread
    virtual _lpcstr                 GetName() { return "CTaskAgent"; };
    virtual int                     HandleMessage(msgid_t uMsg, wparam_t wParam, lparam_t lParam);

    virtual void                    InitInvoker() = 0;

    void                            AddInvoker(const SInvoker& vInvoker);
    SInvoker*                       GetInvoker(int dwTID) const;

protected:
    CTaskManager*                   m_pManager;
    map<int, SInvoker>              m_mapInvokerMap;
};
