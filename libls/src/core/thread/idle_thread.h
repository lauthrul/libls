/************************************************************************
* Idle Thread
* author: neco
* date: 2016/04/05
*************************************************************************/

#ifndef _IDLE_THREAD_H_
#define _IDLE_THREAD_H_

#include "thread.h"

namespace lslib
{
    //////////////////////////////////////////////////////////////////////////
#ifdef _MSC_VER
    //////////////////////////////////////////////////////////////////////////
    class LSLIB_API IIdleTreadClient
    {
    public:
        // subclass should implement this function if it want handler the task callback event
        virtual HWND GetMessageHanlder() {  return NULL;    };
    };

    typedef int (IIdleTreadClient::*PTR_CLIENT_IDLETHREAD_TASKHANDLER)  (ldword, ldword&);    // param: (point of parameter data, point of result data)
    typedef int (*PTR_STATIC_IDLETHREAD_TASKHANDLER)                    (ldword, ldword&);
#endif

    typedef int (CThread::*PTR_THREAD_IDLETHREAD_TASKHANDLER)           (ldword, ldword&);


    struct LSLIB_API SIdleThreadTask
    {
        ldword                     m_dwUniqueID;
        ldword                     m_dwTID;
        string                      m_strTaskName;
        ldword                     m_pParam;  // parameter data
        ldword                     m_pResult; // result data
        bool                        m_bKeepSingle;

        enum EIdleTaskType {ITT_CLIENT, ITT_STATIC, ITT_THREAD};
        EIdleTaskType               m_eTaskType;
        bool                        m_bSyncCallback;    // sync or async when call back

#ifdef _MSC_VER
        // for client task
        PTR_CLIENT_IDLETHREAD_TASKHANDLER m_ptrClientTask;
        IIdleTreadClient*           m_pClient;

        // for static task
        PTR_STATIC_IDLETHREAD_TASKHANDLER m_ptrStaticTask;
        HWND                        m_hcbStatic;
#endif

        // for thread task
        PTR_THREAD_IDLETHREAD_TASKHANDLER m_ptrThreadTask;
        CThread*                    m_pcbThread;

        SIdleThreadTask()
        { Reset(); }

        ~SIdleThreadTask()
        { }

        void Reset()
        {
            m_dwUniqueID = 0;
            m_dwTID = 0;
            m_strTaskName.clear();
            m_pParam = 0;
            m_pResult = 0;
            m_bKeepSingle = false;
            m_eTaskType = ITT_CLIENT;
            m_bSyncCallback = false;

#ifdef _MSC_VER
            m_ptrClientTask = NULL;
            m_pClient = NULL;

            m_ptrStaticTask = NULL;
            m_hcbStatic = NULL;
#endif

            m_ptrThreadTask = NULL;
            m_pcbThread = NULL;
        }
    };

    typedef bool (*FnTaskCompare)       (const SIdleThreadTask& lhs, const SIdleThreadTask& rhs);

    //////////////////////////////////////////////////////////////////////////
    extern LSLIB_API ldword GenerateUniqueID();
    extern LSLIB_API string DumpTaskInfo(const SIdleThreadTask* pTask, bool bBrief = false);

    //////////////////////////////////////////////////////////////////////////
    class LSLIB_API CIdleThread : public CThread
    {
    public :
        CIdleThread(CThread* pManager = NULL);  //CreateSuspended:true 要调用Resume(),false 直接调用Execute()
        virtual ~CIdleThread();

    public:
        bool    IsIdle();
        int     GetTaskRemain();
        ldword RunTask(const SIdleThreadTask& task);
#ifdef _MSC_VER
        bool    CancelTask(IIdleTreadClient* pClient, PTR_CLIENT_IDLETHREAD_TASKHANDLER hanlder = NULL);
#endif
        bool    CancelTask(ldword dwUniqueID);
        bool    RegisterTaskCompareFn(ldword dwTID, FnTaskCompare pFunc);
        bool    UnRegisterTaskCompareFn(ldword dwTID);
        bool    IsTaskExist(const SIdleThreadTask& task);
        bool    IsTaskExist(const SIdleThreadTask& task, FnTaskCompare pfunc);

    protected:
        virtual lpcstr GetName() { return "CIdleThread"; };
        virtual void    OnExecute();
        virtual void    OnDumpThreadInfo();

        void    RetryCBNotify();

    protected:
        CThread*                m_pManager;
        list<SIdleThreadTask*>  m_lstTasks;
        CMutexLock              m_mutexTmpTasks;
        list<SIdleThreadTask*>  m_lstTmpTasks;
        CMutexLock              m_mutexTasks;
        map<SIdleThreadTask*, int>  m_mapRetryCBNotify;     // <SIdleThreadTask*, result code>
        map<ldword, FnTaskCompare> m_mapfnTaksCmp;
        CMutexLock              m_mutexFnTaskCmp;
    };

    //////////////////////////////////////////////////////////////////////////
    class LSLIB_API CIdleThreadManager : public CThread
    {
    public:
        CIdleThreadManager(int nThreadNums);
        virtual ~CIdleThreadManager();

    public:
        ldword RunTask(const SIdleThreadTask& task, int nThreadIndex = -1);
#ifdef _MSC_VER
        bool    CancelTask(IIdleTreadClient* pClient, PTR_CLIENT_IDLETHREAD_TASKHANDLER hanlder = NULL, int nThreadIndex = -1);
#endif
        bool    CancelTask(ldword dwUniqueID, int nThreadIndex = -1);
        bool    RegisterTaskCompareFn(ldword dwTID, FnTaskCompare pFunc);
        bool    UnRegisterTaskCompareFn(ldword dwTID);
        int     GetThreadNums();
        int     GetLastThreadIndex();

    protected:
        virtual lpcstr GetName() { return "CIdleThreadManager"; };
        virtual int HandleMessage(msgid_t msgid, wparam_t wparam, lparam_t lparam);

        void    SetThreadNums(int nNum);

    private:
        int                 m_nThreadNums;
        CIdleThread**       m_pThreads;
        CMutexLock          m_mtxTaskThreadMap;
        map<ldword, int>   m_mapTaskThreadMap;     // <UniqueID, ThreadIndex>
        static int          s_nLastThreadIndex;
    };
}

//////////////////////////////////////////////////////////////////////////

#endif //end of _IDLE_THREAD_H_
