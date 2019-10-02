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

    ldword GenerateUniqueID()
    {
        static int nIndex = 0;
        return (ldword)Time::CurrentTimeStamp() + nIndex++;
    }

    string DumpTaskInfo(const SIdleThreadTask* pTask, bool bBrief /*= false*/)
    {
        if (pTask == NULL) return "";

        string strText;
        if (bBrief)
            strText = strtool::format("task info[0x%p]: uid:%d, tid:%d, name:%s", pTask, pTask->m_dwUniqueID, pTask->m_dwTID, pTask->m_strTaskName.c_str());
        else
            strText = strtool::format(
#ifdef _MSC_VER
                             "task info[0x%p]: uid:%d, tid:%d, name:%s, param:0x%p, result:0x%p, keep single:%d, sync:%d, task type: %d, client task:0x%p, static task:0x%p, thread task:0x%p",
#else
                             "task info[0x%p]: uid:%d, tid:%d, name:%s, param:0x%p, result:0x%p, keep single:%d, sync:%d, thread task:0x%p",
#endif
                             pTask,
                             pTask->m_dwUniqueID,
                             pTask->m_dwTID,
                             pTask->m_strTaskName.c_str(),
                             pTask->m_pParam,
                             pTask->m_pResult,
                             pTask->m_bKeepSingle,
                             pTask->m_bSyncCallback,
                             pTask->m_eTaskType,
#ifdef _MSC_VER
                             pTask->m_ptrClientTask,
                             pTask->m_ptrStaticTask,
#endif
                             pTask->m_ptrThreadTask);
        return strText;
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
                            DEBUG_LOG(g_logger, "%s[0x%p] run client task. %s", GetName(), GetThreadID(), DumpTaskInfo(pTask, true).c_str());
                            int nRet = (pTask->m_pClient->*(pTask->m_ptrClientTask))(pTask->m_pParam, pTask->m_pResult);
                            if (pTask->m_pClient->GetMessageHanlder() != NULL)
                            {
                                // ::PostMessage may fail when hits system message queue limits (10,000 by default)
                                if (pTask->m_bSyncCallback) ::SendMessage(pTask->m_pClient->GetMessageHanlder(), pTask->m_dwTID, (wparam_t)nRet, (lparam_t)pTask->m_pResult);
                                else                        bPostRet = ::PostMessage(pTask->m_pClient->GetMessageHanlder(), pTask->m_dwTID, (wparam_t)nRet, (lparam_t)pTask->m_pResult);
                                if (bPostRet)
                                {
                                    DEBUG_LOG(g_logger, "%s[0x%p] finish client task. ret:%d, %s", GetName(), GetThreadID(), nRet, DumpTaskInfo(pTask, true).c_str());
                                    if (m_pManager != NULL)
                                        m_pManager->PostMessage(WM_TASK_FINISH, pTask->m_dwUniqueID);
                                }
                                else
                                {
                                    ERROR_LOG(g_logger, "%s[0x%p] post client task fail. ret:%d, post result[%d], last err[%d], %s", GetName(), GetThreadID(), nRet, bPostRet, GetLastError(), DumpTaskInfo(pTask, true).c_str());
                                    m_mapRetryCBNotify[pTask] = nRet;
                                    bDelete = FALSE;
                                }
                            }
                        }
                        break;
                    case SIdleThreadTask::ITT_STATIC:
                        {
                            DEBUG_LOG(g_logger, "%s[0x%p] run static task. %s", GetName(), GetThreadID(), DumpTaskInfo(pTask, true).c_str());
                            int nRet = (*(pTask->m_ptrStaticTask))(pTask->m_pParam, pTask->m_pResult);
                            if (pTask->m_hcbStatic != NULL)
                            {
                                // ::PostMessage may fail when hits system message queue limits (10,000 by default)
                                if (pTask->m_bSyncCallback) ::SendMessage(pTask->m_hcbStatic, pTask->m_dwTID, (wparam_t)nRet, (lparam_t)pTask->m_pResult);
                                else                        bPostRet = ::PostMessage(pTask->m_hcbStatic, pTask->m_dwTID, (wparam_t)nRet, (lparam_t)pTask->m_pResult);
                                if (bPostRet)
                                {
                                    DEBUG_LOG(g_logger, "%s[0x%p] finish static task. ret:%d, %s", GetName(), GetThreadID(), nRet, DumpTaskInfo(pTask, true).c_str());
                                    if (m_pManager != NULL)
                                        m_pManager->PostMessage(WM_TASK_FINISH, pTask->m_dwUniqueID);
                                }
                                else
                                {
                                    ERROR_LOG(g_logger, "%s[0x%p] post static task fail. ret:%d, post result[%d], last err[%d], %s", GetName(), GetThreadID(), nRet, bPostRet, GetLastError(), DumpTaskInfo(pTask, true).c_str());
                                    m_mapRetryCBNotify[pTask] = nRet;
                                    bDelete = FALSE;
                                }
                            }
                        }
                        break;
#endif
                    case SIdleThreadTask::ITT_THREAD:
                        if ( pTask->m_pcbThread != NULL && pTask->m_ptrThreadTask != NULL )
                        {
                            DEBUG_LOG(g_logger, "%s[0x%p] run thread task. %s", GetName(), GetThreadID(), DumpTaskInfo(pTask, true).c_str());
                            int nRet = (pTask->m_pcbThread->*(pTask->m_ptrThreadTask))(pTask->m_pParam, pTask->m_pResult);
                            {
                                if (pTask->m_bSyncCallback) pTask->m_pcbThread->SendMessage(pTask->m_dwTID, (wparam_t)nRet, (lparam_t)pTask->m_pResult);
                                else                        pTask->m_pcbThread->PostMessage(pTask->m_dwTID, (wparam_t)nRet, (lparam_t)pTask->m_pResult);
                                if (m_pManager != NULL) m_pManager->PostMessage(WM_TASK_FINISH, pTask->m_dwUniqueID);
                            }
                            DEBUG_LOG(g_logger, "%s[0x%p] finish thread task. ret:%d, %s", GetName(), GetThreadID(), nRet, DumpTaskInfo(pTask, true).c_str());
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

    void CIdleThread::OnDumpThreadInfo()
    {
        CThread::OnDumpThreadInfo();
        DEBUG_LOG(g_logger, "%s[0x%p] task size[%d]", GetName(), GetThreadID(), GetTaskRemain());
    }

    void CIdleThread::RetryCBNotify()
    {
#ifdef _MSC_VER
        static Time stm;
        Time tm;
        if (tm.BetweenAllMilliSec(stm) < 10) // retry cb notify every 10 ms
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
                            DEBUG_LOG(g_logger, "%s[0x%p] retry post client task. ret:%d, post result[%d], last err[%d], %s", GetName(), GetThreadID(), nRet, bPostRet, GetLastError(), DumpTaskInfo(pTask, true).c_str());
                        }
                        break;
                    case SIdleThreadTask::ITT_STATIC:
                        {
                            bPostRet = ::PostMessage(pTask->m_hcbStatic, pTask->m_dwTID, (wparam_t)nRet, (lparam_t)pTask->m_pResult);
                            DEBUG_LOG(g_logger, "%s[0x%p] retry post static task. ret:%d, post result[%d], last err[%d], %s", GetName(), GetThreadID(), nRet, bPostRet, GetLastError(), DumpTaskInfo(pTask, true).c_str());
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

    ldword CIdleThread::RunTask(const SIdleThreadTask& task)
    {
        if (task.m_bKeepSingle && IsTaskExist(task))
            return 0;

        SIdleThreadTask* pTask = new SIdleThreadTask(task);
        if (pTask == NULL) return -1;

        pTask->m_dwUniqueID = GenerateUniqueID();
        m_mutexTmpTasks.Lock();
        m_lstTmpTasks.push_back(pTask);
        m_mutexTmpTasks.Unlock();

        DEBUG_LOG(g_logger, "%s[0x%p] add task. %s", GetName(), GetThreadID(), DumpTaskInfo(pTask).c_str());

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
                DEBUG_LOG(g_logger, "%s[0x%p] cancel task. %s", GetName(), GetThreadID(), DumpTaskInfo(pTask).c_str());
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
                DEBUG_LOG(g_logger, "%s[0x%p] cancel task. %s", GetName(), GetThreadID(), DumpTaskInfo(pTask).c_str());
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

    bool CIdleThread::CancelTask(ldword dwUniqueID)
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
                DEBUG_LOG(g_logger, "%s[0x%p] cancel task. %s", GetName(), GetThreadID(), DumpTaskInfo(pTask).c_str());
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
                DEBUG_LOG(g_logger, "%s[0x%p] cancel task. %s", GetName(), GetThreadID(), DumpTaskInfo(pTask).c_str());
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

    bool CIdleThread::RegisterTaskCompareFn(ldword dwTID, FnTaskCompare pFunc)
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

    bool CIdleThread::UnRegisterTaskCompareFn(ldword dwTID)
    {
        bool bret = true;
        m_mutexFnTaskCmp.Lock();
        map<ldword, FnTaskCompare>::iterator it = m_mapfnTaksCmp.find(dwTID);
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
        if (bret) DEBUG_LOG(g_logger, "%s[0x%p] same task. %s", GetName(), GetThreadID(), DumpTaskInfo(&task).c_str());
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

        if (bret) DEBUG_LOG(g_logger, "%s[0x%p] same task. %s", GetName(), GetThreadID(), DumpTaskInfo(&task).c_str());
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

    ldword CIdleThreadManager::RunTask(const SIdleThreadTask& task, int nThreadIndex /*= -1*/)
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

        ldword dwUniqueID = 0;
        if (nThreadIndex >= 0 && nThreadIndex < m_nThreadNums)
        {
            dwUniqueID = m_pThreads[nThreadIndex]->RunTask(task);
            m_mtxTaskThreadMap.Lock();
            m_mapTaskThreadMap[dwUniqueID] = nThreadIndex;
            m_mtxTaskThreadMap.Unlock();
        }
        else
        {
#if 0
            s_nLastThreadIndex++;
            if (s_nLastThreadIndex >= m_nThreadNums)
                s_nLastThreadIndex = 0;
#else
            int nRemain = 0;
            pair<int, int> minPlayLoad; // <thread index, payload>
            minPlayLoad.first = 0;
            minPlayLoad.second = m_pThreads[0]->GetTaskRemain();
            for (int i = 1; i < m_nThreadNums; i++)
            {
                nRemain = m_pThreads[i]->GetTaskRemain();
                if (minPlayLoad.second > nRemain)
                {
                    minPlayLoad.second = nRemain;
                    minPlayLoad.first = i;
                }
            }
            s_nLastThreadIndex = minPlayLoad.first;
#endif
            dwUniqueID = m_pThreads[s_nLastThreadIndex]->RunTask(task);
            m_mtxTaskThreadMap.Lock();
            m_mapTaskThreadMap[dwUniqueID] = s_nLastThreadIndex;
            m_mtxTaskThreadMap.Unlock();
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

    bool CIdleThreadManager::CancelTask(ldword dwUniqueID, int nThreadIndex /*= -1*/)
    {
        bool bret = false;
        if (nThreadIndex >= 0 && nThreadIndex < m_nThreadNums)
        {
            bret = m_pThreads[nThreadIndex]->CancelTask(dwUniqueID);
        }
        else
        {
            m_mtxTaskThreadMap.Lock();
            map<ldword, int>::iterator it = m_mapTaskThreadMap.find(dwUniqueID);
            if (it != m_mapTaskThreadMap.end())
                bret |= m_pThreads[it->second]->CancelTask(dwUniqueID);
            m_mtxTaskThreadMap.Unlock();
        }
        return bret;
    }

    bool CIdleThreadManager::RegisterTaskCompareFn(ldword dwTID, FnTaskCompare pFunc)
    {
        bool bret = true;
        for (int i = 0; i < m_nThreadNums; i++)
            bret &= m_pThreads[i]->RegisterTaskCompareFn(dwTID, pFunc);
        return bret;
    }

    bool CIdleThreadManager::UnRegisterTaskCompareFn(ldword dwTID)
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
            m_mtxTaskThreadMap.Lock();
            m_mapTaskThreadMap.erase(wparam);
            m_mtxTaskThreadMap.Unlock();
        }
        return 0;
    }

    int CIdleThreadManager::GetLastThreadIndex()
    {
        return s_nLastThreadIndex;
    }
}
