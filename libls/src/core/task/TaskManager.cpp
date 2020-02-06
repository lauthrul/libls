#include "stdafx.h"
#include "TaskManager.h"
#include <stdexcept>

//////////////////////////////////////////////////////////////////////////
namespace lslib
{
    using namespace logger;

    CTaskManager::CTaskManager() : CThread(true), m_pAgents(NULL), m_nAgentCounts(0)
    {
    }

    CTaskManager::~CTaskManager()
    {
        Stop();
        CleanTasks();
        DealMemClean(true);
    }

#ifdef _MSC_VER
    bool CTaskManager::RegisterCallBack(HWND hWnd)
    {
        m_mtxCBs.Lock();
        bool ret = false;
        list<HWND>::iterator it = find(m_lstCBWnds.begin(), m_lstCBWnds.end(), hWnd);
        if (it == m_lstCBWnds.end())
        {
            m_lstCBWnds.push_back(hWnd);
            ret = true;
        }
        m_mtxCBs.Unlock();
        return ret;
    }

    bool CTaskManager::UnRegisterCallBack(HWND hWnd)
    {
        m_mtxCBs.Lock();
        bool ret = false;
        list<HWND>::iterator it = find(m_lstCBWnds.begin(), m_lstCBWnds.end(), hWnd);
        if (it != m_lstCBWnds.end())
        {
            m_lstCBWnds.erase(it);
            ret = true;
        }
        m_mtxCBs.Unlock();
        return ret;
    }
#endif // _MSC_VER

    bool CTaskManager::RegisterCallBack(CThread* pThread)
    {
        if (pThread == NULL) return false;

        m_mtxCBs.Lock();
        bool ret = false;
        list<CThread*>::iterator it = find(m_lstCBThreads.begin(), m_lstCBThreads.end(), pThread);
        if (it == m_lstCBThreads.end())
        {
            m_lstCBThreads.push_back(pThread);
            ret = true;
        }
        m_mtxCBs.Unlock();
        return ret;
    }

    bool CTaskManager::UnRegisterCallBack(CThread* pThread)
    {
        if (pThread == NULL) return false;

        m_mtxCBs.Lock();
        bool ret = false;
        list<CThread*>::iterator it = find(m_lstCBThreads.begin(), m_lstCBThreads.end(), pThread);
        if (it != m_lstCBThreads.end())
        {
            m_lstCBThreads.erase(it);
            ret = true;
        }
        m_mtxCBs.Unlock();
        return ret;
    }

    void CTaskManager::SetCleanInterval(int nSecond /*= 60*/)
    {
        m_nCleanInterval = MAX(0, nSecond);
    }

    void CTaskManager::SetAgentCounts(int nAgentCounts)
    {
        m_nAgentCounts = CLAMP(nAgentCounts, 1, MAX_AGENT_COUNTS);
    }

//     void CTaskManager::InitAgent(int nAgentCounts)
//     {
//         throw logic_error("this function should be implemented by subclass.");
//     }
// 
//     void CTaskManager::ReleaseAgent()
//     {
//         throw logic_error("this function should be implemented by subclass.");
//     }

    CTaskAgent* CTaskManager::GetNextAgent()
    {
        if (m_pAgents == NULL)
        {
            throw logic_error("please call InitAgent() before GetNextAgent()");
            return NULL;
        }

        static CMutexLock mtx;
        static int nLastAgentIndex = 0;

        mtx.Lock();
        CTaskAgent* pAgent = m_pAgents[nLastAgentIndex++];
        if (nLastAgentIndex >= m_nAgentCounts)
            nLastAgentIndex = 0;
        mtx.Unlock();

        return pAgent;
    }

    luint CTaskManager::Invoke(CTaskAgent* pAgent, luint uTID, STaskReq* pReq /*= 0*/, bool bKeepSingle /*= true*/, luint uDelaySecs /*= 0*/)
    {
        if (pAgent == NULL) return -1;

        STask* pTask = new STask();
        pTask->uTID = uTID;
        pTask->pAgent = pAgent;
        pTask->pReq = pReq;
        pTask->uReqID = GenerateUniqueID();
        pTask->bKeepSingle = bKeepSingle;
        pTask->uDelaySecs = uDelaySecs;

        if (bKeepSingle && IsTaskExist(pTask))
        {
            INFO_LOG(g_logger, "same task[%d]", pTask->uTID);
            m_lstTrashbin.push_back(pTask);
            return 0;
        }

        m_mtxTask.Lock();
        m_lstTask.push_back(pTask);
        m_mtxTask.Unlock();

        INFO_LOG(g_logger, "invoke task[%p, %d] success, reqid[%d]", pTask, pTask->uTID, pTask->uReqID);

        return pTask->uReqID;
    }

    bool CTaskManager::IsTaskExist(STask* pTask)
    {
        if (pTask == NULL) return false;
        bool bret = false;

        // check in task list
        m_mtxTask.Lock();
        list<STask*>::iterator it = m_lstTask.begin();
        for (; it != m_lstTask.end(); it++)
        {
            STask* prfTask = *it;
            if (IsSameTask(prfTask, pTask))
            {
                bret = true;
                break;
            }
        }
        m_mtxTask.Unlock();
        if (bret) return bret;

        // check in task retry list
        it = m_lstRetryTask.begin();
        for (; it != m_lstRetryTask.end(); it++)
        {
            STask* prfTask = *it;
            if (IsSameTask(prfTask, pTask))
            {
                bret = true;
                break;
            }
        }
        if (bret) return bret;

        // check in task working list
        m_mtxWorkMapping.Lock();
        map<STask*, CTaskAgent*>::iterator itx = m_mapTaskWorkMapping.begin();
        for (; itx != m_mapTaskWorkMapping.end(); itx++)
        {
            if (IsSameTask(pTask, itx->first))
            {
                bret = true;
                break;
            }
        }
        m_mtxWorkMapping.Unlock();
        return bret;
    }

    void CTaskManager::OnExecute()
    {
        DealTask();
        DealRetryTask();
        DealMemClean();
    }

    void CTaskManager::DealTask()
    {
        // deal normal tasks
        time_t tNow = Time::CurrentTimeStamp();

        m_mtxTask.Lock();
        list<STask*>::iterator it = m_lstTask.begin();
        while (it != m_lstTask.end())
        {
            STask* pTask = *it;
            if (pTask->pReq != NULL && tNow - pTask->pReq->tReqTime < pTask->uDelaySecs)
            {
                it++;
                continue;
            }

            bool bDealed = false;
            if (pTask->pAgent != NULL)
            {
                SInvoker* pInvoker = pTask->pAgent->GetInvoker(pTask->uTID);
                if (pInvoker != NULL)
                {
                    SIdleThreadTask vIdleTask;
                    vIdleTask.m_dwTID = pTask->uTID;
                    vIdleTask.m_eTaskType = SIdleThreadTask::ITT_THREAD;
                    vIdleTask.m_strTaskName = pInvoker->strTaskName;
                    vIdleTask.m_ptrThreadTask = pInvoker->ptrHandler;
                    vIdleTask.m_pcbThread = pTask->pAgent; //
                    vIdleTask.m_bSyncCallback = true; // self thread callback, so set sync as true, in case other task pending callback
                    vIdleTask.m_pParam = (ldword)pTask;

                    INFO_LOG(g_logger, "running task[%p, %d], reqid[%d]", pTask, pTask->uTID, pTask->uReqID);

                    if (pTask->pReq != NULL) pTask->pReq->tDealTime = tNow;

                    pTask->pAgent->RunTask(vIdleTask);

                    m_mtxWorkMapping.Lock();
                    m_mapTaskWorkMapping[pTask] = pTask->pAgent;
                    m_mtxWorkMapping.Unlock();

                    bDealed = true;
                }
            }
            if (!bDealed)
            {
                ERROR_LOG(g_logger, "no invoker for task[%d]", pTask->uTID);
                m_lstTrashbin.push_back(pTask);
            }
            it = m_lstTask.erase(it);
        }
        m_mtxTask.Unlock();
    }

    void CTaskManager::DealRetryTask()
    {
        time_t tNow = Time::CurrentTimeStamp();

        list<STask*>::iterator it = m_lstRetryTask.begin();
        while (it != m_lstRetryTask.end())
        {
            STask* pTask = *it;
            if (pTask == NULL) continue;

            time_t tLastTime = 0;
            if (pTask->pReq != NULL)            tLastTime = pTask->pReq->tDealTime;
            else if (pTask->pResp != NULL)      tLastTime = pTask->pResp->tRespTime;
            if (tNow - tLastTime > TASK_RETRY_INTERVAL)
            {
                m_mtxTask.Lock();
                m_lstTask.push_back(pTask);
                m_mtxTask.Unlock();

                it = m_lstRetryTask.erase(it);
            }
            else it++;
        }
    }

    void CTaskManager::DealMemClean(bool bForce /*= false*/)
    {
        time_t tNow = Time::CurrentTimeStamp();

        ldword dwSize = 0;
        list<STask*>::iterator it = m_lstTrashbin.begin();
        while (it != m_lstTrashbin.end())
        {
            STask* pTask = *it;
            if (pTask == NULL)
            {
                it = m_lstTrashbin.erase(it);
            }
            else
            {
                if (tNow - pTask->tRecycleTime > m_nCleanInterval || bForce)
                {
                    if (pTask->pReq != NULL)
                    {
                        dwSize += sizeof(*pTask->pReq);
                        delete pTask->pReq;
                        pTask->pReq = NULL;
                    }
                    if (pTask->pResp != NULL)
                    {
                        dwSize += sizeof(*pTask->pResp);
                        delete pTask->pResp;
                        pTask->pResp = NULL;
                    }

                    dwSize += sizeof(*pTask);
                    delete pTask;
                    it = m_lstTrashbin.erase(it);
                }
                else it++;
            }
        }
        if (dwSize > 0)
        {
            DEBUG_LOG(g_logger, "memory clean. size[%0.3f KB]", (float)dwSize / 1024);
        }
    }

    void CTaskManager::CleanTasks()
    {
        m_mtxTask.Lock();
        while (!m_lstTask.empty())
        {
            list<STask*>::iterator it = m_lstTask.begin();
            (*it)->tRecycleTime = 0; // recyecle immdiately
            m_lstTrashbin.push_back(*it);
            m_lstTask.erase(it);
        }
        m_mtxTask.Unlock();

        while (!m_lstRetryTask.empty())
        {
            list<STask*>::iterator it = m_lstRetryTask.begin();
            (*it)->tRecycleTime = 0; // recyecle immdiately
            m_lstTrashbin.push_back(*it);
            m_lstRetryTask.erase(it);
        }
    }

    void CTaskManager::Recycle(STask* pTask)
    {
        if (pTask == NULL) return;

        pTask->tRecycleTime = Time::CurrentTimeStamp();
        m_lstTrashbin.push_back(pTask);
    }

    void CTaskManager::Notice(SInvoker* pInvoker, wparam_t wParam /*= 0*/, lparam_t lParam /*= 0*/)
    {
        if (pInvoker == NULL || pInvoker->nMsgID <= 0) return;

        m_mtxCBs.Lock();
#ifdef _MSC_VER
        for (list<HWND>::iterator it = m_lstCBWnds.begin(); it != m_lstCBWnds.end(); it++)
        {
            ::PostMessage(*it, pInvoker->nMsgID, wParam, lParam);
        }
#endif // _MSC_VER

        for (list<CThread*>::iterator it = m_lstCBThreads.begin(); it != m_lstCBThreads.end(); it++)
        {
            CThread* pThread = *it;
            if (pThread == NULL) continue;
            pThread->PostMessage(pInvoker->nMsgID, wParam, lParam);
        }
        m_mtxCBs.Unlock();
    }

    void CTaskManager::Notice(int nMsgID, wparam_t wParam /*= 0*/, lparam_t lParam /*= 0*/)
    {
        if (nMsgID <= 0) return;

        m_mtxCBs.Lock();
#ifdef _MSC_VER
        for (list<HWND>::iterator it = m_lstCBWnds.begin(); it != m_lstCBWnds.end(); it++)
        {
            ::PostMessage(*it, nMsgID, wParam, lParam);
        }
#endif // _MSC_VER

        for (list<CThread*>::iterator it = m_lstCBThreads.begin(); it != m_lstCBThreads.end(); it++)
        {
            CThread* pThread = *it;
            if (pThread == NULL) continue;
            pThread->PostMessage(nMsgID, wParam, lParam);
        }
        m_mtxCBs.Unlock();
    }

    int CTaskManager::HandleCustomMessage(msgid_t uMsg, wparam_t wParam, lparam_t lParam, __inout__ bool& bHandled)
    {
        return 0;
    }

    int CTaskManager::HandleMessage(msgid_t uMsg, wparam_t wParam, lparam_t lParam)
    {
        // custome message handle
        bool bHandled = false;
        int lResult = HandleCustomMessage(uMsg, wParam, lParam, bHandled);
        if (bHandled) return lResult;

        // private message handle
        STask* pTask = (STask*)lParam;
        if (pTask == NULL || pTask->pResp == NULL) return -1;

        // message handle
        bool bRecycle = true;
        bool bNotice = true;
        int ret = OnHandleMessage(pTask, bRecycle, bNotice);

        // notice to application and recycle tasks
        if (bNotice)    Notice(pTask->pAgent->GetInvoker(uMsg), wParam, lParam);
        if (bRecycle)   Recycle(pTask);

        m_mtxWorkMapping.Lock();
        m_mapTaskWorkMapping.erase(pTask);
        m_mtxWorkMapping.Unlock();

        return ret;
    }

    int CTaskManager::OnHandleMessage(STask* pTask, __inout__ bool& bRecycle, __inout__ bool& bNotice)
    {
        return 0;
    }
}
