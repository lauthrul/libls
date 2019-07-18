/************************************************************************
* Idle Thread
* author: neco
* date: 2016/04/05
*************************************************************************/

#include "stdafx.h"
#include "idle_thread.h"

namespace lslib
{
    using namespace logger;

#define WM_TASK_FINISH      0x7fffffff

//////////////////////////////////////////////////////////////////////////
    bool FnDefaultTaskCompare(const SIdleThreadTask& lhs, const SIdleThreadTask& rhs)
    {
        if (lhs.m_dwTID != rhs.m_dwTID)                         return false;
        if (lhs.m_strTaskName != rhs.m_strTaskName)             return false;
        if (lhs.m_pParam != rhs.m_pParam)                       return false;
        if (lhs.m_eTaskType != rhs.m_eTaskType)                 return false;
        switch (lhs.m_eTaskType)
        {
#ifdef _MSC_VER
            case SIdleThreadTask::ITT_CLIENT:
                if (lhs.m_ptrClientTask != rhs.m_ptrClientTask)     return false;
                if (lhs.m_pClient != rhs.m_pClient)                 return false;
            case SIdleThreadTask::ITT_STATIC:
                if (lhs.m_ptrStaticTask != rhs.m_ptrStaticTask)     return false;
                if (lhs.m_hcbStatic != rhs.m_hcbStatic)             return false;
#endif
            case SIdleThreadTask::ITT_THREAD:
                if (lhs.m_ptrThreadTask != rhs.m_ptrThreadTask)     return false;
                if (lhs.m_pcbThread != rhs.m_pcbThread)             return false;
        }
        return true;
    }

    _ldword GenerateUniqueID()
    {
        static int nIndex = 0;
        return Time::GetCurDateTime().GetDateTime() + nIndex++;
    }

//////////////////////////////////////////////////////////////////////////
    CIdleThread::CIdleThread(CThread* pManager /*= NULL*/) : CThread(true), m_pManager(pManager)
    {
        Resume();
    }

    CIdleThread::~CIdleThread()
    {
        Stop();

        list<SIdleThreadTask*>::iterator it = m_lstTmpTasks.begin();
        while (it != m_lstTmpTasks.end())
        {
            SIdleThreadTask* pTask = *it;
            if (pTask)
            {
                delete pTask;
                pTask = NULL;
            }
            m_lstTmpTasks.erase(it++);
        }

        it = m_lstTasks.begin();
        while (it != m_lstTasks.end())
        {
            SIdleThreadTask* pTask = *it;
            if (pTask)
            {
                delete pTask;
                pTask = NULL;
            }
            m_lstTasks.erase(it++);
        }
    }

    bool CIdleThread::IsIdle()
    {
        bool bIdle = m_lstTmpTasks.empty();
        bIdle &= m_lstTasks.empty();
        return bIdle;
    }

    int CIdleThread::GetTaskRemain()
    {
        return m_lstTmpTasks.size() + m_lstTasks.size();
    }

    void CIdleThread::OnExecute()
    {
        //get task from temp task list
        m_mutexTmpTasks.Lock();
        list<SIdleThreadTask*>::iterator it = m_lstTmpTasks.begin();
        while (it != m_lstTmpTasks.end() && m_lstTasks.size() < 5)
        {
            SIdleThreadTask* pTask = *it;
            m_lstTmpTasks.erase(it++);
            m_lstTasks.push_back(pTask);
        }
        m_mutexTmpTasks.Unlock();

        //run task
        bool bPostRet = true;
        bool bDelete = true;
        it = m_lstTasks.begin();
        while (it != m_lstTasks.end())
        {
            m_mutexTasks.Lock();
            SIdleThreadTask* pTask = *it;
            if (pTask != NULL)
            {
                switch (pTask->m_eTaskType)
                {
#ifdef _MSC_VER
                    case SIdleThreadTask::ITT_CLIENT:
                        if ( pTask->m_pClient != NULL && pTask->m_ptrClientTask != NULL )
                        {
                            DEBUG_LOG(g_logger, "[%d] run client task[%d, %s, 0x%p]", GetThreadID(), pTask->m_dwUniqueID, pTask->m_strTaskName.c_str(), pTask->m_ptrClientTask);
                            int nRet = (pTask->m_pClient->*(pTask->m_ptrClientTask))(pTask->m_pParam, pTask->m_pResult);
                            if (pTask->m_pClient->GetMessageHanlder() != NULL)
                            {
                                // ::PostMessage may fail when hits system message queue limits (10,000 by default)
                                if (pTask->m_bSyncCallback) ::SendMessage(pTask->m_pClient->GetMessageHanlder(), pTask->m_dwTID, (wparam_t)nRet, (lparam_t)pTask->m_pResult);
                                else                        bPostRet = ::PostMessage(pTask->m_pClient->GetMessageHanlder(), pTask->m_dwTID, (wparam_t)nRet, (lparam_t)pTask->m_pResult);
                                DEBUG_LOG(g_logger, "post client task[%d] msg[0x%p, %d, %d, 0x%p], result[%d], err[%d]",
                                          pTask->m_dwUniqueID, pTask->m_pClient->GetMessageHanlder(), pTask->m_dwTID, (wparam_t)nRet, (lparam_t)pTask->m_pResult, bPostRet, GetLastError());
                                if (bPostRet)
                                {
                                    if (m_pManager != NULL)
                                        m_pManager->PostMessage(WM_TASK_FINISH, pTask->m_dwUniqueID);
                                }
                                else
                                {
                                    m_mapRetryCBNotify[pTask] = nRet;
                                    bDelete = FALSE;
                                }
                            }
                            DEBUG_LOG(g_logger, "[%d] finish client task[%d, %s, 0x%p]", GetThreadID(), pTask->m_dwUniqueID, pTask->m_strTaskName.c_str(), pTask->m_ptrClientTask);
                        }
                        break;
                    case SIdleThreadTask::ITT_STATIC:
                        {
                            DEBUG_LOG(g_logger, "[%d] run static task[%d, %s, 0x%p]", GetThreadID(), pTask->m_dwUniqueID, pTask->m_strTaskName.c_str(), pTask->m_ptrStaticTask);
                            int nRet = (*(pTask->m_ptrStaticTask))(pTask->m_pParam, pTask->m_pResult);
                            if (pTask->m_hcbStatic != NULL)
                            {
                                // ::PostMessage may fail when hits system message queue limits (10,000 by default)
                                if (pTask->m_bSyncCallback) ::SendMessage(pTask->m_hcbStatic, pTask->m_dwTID, (wparam_t)nRet, (lparam_t)pTask->m_pResult);
                                else                        bPostRet = ::PostMessage(pTask->m_hcbStatic, pTask->m_dwTID, (wparam_t)nRet, (lparam_t)pTask->m_pResult);
                                DEBUG_LOG(g_logger, "post client task[%d] msg[0x%p, %d, %d, 0x%p], result[%d], err[%d]",
                                          pTask->m_dwUniqueID, pTask->m_hcbStatic, pTask->m_dwTID, (wparam_t)nRet, (lparam_t)pTask->m_pResult, bPostRet, GetLastError());
                                if (bPostRet)
                                {
                                    if (m_pManager != NULL)
                                        m_pManager->PostMessage(WM_TASK_FINISH, pTask->m_dwUniqueID);
                                }
                                else
                                {
                                    m_mapRetryCBNotify[pTask] = nRet;
                                    bDelete = FALSE;
                                }
                            }
                            DEBUG_LOG(g_logger, "[%d] finish static task[%d, %s, 0x%p]", GetThreadID(), pTask->m_dwUniqueID, pTask->m_strTaskName.c_str(), pTask->m_ptrStaticTask);
                        }
                        break;
#endif
                    case SIdleThreadTask::ITT_THREAD:
                        if ( pTask->m_pcbThread != NULL && pTask->m_ptrThreadTask != NULL )
                        {
                            DEBUG_LOG(g_logger, "[%d] run thread task[%d, %s, 0x%p]", GetThreadID(), pTask->m_dwUniqueID, pTask->m_strTaskName.c_str(), pTask->m_ptrThreadTask);
                            int nRet = (pTask->m_pcbThread->*(pTask->m_ptrThreadTask))(pTask->m_pParam, pTask->m_pResult);
                            {
                                if (pTask->m_bSyncCallback) pTask->m_pcbThread->SendMessage(pTask->m_dwTID, (wparam_t)nRet, (lparam_t)pTask->m_pResult);
                                else                        pTask->m_pcbThread->PostMessage(pTask->m_dwTID, (wparam_t)nRet, (lparam_t)pTask->m_pResult);
                                if (m_pManager != NULL) m_pManager->PostMessage(WM_TASK_FINISH, pTask->m_dwUniqueID);
                            }
                            DEBUG_LOG(g_logger, "[%d] finish thread task[%d, %s, 0x%p]", GetThreadID(), pTask->m_dwUniqueID, pTask->m_strTaskName.c_str(), pTask->m_ptrThreadTask);
                        }
                        break;
                }
            }
            m_lstTasks.erase(it++);
            if (bDelete) delete pTask;
            m_mutexTasks.Unlock();
        }

        // retry cb notify
        RetryCBNotify();
    }

    void CIdleThread::RetryCBNotify()
    {
#ifdef _MSC_VER
        static Time stm;
        Time tm = Time::GetCurDateTime();
        if (tm.BetweenAllMilliSec(stm) < 100) // retry cb notiry every 100 ms
            return;
        stm = tm;

        bool bPostRet = true;
        map<SIdleThreadTask*, int>::iterator itx = m_mapRetryCBNotify.begin();
        while (itx != m_mapRetryCBNotify.end())
        {
            SIdleThreadTask* pTask = itx->first;
            int nRet = itx->second;
            if (pTask != NULL)
            {
                switch (pTask->m_eTaskType)
                {
                    case SIdleThreadTask::ITT_CLIENT:
                        {
                            bPostRet = ::PostMessage(pTask->m_pClient->GetMessageHanlder(), pTask->m_dwTID, (wparam_t)nRet, (lparam_t)pTask->m_pResult);
                            DEBUG_LOG(g_logger, "retry post client task[%d] msg[0x%p, %d, %d, 0x%p], result[%d], err[%d]",
                                      pTask->m_dwUniqueID, pTask->m_pClient->GetMessageHanlder(), pTask->m_dwTID, (wparam_t)nRet, (lparam_t)pTask->m_pResult, bPostRet, GetLastError());
                        }
                        break;
                    case SIdleThreadTask::ITT_STATIC:
                        {
                            bPostRet = ::PostMessage(pTask->m_hcbStatic, pTask->m_dwTID, (wparam_t)nRet, (lparam_t)pTask->m_pResult);
                            DEBUG_LOG(g_logger, "retry post client task[%d] msg[0x%p, %d, %d, 0x%p], result[%d], err[%d]",
                                      pTask->m_dwUniqueID, pTask->m_hcbStatic, pTask->m_dwTID, (wparam_t)nRet, (lparam_t)pTask->m_pResult, bPostRet, GetLastError());
                        }
                        break;
                }
                if (bPostRet)
                {
                    if (m_pManager != NULL)
                        m_pManager->PostMessage(WM_TASK_FINISH, pTask->m_dwUniqueID);
                    m_mapRetryCBNotify.erase(itx++);
                    delete pTask;
                }
                else itx++;
            }
        }
#endif
    }

    _ldword CIdleThread::RunTask(SIdleThreadTask task)
    {
        if (task.m_bKeepSingle && IsTaskExist(task))
            return 0;

        task.m_dwUniqueID = GenerateUniqueID();
#ifdef _MSC_VER
        DEBUG_LOG(g_logger, "[%d] add task[%d, %s, %d, %d, 0x%p, 0x%p, 0x%p]", GetThreadID(),
                  task.m_dwUniqueID, task.m_strTaskName.c_str(), task.m_pParam, task.m_eTaskType, task.m_ptrClientTask, task.m_ptrStaticTask, task.m_ptrThreadTask);
#else
        DEBUG_LOG(g_logger, "[%d] add task[%d, %s, %d, %d, 0x%p]", GetThreadID(),
                  task.m_dwUniqueID, task.m_strTaskName.c_str(), task.m_pParam, task.m_eTaskType, task.m_ptrThreadTask);
#endif
        m_mutexTmpTasks.Lock();
        m_lstTmpTasks.push_back(new SIdleThreadTask(task));
        m_mutexTmpTasks.Unlock();
        return task.m_dwUniqueID;
    }

#ifdef _MSC_VER
    bool CIdleThread::CancelTask(IIdleTreadClient* pClient, PTR_CLIENT_IDLETHREAD_TASKHANDLER hanlder /*= NULL*/)
    {
        if (pClient == NULL) return false;

        list<SIdleThreadTask*>::iterator it;
        bool bret = false;

        m_mutexTmpTasks.Lock();
        it = m_lstTmpTasks.begin();
        while (it != m_lstTmpTasks.end())
        {
            SIdleThreadTask* pTask = *it;
            if ( pTask && pClient == pTask->m_pClient && ( hanlder == NULL || hanlder == pTask->m_ptrClientTask ) )
            {
                DEBUG_LOG(g_logger, "[%d] cancel task[%d, %s, 0x%p]", GetThreadID(), pTask->m_dwUniqueID, pTask->m_strTaskName.c_str(), pTask->m_ptrClientTask);
                delete pTask;
                pTask = NULL;
                m_lstTmpTasks.erase(it++);
                bret |= true;
            }
            else it++;
        }
        m_mutexTmpTasks.Unlock();

        if (bret) return bret;

        m_mutexTasks.Lock();
        it = m_lstTasks.begin();
        while (it != m_lstTasks.end())
        {
            SIdleThreadTask* pTask = *it;
            if ( pTask && pClient == pTask->m_pClient && ( hanlder == NULL || hanlder == pTask->m_ptrClientTask ) )
            {
                DEBUG_LOG(g_logger, "[%d] cancel task[%d, %s, 0x%p]", GetThreadID(), pTask->m_dwUniqueID, pTask->m_strTaskName.c_str(), pTask->m_ptrClientTask);
                delete pTask;
                pTask = NULL;
                m_lstTasks.erase(it++);
                bret |= true;
            }
            else it++;
        }
        m_mutexTasks.Unlock();

        return bret;
    }
#endif

    bool CIdleThread::CancelTask(_ldword dwUniqueID)
    {
        list<SIdleThreadTask*>::iterator it;
        bool bret = false;

        m_mutexTmpTasks.Lock();
        it = m_lstTmpTasks.begin();
        while (it != m_lstTmpTasks.end())
        {
            SIdleThreadTask* pTask = *it;
            if (pTask && dwUniqueID == pTask->m_dwUniqueID)
            {
                DEBUG_LOG(g_logger, "[%d] cancel task[%d, %s]", GetThreadID(), pTask->m_dwUniqueID, pTask->m_strTaskName.c_str());
                delete pTask;
                pTask = NULL;
                m_lstTmpTasks.erase(it++);
                bret = true;
            }
            else it++;
        }
        m_mutexTmpTasks.Unlock();

        if (bret) return bret;

        m_mutexTasks.Lock();
        it = m_lstTasks.begin();
        while (it != m_lstTasks.end())
        {
            SIdleThreadTask* pTask = *it;
            if (pTask && dwUniqueID == pTask->m_dwUniqueID)
            {
                DEBUG_LOG(g_logger, "[%d] cancel task[%d, %s]", GetThreadID(), pTask->m_dwUniqueID, pTask->m_strTaskName.c_str());
                delete pTask;
                pTask = NULL;
                m_lstTasks.erase(it++);
                bret = true;
            }
            else it++;
        }
        m_mutexTasks.Unlock();

        return bret;
    }

    bool CIdleThread::RegisterTaskCompareFn(_ldword dwTID, FnTaskCompare pFunc)
    {
        bool bret = true;
        m_mutexFnTaskCmp.Lock();
        if (m_mapfnTaksCmp.find(dwTID) != m_mapfnTaksCmp.end())
            bret = false;
        else
            m_mapfnTaksCmp[dwTID] = pFunc;
        m_mutexFnTaskCmp.Unlock();
        return bret;
    }

    bool CIdleThread::UnRegisterTaskCompareFn(_ldword dwTID)
    {
        bool bret = true;
        m_mutexFnTaskCmp.Lock();
        map<_ldword, FnTaskCompare>::iterator it = m_mapfnTaksCmp.find(dwTID);
        if (it == m_mapfnTaksCmp.end())
            bret = false;
        else
            m_mapfnTaksCmp.erase(it);
        m_mutexFnTaskCmp.Unlock();
        return bret;
    }

    bool CIdleThread::IsTaskExist(const SIdleThreadTask& task)
    {
        FnTaskCompare fnCmp = FnDefaultTaskCompare;

        m_mutexFnTaskCmp.Lock();
        if (m_mapfnTaksCmp.find(task.m_dwTID) != m_mapfnTaksCmp.end())
            fnCmp = m_mapfnTaksCmp[task.m_dwTID];
        m_mutexFnTaskCmp.Unlock();

        bool bret = IsTaskExist(task, fnCmp);
        if (bret)
        {
#ifdef _MSC_VER
            DEBUG_LOG(g_logger, "[%d] same task[%d, %s, %d, %d, 0x%p, 0x%p, 0x%p]", GetThreadID(),
                      task.m_dwUniqueID, task.m_strTaskName.c_str(), task.m_pParam, task.m_eTaskType, task.m_ptrClientTask, task.m_ptrStaticTask, task.m_ptrThreadTask);
#else
            DEBUG_LOG(g_logger, "[%d] same task[%d, %s, %d, %d, 0x%p]", GetThreadID(),
                                  task.m_dwUniqueID, task.m_strTaskName.c_str(), task.m_pParam, task.m_eTaskType, task.m_ptrThreadTask);
#endif
        }
        return bret;
    }

    bool CIdleThread::IsTaskExist(const SIdleThreadTask& task, FnTaskCompare pfunc)
    {
        list<SIdleThreadTask*>::iterator it;
        bool bret = false;

        m_mutexTmpTasks.Lock();
        for (it = m_lstTmpTasks.begin(); it != m_lstTmpTasks.end(); it++)
        {
            SIdleThreadTask* pTask = *it;
            if ((*pfunc)(task, *pTask))
            {
                bret = true;
                break;
            }
        }
        m_mutexTmpTasks.Unlock();

        if (bret) return bret;

        m_mutexTasks.Lock();
        for (it = m_lstTasks.begin(); it != m_lstTasks.end(); it++)
        {
            SIdleThreadTask* pTask = *it;
            if ((*pfunc)(task, *pTask))
            {
                bret = true;
                break;
            }
        }
        m_mutexTasks.Unlock();

        return bret;
    }

//////////////////////////////////////////////////////////////////////////
#define MAX_THREAD_NUMS     5

    int CIdleThreadManager::s_nLastThreadIndex = -1;

//////////////////////////////////////////////////////////////////////////
    CIdleThreadManager::CIdleThreadManager(int nThreadNums) : CThread(true)
    {
        SetThreadNums(nThreadNums);
        m_pThreads = new CIdleThread* [m_nThreadNums];
        for (int i = 0; i < m_nThreadNums; i++)
        {
            m_pThreads[i] = new CIdleThread(this);
        }
        DEBUG_LOG(g_logger, "create. thread nums[%d]", m_nThreadNums);
        Resume();
    }

    CIdleThreadManager::~CIdleThreadManager()
    {
        for (int i = 0; i < m_nThreadNums; i++)
            delete m_pThreads[i];
        delete m_pThreads;

        Stop();
    }

    int CIdleThreadManager::GetThreadNums()
    {
        return m_nThreadNums;
    }

    void CIdleThreadManager::SetThreadNums(int nNum)
    {
        if (nNum < 1)               nNum = 1;
        if (nNum > MAX_THREAD_NUMS) nNum = MAX_THREAD_NUMS;
        m_nThreadNums = nNum;
    }

    _ldword CIdleThreadManager::RunTask(SIdleThreadTask task, int nThreadIndex /*= -1*/)
    {
        if (task.m_bKeepSingle)
        {
            if (nThreadIndex >= 0 && nThreadIndex < m_nThreadNums)
            {
                if (m_pThreads[nThreadIndex]->IsTaskExist(task))
                {
                    return 0;
                }
            }
            else
            {
                for (int i = 0; i < m_nThreadNums; i++)
                {
                    if (m_pThreads[i]->IsTaskExist(task))
                    {
                        return 0;
                    }
                }
            }
        }

        _ldword dwUniqueID = 0;
        if (nThreadIndex >= 0 && nThreadIndex < m_nThreadNums)
        {
            dwUniqueID = m_pThreads[nThreadIndex]->RunTask(task);
            m_mapTaskThreadMap[dwUniqueID] = nThreadIndex;
        }
        else
        {
#if 0
            s_nLastThreadIndex++;
            if (s_nLastThreadIndex >= m_nThreadNums)
                s_nLastThreadIndex = 0;
#else
            pair<int, int> minPlayLoad; // <thread index, payload>
            minPlayLoad.first = 0;
            minPlayLoad.second = m_pThreads[0]->GetTaskRemain();
            for (int i = 0; i < m_nThreadNums; i++)
            {
                if (minPlayLoad.second > m_pThreads[i]->GetTaskRemain())
                {
                    minPlayLoad.second = m_pThreads[i]->GetTaskRemain();
                    minPlayLoad.first = i;
                }
            }
            s_nLastThreadIndex = minPlayLoad.first;
#endif
            dwUniqueID = m_pThreads[s_nLastThreadIndex]->RunTask(task);
            m_mapTaskThreadMap[dwUniqueID] = s_nLastThreadIndex;
        }
        return dwUniqueID;
    }

#ifdef _MSC_VER
    bool CIdleThreadManager::CancelTask(IIdleTreadClient* pClient, PTR_CLIENT_IDLETHREAD_TASKHANDLER hanlder /*= NULL*/, int nThreadIndex /*= -1*/)
    {
        bool bret = false;
        if (nThreadIndex >= 0 && nThreadIndex < m_nThreadNums)
        {
            bret = m_pThreads[nThreadIndex]->CancelTask(pClient, hanlder);
        }
        else
        {
            for (int i = 0; i < m_nThreadNums; i++)
            {
                bret |= m_pThreads[i]->CancelTask(pClient, hanlder);
            }
        }
        return true;
    }
#endif

    bool CIdleThreadManager::CancelTask(_ldword dwUniqueID, int nThreadIndex /*= -1*/)
    {
        bool bret = false;
        if (nThreadIndex >= 0 && nThreadIndex < m_nThreadNums)
        {
            bret = m_pThreads[nThreadIndex]->CancelTask(dwUniqueID);
        }
        else
        {
            map<_ldword, int>::iterator it = m_mapTaskThreadMap.find(dwUniqueID);
            if (it != m_mapTaskThreadMap.end())
                bret |= m_pThreads[it->second]->CancelTask(dwUniqueID);
        }
        return bret;
    }

    bool CIdleThreadManager::RegisterTaskCompareFn(_ldword dwTID, FnTaskCompare pFunc)
    {
        bool bret = true;
        for (int i = 0; i < m_nThreadNums; i++)
            bret &= m_pThreads[i]->RegisterTaskCompareFn(dwTID, pFunc);
        return bret;
    }

    bool CIdleThreadManager::UnRegisterTaskCompareFn(_ldword dwTID)
    {
        bool bret = true;
        for (int i = 0; i < m_nThreadNums; i++)
            bret &= m_pThreads[i]->UnRegisterTaskCompareFn(dwTID);
        return bret;
    }

    int CIdleThreadManager::HandleMessage(msgid_t msgid, wparam_t wparam, lparam_t lparam)
    {
        if (msgid == WM_TASK_FINISH)
        {
            m_mapTaskThreadMap.erase(wparam);
        }
        return 0;
    }

    int CIdleThreadManager::GetLastThreadIndex()
    {
        return s_nLastThreadIndex;
    }
}
