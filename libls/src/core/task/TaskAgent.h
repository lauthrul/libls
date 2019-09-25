#pragma once

//////////////////////////////////////////////////////////////////////////
namespace lslib
{
    struct SInvoker
    {
        int     nID;                                        ///< task id
        string  strTaskName;                                ///< task name
        PTR_THREAD_IDLETHREAD_TASKHANDLER   ptrHandler;     ///< task handle

        SInvoker() {}
        SInvoker(int tid, _lpcstr name, PTR_THREAD_IDLETHREAD_TASKHANDLER handler)
            : nID(tid), strTaskName(name), ptrHandler(handler)
        {}
    };

    //////////////////////////////////////////////////////////////////////////

    class CTaskManager;
    class LSLIB_API CTaskAgent : public CIdleThread
    {
    public:
        CTaskAgent(CTaskManager* pManager);
        virtual ~CTaskAgent();

    public:
        // implement from CThread
        virtual const char*             GetName() { return "CTaskAgent"; };
        virtual int                     HandleMessage(msgid_t uMsg, wparam_t wParam, lparam_t lParam);

        virtual void                    InitInvoker() = 0;

        void                            AddInvoker(const SInvoker& vInvoker);
        SInvoker*                       GetInvoker(int dwTID) const;

    protected:
        CTaskManager*                   m_pManager;
        map<int, SInvoker>              m_mapInvokerMap;
    };
}