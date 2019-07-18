/************************************************************************/
/* desc: Task Manager                                                   */
/* autor: neco                                                          */
/* date: 2018.02.06                                                     */
/************************************************************************/

#include "stdafx.h"
#include "TaskManager.h"

//////////////////////////////////////////////////////////////////////////
#define TASK_RETRY_INTERVAL                 5   // seconds
#define MEM_CLEAN_INTERVAL                  60  // seconds

//////////////////////////////////////////////////////////////////////////
CTaskManager::CTaskManager() : CThread(true), m_pAgents(NULL), m_nAgentCounts(0)
{
}

CTaskManager::~CTaskManager()
{
    Stop();
    CleanTasks();
    DealMemClean(true);
}

bool CTaskManager::RegisterCallBack(CThread* pThread)
{
    if (pThread == NULL) return false;
    list<CThread*>::iterator it = find(m_lstCBThreads.begin(), m_lstCBThreads.end(), pThread);
    if (it == m_lstCBThreads.end())
    {
        m_lstCBThreads.push_back(pThread);
        return true;
    }
    return false;
}

bool CTaskManager::UnRegisterCallBack(CThread* pThread)
{
    if (pThread == NULL) return false;
    list<CThread*>::iterator it = find(m_lstCBThreads.begin(), m_lstCBThreads.end(), pThread);
    if (it != m_lstCBThreads.end())
    {
        m_lstCBThreads.erase(it);
        return true;
    }
    return false;
}

void CTaskManager::InitAgent(int nAgentCounts)
{
    throw logic_error("this function should be implemented by subclass.");
}

void CTaskManager::ReleaseAgent()
{
    throw logic_error("this function should be implemented by subclass.");
}

CTaskAgent* CTaskManager::GetNextAgent()
{
    if (m_pAgents == NULL)
    {
        throw logic_error("please call InitAgent() before GetNextAgent()");
        return NULL;
    }

    static int nLastAgentIndex = 0;
    CTaskAgent* pAgent = m_pAgents[nLastAgentIndex++];
    if (nLastAgentIndex >= m_nAgentCounts)
        nLastAgentIndex = 0;
    return pAgent;
}

int CTaskManager::Invoke(CTaskAgent* pAgent, int dwTID, STaskReq* pReq /*= 0*/, bool bKeepSingle /*= true*/)
{
    if (pAgent == NULL) return -2;

    STask* pTask = new STask();
    pTask->dwTID = dwTID;
    pTask->pAgent = pAgent;
    pTask->pReq = pReq;
    pTask->nReqID = GenerateUniqueID();
    pTask->bKeepSingle = bKeepSingle;

    if (bKeepSingle && IsTaskExist(pTask))
    {
        INFO_LOG(g_pLogger, "same task[%d]", pTask->dwTID);
        m_lstTrashbin.push_back(pTask);
        return 0;
    }

    m_mtxTask.Lock();
    m_lstTask.push_back(pTask);
    m_mtxTask.Unlock();

    return pTask->nReqID;
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
    static list<STask*> lstOrderReqTask;

    // deal normal tasks
    m_mtxTask.Lock();
    list<STask*>::iterator it = m_lstTask.begin();
    while (it != m_lstTask.end())
    {
        STask* pTask = *it;
        bool bDealed = false;
        if (pTask->pAgent != NULL)
        {
            SInvoker* pInvoker = pTask->pAgent->GetInvoker(pTask->dwTID);
            if (pInvoker != NULL)
            {
                SIdleThreadTask vIdleTask;
                vIdleTask.m_dwTID = pTask->dwTID;
                vIdleTask.m_eTaskType = SIdleThreadTask::ITT_THREAD;
                vIdleTask.m_strTaskName = pInvoker->strTaskName;
                vIdleTask.m_ptrThreadTask = pInvoker->ptrHandler;
                vIdleTask.m_pcbThread = pTask->pAgent; //
                vIdleTask.m_bSyncCallback = true; // self thread callback, so set sync as true, in case other task pending callback
                vIdleTask.m_pParam = (_ldword)pTask;

                if (pTask->pReq != NULL)
                    pTask->pReq->lDealTime = Time::GetCurDateTime().GetDateTime();
                pTask->pAgent->RunTask(vIdleTask);

                m_mtxWorkMapping.Lock();
                m_mapTaskWorkMapping[pTask] = pTask->pAgent;
                m_mtxWorkMapping.Unlock();

                bDealed = true;
            }
        }
        if (!bDealed)
        {
            ERROR_LOG(g_pLogger, "no invoker for task[%d]", pTask->dwTID);
            m_lstTrashbin.push_back(pTask);
        }
        it = m_lstTask.erase(it);
    }
    m_mtxTask.Unlock();
}

void CTaskManager::DealRetryTask()
{
    long lNow = Time::GetCurDateTime().GetDateTime();

    list<STask*>::iterator it = m_lstRetryTask.begin();
    while (it != m_lstRetryTask.end())
    {
        STask* pTask = *it;
        if (pTask == NULL) continue;

        long lLastTime = 0;
        if (pTask->pReq != NULL)            lLastTime = pTask->pReq->lDealTime;
        else if (pTask->pResp != NULL)      lLastTime = pTask->pResp->lRespTime;
        if (lNow - lLastTime > TASK_RETRY_INTERVAL)
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
    long lNow = Time::GetCurDateTime().GetDateTime();

    _ldword dwSize = 0;
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
            if (lNow - pTask->lRecycleTime > MEM_CLEAN_INTERVAL || bForce)
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
        DEBUG_LOG(g_pLogger, "memory clean. size[%0.3f KB]", (float)dwSize / 1024);
    }
}

void CTaskManager::CleanTasks()
{
    m_mtxTask.Lock();
    while (!m_lstTask.empty())
    {
        list<STask*>::iterator it = m_lstTask.begin();
        (*it)->lRecycleTime = 0; // recyecle immdiately
        m_lstTrashbin.push_back(*it);
        m_lstTask.erase(it);
    }
    m_mtxTask.Unlock();

    while (!m_lstRetryTask.empty())
    {
        list<STask*>::iterator it = m_lstRetryTask.begin();
        (*it)->lRecycleTime = 0; // recyecle immdiately
        m_lstTrashbin.push_back(*it);
        m_lstRetryTask.erase(it);
    }
}

void CTaskManager::Recycle(STask* pTask)
{
    if (pTask == NULL) return;

    pTask->lRecycleTime = Time::GetCurDateTime().GetDateTime();
    m_lstTrashbin.push_back(pTask);
}

void CTaskManager::Notice(SInvoker* pInvoker, wparam_t wParam /*= 0*/, lparam_t lParam /*= 0*/)
{
    if (pInvoker == NULL) return;

    for (list<CThread*>::iterator it = m_lstCBThreads.begin(); it != m_lstCBThreads.end(); it++)
    {
        CThread* pThread = *it;
        if (pThread == NULL) continue;
        pThread->PostMessage(pInvoker->nID, wParam, lParam);
    }
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
