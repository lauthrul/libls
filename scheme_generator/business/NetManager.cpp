/************************************************************************/
/* desc: Business Manager                                               */
/* autor: neco                                                          */
/* date: 2018.02.06                                                     */
/************************************************************************/

#include "stdafx.h"
#include "NetManager.h"

//////////////////////////////////////////////////////////////////////////
#define NET_RETRY_MAX                       2
#define MAX_PROCESS_COUNT                   5
#define REQUEST_ISSUE_INFO_INTERVAL         25  // seconds
#define REQUEST_ORDER_DETAIL_INTERVAL       5   // seconds
#define MAX_BATCH_COUNTS_PER_REQUEST        10


//////////////////////////////////////////////////////////////////////////
CNetManager::CNetManager()
{
    InitNetAgent(DEFAULT_AGENT_COUNT);
    Resume();
}

CNetManager::~CNetManager()
{
    Stop();
    ReleaseNetAgent();
}

void CNetManager::InitNetAgent(int nAgentCounts)
{
    if (nAgentCounts <= 0) return;

    m_pAgents = new(nothrow) CTaskAgent* [nAgentCounts];
    if (m_pAgents != NULL)
    {
        for (int i = 0; i < nAgentCounts; i++)
        {
            m_pAgents[i] = new CNetAgent(this);
            m_pAgents[i]->InitInvoker();
        }
        m_nAgentCounts = nAgentCounts;
    }
}

void CNetManager::ReleaseNetAgent()
{
    if (m_pAgents != NULL)
    {
        for (int i = 0; i < m_nAgentCounts; i++)
        {
            if (m_pAgents[i] != NULL)
            {
                m_pAgents[i]->Stop();
                delete m_pAgents[i];
            }
        }
        delete[] m_pAgents;
    }
}

bool CNetManager::IsSameTask(STask* plhs, STask* prhs)
{
    if (plhs->dwTID != prhs->dwTID)         return false;

    bool bret = false;
    switch (prhs->dwTID)
    {
        case TID_GET_LOTTERY_CFG:               return true; // no parameter
        case TID_GET_HISTORY_CODE:              return (*((SHistoryCodeReq*)plhs->pReq) == *((SHistoryCodeReq*)prhs->pReq));
    }
    return bret;
}

int CNetManager::GetLotteryCfgRequest()
{
    return Invoke(GetNextAgent(), TID_GET_LOTTERY_CFG);
}

int CNetManager::GetHistoryCodeRequest(_lpcstr lpstrLottery, int nIssues)
{
    return Invoke(GetNextAgent(), TID_GET_HISTORY_CODE, new SHistoryCodeReq(lpstrLottery, nIssues)); // remember to delete
}

int CNetManager::OnHandleMessage(STask* pTask, __inout bool& bRecycle, __inout bool& bNotice)
{
    switch (pTask->dwTID)
    {
        case TID_GET_LOTTERY_CFG:
        case TID_GET_HISTORY_CODE:
            if (pTask->pResp->nResultCode == ERR_SERVER_ERROR) // network error. retry get user login state 3 times
            {
                if (pTask->nRetryTimes < NET_RETRY_MAX)
                {
                    m_lstRetryTask.push_back(pTask);
                    bNotice = false;
                    bRecycle = false;
                    pTask->nRetryTimes++;
                }
            }
            break;
    }
    return 0;
}
