#pragma once

#include "lock/lock.h"
#include <list>
#include <map>
using namespace std;

namespace lslib
{

    enum EThreadState
    {
        TS_WORKING,
        TS_SUSPENDED,
    };

#ifdef _MSC_VER

    //////////////////////////////////////////////////////////////////////////
#include <windows.h>

#define __THREAD__ GetCurrentThreadId()

    typedef bool (*CompareMessageFunc) (MSG, int, MSG, int);

    class LSLIB_API CThread
    {
    public :
        CThread(bool bSuspend);
        virtual ~CThread();

    public:
        virtual const char* GetName() = 0;

        bool Suspend();
        bool Resume();
        virtual bool Stop(); // must call in subclass destructor firstly
        unsigned int GetThreadID() const;
        HANDLE GetThreadHandle() const;
        EThreadState GetThreadState() const;
        int GetPriority() const;
        bool SetPriority(int nPriority);
        bool WaitFor(DWORD dwMilliseconds);// return: [true]-OK; [false]-timeout
        bool PostMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0, int nLevel = 0);
        LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);
        bool FindMessage(MSG msg, int level, CompareMessageFunc pfunc);

    protected:
        bool GetMessage(LPMSG lpMsg);

        virtual void Execute();
        virtual void OnExecute();
        virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    private:
        static unsigned __stdcall _ThreadEntry(void* pParam);

    protected:
        EThreadState m_eThreadState;
        int m_nPriority;
        unsigned int m_uThreadID;
        HANDLE m_hThreadHandle;
        CMutexLock m_mutexMsgs;
        map<int, list<MSG> > m_mapMsgs;// <level, list<msg>>
    };

#else

    //////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>

#define WM_QUIT 0
#define MAXINT 2147483647

#define __THREAD__ pthread_self()

    typedef uint32_t msgid_t;
    typedef uint64_t wparam_t;
    typedef uint64_t lparam_t;

    struct msg_t
    {
        msgid_t message;
        wparam_t wParam;
        lparam_t lParam;
    };

    typedef bool (*CompareMessageFunc)(msg_t, int, msg_t, int);

    class LSLIB_API CThread
    {
    public:
        CThread(bool bSuspend);
        virtual ~CThread();

    public:
        const virtual char* GetName() = 0;

        bool Suspend();
        bool Resume();
        virtual bool Stop(); // must call in subclass destructor firstly
        unsigned int GetThreadID() const;
        EThreadState GetThreadState() const;
        bool PostMessage(msgid_t msgid, wparam_t wParam = 0, lparam_t lParam = 0, int nLevel = 0);
        int SendMessage(msgid_t msgid, wparam_t wParam = 0, lparam_t lParam = 0);
        bool FindMessage(msg_t msg, int level, CompareMessageFunc pfunc);

    protected:
        bool GetMessage(__out__ msg_t* pmsg);

        virtual void Execute();
        virtual void OnExecute();
        virtual int HandleMessage(msgid_t msgid, wparam_t wparam, lparam_t lparam);

    private:
        static void* _ThreadEntry(void* pParam);

    protected:
        EThreadState m_eThreadState;
        int m_nPriority;
        unsigned int m_uThreadID;
        pthread_t th;
        int m_hThreadHandle;
        CMutexLock m_mutexMsgs;
        map<int, list<msg_t> > m_mapMsgs;       // <level, list<msg_t>>

        pthread_mutex_t mutex_pause;
        pthread_cond_t cond_pause;
    };

#endif

}
