#include "stdafx.h"
#include "thread.h"
#include "logger/logger.h"
using namespace lslib::logger;

//////////////////////////////////////////////////////////////////////////
namespace lslib
{

#ifdef _MSC_VER

    CThread::CThread(bool bSuspend)
    {
        if (bSuspend)
        {
            m_hThreadHandle = (HANDLE)_beginthreadex(NULL, 0, _ThreadEntry, this, CREATE_SUSPENDED, &m_uThreadID);
            m_eThreadState = TS_SUSPENDED;
        }
        else
        {
            m_hThreadHandle = (HANDLE)_beginthreadex(NULL, 0, _ThreadEntry, this, NULL, &m_uThreadID);
            m_eThreadState = TS_WORKING;
        }
    }

    CThread::~CThread()
    {
        Stop();
    }

    bool CThread::Suspend()
    {
        DEBUG_LOG(g_logger, "%s[%d] suspend", GetName(), m_uThreadID);

        if (m_hThreadHandle <= 0)
        return FALSE;
        if (SuspendThread(m_hThreadHandle) < 0)
        return FALSE;
        m_eThreadState = TS_SUSPENDED;
        return TRUE;
    }

    bool CThread::Resume()
    {
        DEBUG_LOG(g_logger, "%s[%d] resume", GetName(), m_uThreadID);

        if (m_hThreadHandle <= 0)
        return FALSE;
        if (ResumeThread(m_hThreadHandle) < 0)
        return FALSE;
        m_eThreadState = TS_WORKING;
        return TRUE;
    }

    void CThread::Run()
    {
        WaitFor(INFINITE);
    }

    bool CThread::Stop()
    {
        if (m_eThreadState == TS_WORKING)
        {
            PostMessage(WM_QUIT, 0, 0, MAXINT);
            return WaitFor(5000);
        }
        else
        {
            return TerminateThread(m_hThreadHandle, 0);
        }
    }

    unsigned int CThread::GetThreadID() const
    {
        return m_uThreadID;
    }

    HANDLE CThread::GetThreadHandle() const
    {
        return m_hThreadHandle;
    }

    EThreadState CThread::GetThreadState() const
    {
        return m_eThreadState;
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

    bool CThread::PostMessage(UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/, int nLevel)
    {
        MSG msg =
        {   NULL, uMsg, wParam, lParam, 0, 0};
        m_mutexMsgs.Lock();
        m_mapMsgs[nLevel].push_back(msg);
        m_mutexMsgs.Unlock();
        return true;
    }

    int CThread::SendMessage(msgid_t uMsg, wparam_t wParam /*= 0*/, lparam_t lParam /*= 0*/)
    {
        return HandleMessage(uMsg, wParam, lParam);
    }

    bool CThread::FindMessage(MSG msg, int level, CompareMessageFunc pfunc)
    {
        bool bret = false;
        m_mutexMsgs.Lock();
        map<int, list<MSG> >::iterator it = m_mapMsgs.begin();
        for (; it != m_mapMsgs.end(); it++)
        {
            list<MSG>& msglist = it->second;
            list<MSG>::iterator itj = msglist.begin();
            for (; itj != msglist.end(); itj++)
            {
                MSG rfmsg = *itj;
                bret = (*pfunc)(msg, level, rfmsg, it->first);
                if (bret)
                break;
            }
            if (bret)
            break;
        }
        m_mutexMsgs.Unlock();
        return bret;
    }

    void CThread::Execute()
    {
        string strThreadName = GetName();
        DEBUG_LOG(g_logger, "%s[%d] excute", strThreadName.c_str(), m_uThreadID);

        while (true)
        {
            MSG msg;
            if (GetMessage(&msg))
            {
                if (msg.message == WM_QUIT)
                break;
                else
                HandleMessage(msg.message, msg.wParam, msg.lParam);
            }
            OnExecute();
            Sleep(1);
        }

        int nRemains = 0;
        m_mutexMsgs.Lock();
        for (map<int, list<MSG> >::iterator it = m_mapMsgs.begin(); it != m_mapMsgs.end(); it++)
        nRemains += it->second.size();
        m_mutexMsgs.Unlock();

        DEBUG_LOG(g_logger, "%s[%d] exit. remain tasks[%d]", strThreadName.c_str(), m_uThreadID, nRemains);
    }

    void CThread::OnExecute()
    {
    }

    int CThread::HandleMessage(msgid_t uMsg, wparam_t wParam /*= 0*/, lparam_t lParam /*= 0*/)
    {
        return S_OK;
    }

    bool CThread::GetMessage(LPMSG lpMsg)
    {
        bool bret = false;
        m_mutexMsgs.Lock();
        if (!m_mapMsgs.empty())
        {
            map<int, list<MSG> >::reverse_iterator rit = m_mapMsgs.rbegin();
            list<MSG>& msglist = rit->second;
            if (!msglist.empty())
            {
                MSG msg = msglist.front();
                msglist.pop_front();
                if (lpMsg != NULL)
                {
                    *lpMsg = msg;
//                      DEBUG_LOG(g_logger, "get msg. level[%d], msg[%d, (0x%x, 0x%x)]", rit->first, msg.message, msg.wParam, msg.lParam);
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

    unsigned int __stdcall CThread::_ThreadEntry(void* pParam)
    {
        if (pParam == NULL)
        return -1;
        ((CThread*)pParam)->Execute();
        return 0;
    }

#else

    CThread::CThread(bool bSuspend)
    {
        pthread_mutex_init(&m_mtxPause, NULL);
        pthread_cond_init(&m_conPause, NULL);
        m_eThreadState = TS_WORKING;

        pthread_create(&m_uThreadID, NULL, _ThreadEntry, this);

        if (bSuspend) Suspend();
    }

    CThread::~CThread()
    {
        Stop();
    }

    bool CThread::Suspend()
    {
        DEBUG_LOG(g_logger, "%s[%d] suspend", GetName(), m_uThreadID);

        if (m_eThreadState == TS_WORKING)
        {
            m_eThreadState = TS_SUSPENDED;
        }
        return true;
    }

    bool CThread::Resume()
    {
        DEBUG_LOG(g_logger, "%s[%d] resume", GetName(), m_uThreadID);

        if (m_eThreadState == TS_SUSPENDED)
        {
            m_eThreadState = TS_WORKING;
            pthread_cond_signal(&m_conPause);
        }
        return true;
    }

    void CThread::Run()
    {
        pthread_join(m_uThreadID, NULL);
    }

    bool CThread::Stop()
    {
        if (m_eThreadState == TS_WORKING)
        {
            PostMessage(WM_QUIT, 0, 0, MAXINT);
            return (pthread_join(m_uThreadID, NULL) == 0);
        }
        else
        {
            return (pthread_cancel(m_uThreadID) == 0);
        }
    }

    unsigned int CThread::GetThreadID() const
    {
        return m_uThreadID;
    }

    EThreadState CThread::GetThreadState() const
    {
        return m_eThreadState;
    }

    bool CThread::PostMessage(msgid_t uMsg, wparam_t wParam /*= 0*/, lparam_t lParam /*= 0*/, int nLevel /*= 0*/)
    {
        msg_t msg = { uMsg, wParam, lParam };
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

    void CThread::Execute()
    {
        string strThreadName = GetName();
        DEBUG_LOG(g_logger, "%s[%d] excute", strThreadName.c_str(), m_uThreadID);

        while (true)
        {
            if (m_eThreadState == TS_SUSPENDED)
            {
                pthread_mutex_lock(&m_mtxPause);
                pthread_cond_wait(&m_conPause, &m_mtxPause);
                pthread_mutex_unlock(&m_mtxPause);
            }
            else
            {
                msg_t msg;
                if (GetMessage(&msg))
                {
                    if (msg.message == WM_QUIT)
                    {
                        printf("stopped");
                        break;
                    }
                    else HandleMessage(msg.message, msg.wParam, msg.lParam);
                }
                OnExecute();
//                usleep(1000); // 1msec.  1sec = 1000msec = 1000*1000usec
            }
        }

        int nRemains = 0;
        m_mutexMsgs.Lock();
        for (map<int, list<msg_t> >::iterator it = m_mapMsgs.begin(); it != m_mapMsgs.end(); it++)
            nRemains += it->second.size();
        m_mutexMsgs.Unlock();

        DEBUG_LOG(g_logger, "%s[%d] exit. remain tasks[%d]", strThreadName.c_str(), m_uThreadID, nRemains);
    }

    void CThread::OnExecute()
    {
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
                    DEBUG_LOG(g_logger, "get msg. level[%d], msg[%d, (0x%x, 0x%x)]", rit->first, msg.message, msg.wParam, msg.lParam);
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

    void* CThread::_ThreadEntry(void* pParam)
    {
        if (pParam == NULL) return (void*)-1;
        ((CThread*)pParam)->Execute();
        return (void*)0;
    }

#endif

}
