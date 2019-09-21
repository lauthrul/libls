#include "stdafx.h"
#include "thread.h"

//////////////////////////////////////////////////////////////////////////
namespace lslib
{
    using namespace logger;

    CThread::CThread(bool bSuspend) : m_tDumpTimeStamp(0)
    {
#ifdef _MSC_VER
        if (bSuspend)
        {
            m_hThreadHandle = (HANDLE)_beginthreadex(NULL, 0, (unsigned int(__stdcall *)(void *))_ThreadEntry, this, CREATE_SUSPENDED, &m_uThreadID);
            m_eThreadState = TS_SUSPENDED;
        }
        else
        {
            m_hThreadHandle = (HANDLE)_beginthreadex(NULL, 0, (unsigned int(__stdcall *)(void *))_ThreadEntry, this, NULL, &m_uThreadID);
            m_eThreadState = TS_WORKING;
        }
#else
        pthread_mutex_init(&m_mtxPause, NULL);
        pthread_cond_init(&m_conPause, NULL);
        m_eThreadState = TS_WORKING;

        pthread_create(&m_uThreadID, NULL, _ThreadEntry, this);

        if (bSuspend) Suspend();
#endif
    }

    CThread::~CThread()
    {
        Stop();
    }

    bool CThread::Suspend()
    {
        DEBUG_LOG(g_logger, "%s[0x%p] suspend", GetName(), m_uThreadID);

#ifdef _MSC_VER
        if (m_hThreadHandle <= 0)                   return false;
        if (SuspendThread(m_hThreadHandle) < 0)     return false;
        m_eThreadState = TS_SUSPENDED;
        return true;
#else
        if (m_eThreadState == TS_WORKING)
            m_eThreadState = TS_SUSPENDED;
        return true;
#endif
    }

    bool CThread::Resume()
    {
        DEBUG_LOG(g_logger, "%s[0x%p] resume", GetName(), m_uThreadID);

#ifdef _MSC_VER
        if (m_hThreadHandle <= 0)                   return false;
        if (ResumeThread(m_hThreadHandle) < 0)      return false;
        m_eThreadState = TS_WORKING;
        return true;
#else
        if (m_eThreadState == TS_SUSPENDED)
        {
            m_eThreadState = TS_WORKING;
            pthread_cond_signal(&m_conPause);
        }
        return true;
#endif
    }

    void CThread::Run()
    {
#ifdef _MSC_VER
        WaitFor(INFINITE);
#else
        pthread_join(m_uThreadID, NULL);
#endif
    }

    bool CThread::Stop()
    {
#ifdef _MSC_VER
        if (m_eThreadState == TS_WORKING)
        {
            PostMessage(WM_QUIT, 0, 0, MAXINT);
            return WaitFor(5000);
        }
        else
        {
            return TerminateThread(m_hThreadHandle, 0);
        }
#else
        if (m_eThreadState == TS_WORKING)
        {
            PostMessage(WM_QUIT, 0, 0, MAXINT);
            return (pthread_join(m_uThreadID, NULL) == 0);
        }
        else
        {
            return (pthread_cancel(m_uThreadID) == 0);
        }
#endif
    }

    unsigned int CThread::GetThreadID() const
    {
        return m_uThreadID;
    }

    EThreadState CThread::GetThreadState() const
    {
        return m_eThreadState;
    }

    void CThread::Sleep(int nMilliseconds)
    {
#ifdef _MSC_VER
        ::Sleep(nMilliseconds);
#else
        usleep(nMilliseconds * 1000); // 1sec = 1000msec = 1000*1000usec
#endif
    }

#ifdef _MSC_VER

    HANDLE CThread::GetThreadHandle() const
    {
        return m_hThreadHandle;
    }

    int CThread::GetPriority() const
    {
        return m_nPriority;
    }

    bool CThread::SetPriority(int nPriority)
    {
        if (SetThreadPriority(m_hThreadHandle, nPriority))
        {
            m_nPriority = nPriority;
            return true;
        }
        return false;
    }

    bool CThread::WaitFor(DWORD dwMilliseconds)
    {
        if (WaitForSingleObject(m_hThreadHandle, dwMilliseconds) == WAIT_OBJECT_0)
            return true;
        return false;
    }

#endif

    bool CThread::PostMessage(msgid_t uMsg, wparam_t wParam /*= 0*/, lparam_t lParam /*= 0*/, int nLevel /*= 0*/)
    {
#ifdef _MSC_VER
        msg_t msg = { NULL, uMsg, wParam, lParam, 0, 0 };
#else
        msg_t msg = { uMsg, wParam, lParam };
#endif
        m_mutexMsgs.Lock();
        m_mapMsgs[nLevel].push_back(msg);
        m_mutexMsgs.Unlock();
        return true;
    }

    int CThread::SendMessage(msgid_t uMsg, wparam_t wParam /*= 0*/, lparam_t lParam /*= 0*/)
    {
        return HandleMessage(uMsg, wParam, lParam);
    }

    bool CThread::FindMessage(msg_t msg, int level, CompareMessageFunc pfunc)
    {
        bool bret = false;
        m_mutexMsgs.Lock();
        map<int, list<msg_t> >::iterator it = m_mapMsgs.begin();
        for (; it != m_mapMsgs.end(); it++)
        {
            list<msg_t>& msglist = it->second;
            list<msg_t>::iterator itj = msglist.begin();
            for (; itj != msglist.end(); itj++)
            {
                msg_t rfmsg = *itj;
                bret = (*pfunc)(msg, level, rfmsg, it->first);
                if (bret) break;
            }
            if (bret) break;
        }
        m_mutexMsgs.Unlock();
        return bret;
    }

    int CThread::GetMessageSize()
    {
        int nSize = 0;
        m_mutexMsgs.Lock();
        for (map<int, list<msg_t> >::iterator it = m_mapMsgs.begin(); it != m_mapMsgs.end(); it++)
            nSize += it->second.size();
        m_mutexMsgs.Unlock();
        return nSize;
    }

    void CThread::Execute()
    {
        string strThreadName = GetName();
        DEBUG_LOG(g_logger, "%s[0x%p] excute", strThreadName.c_str(), m_uThreadID);

        while (true)
        {
#ifndef _MSC_VER
            if (m_eThreadState == TS_SUSPENDED)
            {
                pthread_mutex_lock(&m_mtxPause);
                pthread_cond_wait(&m_conPause, &m_mtxPause);
                pthread_mutex_unlock(&m_mtxPause);
            }
            else
#endif
            {
                DumpThreadInfo();
                msg_t msg;
                if (GetMessage(&msg))
                {
                    if (msg.message == WM_QUIT)     break;
                    else                            HandleMessage(msg.message, msg.wParam, msg.lParam);
                }
                OnExecute();
                Sleep(1); // 1msec
            }
        }

        DEBUG_LOG(g_logger, "%s[0x%p] exit. remain tasks[%d]", strThreadName.c_str(), m_uThreadID, GetMessageSize());
    }

    void CThread::OnExecute()
    {
    }

    void CThread::DumpThreadInfo()
    {
        time_t stnow = Time::CurrentTimeStamp();
        if (stnow - m_tDumpTimeStamp < 10)  return; // every 10 second

        OnDumpThreadInfo();
        m_tDumpTimeStamp = stnow;
    }

    void CThread::OnDumpThreadInfo()
    {
        DEBUG_LOG(g_logger, "%s[0x%p] msg size[%d]", GetName(), m_uThreadID, GetMessageSize());
    }

    int CThread::HandleMessage(msgid_t uMsg, wparam_t wParam /*= 0*/, lparam_t lParam /*= 0*/)
    {
        return 0;
    }

    bool CThread::GetMessage(__out__ msg_t* lpMsg)
    {
        bool bret = false;
        m_mutexMsgs.Lock();
        if (!m_mapMsgs.empty())
        {
            map<int, list<msg_t> >::reverse_iterator rit = m_mapMsgs.rbegin();
            list<msg_t>& msglist = rit->second;
            if (!msglist.empty())
            {
                msg_t msg = msglist.front();
                msglist.pop_front();
                if (lpMsg != NULL)
                {
                    *lpMsg = msg;
//                     DEBUG_LOG(g_logger, "get msg. level[%d], msg[%d, (0x%p, 0x%p)]", rit->first, msg.message, msg.wParam, msg.lParam);
                }
                if (msglist.empty())
                {
                    m_mapMsgs.erase(rit->first);
                }
            }
            bret = true;
        }
        m_mutexMsgs.Unlock();
        return bret;
    }

// #ifdef _MSC_VER
//     unsigned __stdcall CThread::_ThreadEntry(void* pParam)
//     {
//         if (pParam == NULL) return -1;
//         ((CThread*)pParam)->Execute();
//         return 0;
//     }
// #else
    void* CThread::_ThreadEntry(void* pParam)
    {
        if (pParam == NULL) return (void*)-1;
        ((CThread*)pParam)->Execute();
        return (void*)0;
    }
// #endif

}
