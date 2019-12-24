#pragma once

#ifdef _MSC_VER
#include <windows.h>
#else
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#endif

namespace lslib
{
    //////////////////////////////////////////////////////////////////////////

    enum EThreadState
    {
        TS_WORKING,
        TS_SUSPENDED,
    };

#ifdef _MSC_VER

#define __THREAD__      GetCurrentThreadId()

    typedef UINT        msgid_t;
    typedef WPARAM      wparam_t;
    typedef LPARAM      lparam_t;

#else

#define __THREAD__      pthread_self()
#define WM_QUIT         0
#define MAXINT          2147483647

    typedef uint32_t    msgid_t;
    typedef uint64_t    wparam_t;
    typedef uint64_t    lparam_t;

#endif // end of _MSC_VER

    typedef struct msg_t
    {
        msgid_t message;
        wparam_t wParam;
        lparam_t lParam;
        luint uDelay;
        time_t tTime;

        msg_t() { memset(this, 0, sizeof(msg_t)); }
    };

    typedef bool (*CompareMessageFunc) (msg_t, int, msg_t, int);

    //////////////////////////////////////////////////////////////////////////
    class LSLIB_API CThread
    {
    public :
        CThread(bool bSuspend);
        virtual ~CThread();

    private:
        CThread(const CThread& value);
        CThread& operator = (const CThread& value);

    public:
        virtual lpcstr GetName() { return "CThread"; };

        bool Suspend();
        bool Resume();
        virtual void Run();
        virtual bool Stop(); // must call in subclass destructor firstly
        unsigned int GetThreadID() const;
        EThreadState GetThreadState() const;
        void Sleep(int nMilliseconds);

#ifdef _MSC_VER
        HANDLE GetThreadHandle() const;
        int GetPriority() const;
        bool SetPriority(int nPriority);
        bool WaitFor(DWORD dwMilliseconds);// return: [true]-OK; [false]-timeout
#endif
        bool PostMessage(msgid_t msgid, wparam_t wParam = 0, lparam_t lParam = 0, int nLevel = 0);
        bool PostMessageDelay(msgid_t msgid, luint uDelay, wparam_t wParam = 0, lparam_t lParam = 0, int nLevel = 0);
        int SendMessage(msgid_t msgid, wparam_t wParam = 0, lparam_t lParam = 0);
        bool FindMessage(msg_t msg, int level, CompareMessageFunc pfunc);
        int GetMessageSize();

    protected:
        bool GetMessage(__out__ msg_t* pmsg);

        virtual void Execute();
        virtual void OnExecute();
        virtual void DumpThreadInfo();
        virtual void OnDumpThreadInfo();
        virtual int HandleMessage(msgid_t msgid, wparam_t wparam, lparam_t lparam);

    private:
        static void* _ThreadEntry(void* pParam);

    protected:
        EThreadState m_eThreadState;
        CMutexLock m_mutexMsgs;
        map<int, list<msg_t> > m_mapMsgs;// <level, list<msg_t>>
        time_t m_tDumpTimeStamp;

#ifdef _MSC_VER
        int m_nPriority;
        unsigned int m_uThreadID;
        HANDLE m_hThreadHandle;
#else
        pthread_t m_uThreadID;
        pthread_mutex_t m_mtxPause;
        pthread_cond_t m_conPause;
#endif

    };
}
