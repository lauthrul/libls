#include "stdafx.h"
#include "TaskAgent.h"
#include "TaskManager.h"

//////////////////////////////////////////////////////////////////////////
namespace lslib
{
    CTaskAgent::CTaskAgent(CTaskManager* pManager) : m_pManager(pManager)
    {
    }

    CTaskAgent::~CTaskAgent()
    {
    }

    SInvoker* CTaskAgent::GetInvoker(luint id) const
    {
        map<luint, SInvoker>::const_iterator it = m_mapInvokerMap.find(id);
        if (it == m_mapInvokerMap.end())
            return NULL;
        else
            return const_cast<SInvoker*>(&it->second);
    }

    void CTaskAgent::AddInvoker(const SInvoker& vInvoker)
    {
        m_mapInvokerMap[vInvoker.nID] = vInvoker;
    }

    int CTaskAgent::HandleMessage(msgid_t uMsg, wparam_t wParam, lparam_t lParam)
    {
        m_pManager->PostMessage(uMsg, wParam, lParam);
        return 0;
    }
}