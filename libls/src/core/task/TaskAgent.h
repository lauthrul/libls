#pragma once

//////////////////////////////////////////////////////////////////////////
namespace lslib
{
    struct SInvoker
    {
        int     nID;                                        ///< task id
        string  strTaskName;                                ///< task name
        PTR_THREAD_IDLETHREAD_TASKHANDLER   ptrHandler;     ///< task handle
        int     nMsgID;                                     ///< message id on which will fire after task finish. no trigge if messageID == 0

        SInvoker() : nID(0), ptrHandler(NULL), nMsgID(0) {};
        SInvoker(int tid, lpcstr name, PTR_THREAD_IDLETHREAD_TASKHANDLER handler, int msgid = 0)
            : nID(tid), strTaskName(name), ptrHandler(handler), nMsgID(msgid)
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
        SInvoker*                       GetInvoker(luint uTID) const;

    protected:
        CTaskManager*                   m_pManager;
        map<luint, SInvoker>            m_mapInvokerMap;
    };
}