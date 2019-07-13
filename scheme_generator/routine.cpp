#include "stdafx.h"
#include "routine.h"
#include "lottery/lottery.h"

//////////////////////////////////////////////////////////////////////////
#define ROUTINE_INTERVAL            5   // second
#define HISTORY_CODE_CACHE_SIZE     100
enum
{
    TID_HISTORY_CODE_UPDATED = TID_BASE + 2000,
};

CRoutine::CRoutine() : CThread(true)
{
}

CRoutine::~CRoutine()
{

}

void CRoutine::Run()
{
    Resume();
    CThread::Run();
}

void CRoutine::OnExecute()
{
    Routine();
}

int CRoutine::HandleMessage(msgid_t uMsg, wparam_t wParam /*= 0*/, lparam_t lParam /*= 0*/)
{
    STask* pTask = (STask*)lParam;
    if (pTask == NULL || pTask->pResp == NULL) return -1;

    switch (uMsg)
    {
    case TID_GET_HISTORY_CODE:
        {
            SHistroyCodeResp* pResp = (SHistroyCodeResp*)pTask->pResp;
            if (pResp != NULL && pResp->nResultCode == 0)
            {
                lstring& rfLottery = pResp->strLottery;
                MapHistoryCode& rfHistoryResp = pResp->mapHistoryCode;
                MapHistoryCode& rfHistory = m_mapAllHistoryCode[pResp->strLottery];

                bool bNewResult = false;
                MapHistoryCode newHistory;
                if (rfHistory.empty())
                    newHistory = rfHistoryResp;
                else
                {
                    SHistoryCodeItem codeBegin = rfHistoryResp.begin()->second;
                    MapHistoryCode::iterator it = rfHistory.find(codeBegin.strIssue);
                    if (it == rfHistory.end())
                        newHistory = rfHistoryResp;
                    else
                    {
                        size_t dist = std::distance(it, rfHistory.end());
                        if (dist <= rfHistoryResp.size())
                        {
                            it = rfHistoryResp.begin();
                            std::advance(it, dist);
                            if (it != rfHistoryResp.end())
                            {
                                for (MapHistoryCode::iterator idx = it; idx != rfHistoryResp.end(); idx++)
                                    newHistory[idx->first] = idx->second;
                            }
                        }
                    }
                }

                if (!newHistory.empty())
                {
                    if (rfHistory.size() > HISTORY_CODE_CACHE_SIZE)
                    {
                        int nIndex = rfHistory.size() - HISTORY_CODE_CACHE_SIZE;
                        while (nIndex > 0)
                        {
                            rfHistory.erase(rfHistory.begin());
                            nIndex--;
                        }
                    }
                    for (MapHistoryCode::iterator it = newHistory.begin(); it != newHistory.end(); it++)
                        rfHistory[it->first] = it->second;
                    PostMessage(TID_HISTORY_CODE_UPDATED, (wparam_t)new lstring(rfLottery), (lparam_t)new MapHistoryCode(newHistory)); // remember to delete
                    INFO_LOG(g_pLogger, "彩种[%s]获取到[%d]期新的开奖号! 最新奖期[%s]", rfLottery.c_str(), newHistory.size(), newHistory.rbegin()->first.c_str());
                }
            }
        }
        break;
    case TID_HISTORY_CODE_UPDATED:
        {
            auto_ptr<lstring> apLottery((lstring*)wParam);
            auto_ptr<MapHistoryCode> apNewHistorys((MapHistoryCode*)lParam);

            map<int, SubScheme> mapSubSchemes;
            g_dbWrapper.GetSubSchemesByLottery(mapSubSchemes, apLottery->c_str());
            for (map<int, SubScheme>::iterator it = mapSubSchemes.begin(); it != mapSubSchemes.end(); it++)
            {
                SubScheme& rfSubScheme = it->second;
                map<int, SchemeDetail> mapSchemeDetails;
                g_dbWrapper.GetSchemeDetailsBySubSchemeID(mapSchemeDetails, rfSubScheme.nID, apNewHistorys->begin()->first.c_str());

                SchemeDetail* pLastSchemeDetail = NULL;
                auto_ptr<SchemeDetail> apLastSchemeDetail;
                MapHistoryCode::iterator it_hlast = apNewHistorys->end();
                if (!mapSchemeDetails.empty())
                {
                    SchemeDetail lastSchemeDetail = mapSchemeDetails.rbegin()->second; // 找到最后一期记录
                    it_hlast = apNewHistorys->find(lastSchemeDetail.strIssue);
                    if (lastSchemeDetail.strOpenCode.empty() && it_hlast != apNewHistorys->end()) // 未开奖，且在当前新奖期范围内
                    {
                        lastSchemeDetail.strOpenCode = it_hlast->second.strCode;
                        lastSchemeDetail.bWin = lottery::CheckWin(lastSchemeDetail);
                        lottery::StatisticSubScheme(rfSubScheme, lastSchemeDetail.bWin);

                        g_dbWrapper.UpdateSchemeDetail(lastSchemeDetail);

                        pLastSchemeDetail = new SchemeDetail(lastSchemeDetail);
                        apLastSchemeDetail = auto_ptr<SchemeDetail>(apLastSchemeDetail);

                        it_hlast++;
                    }
                }
                
                if (it_hlast == apNewHistorys->end())
                    it_hlast = apNewHistorys->begin();

                int nIndex = 0;
                for (MapHistoryCode::iterator it_h = it_hlast; it_h != apNewHistorys->end(); it_h++)
                {
                    const SHistoryCodeItem& rfHistoryCode = it_h->second;
                    SchemeDetail detail;
                    detail.nID = nIndex++; // no use. only used as key
                    detail.strLottery = apLottery->c_str();
                    detail.strIssue = rfHistoryCode.strIssue;
                    detail.strPlayKind = rfSubScheme.strPlayKind;
                    detail.strPlayName = rfSubScheme.strPlayName;
                    detail.nDWDPos = rfSubScheme.nDWDPos;
                    detail.nRoundTotal = rfSubScheme.nIssuesPerRound;
                    if (pLastSchemeDetail != NULL && !pLastSchemeDetail->bWin)
                    {
                        detail.nRoundIndex = (pLastSchemeDetail->nRoundIndex + 1) % detail.nRoundTotal;
                        if (detail.nRoundIndex == 0)
                            detail.strCode = lottery::GenereateCode(rfSubScheme);
                        else
                            detail.strCode = pLastSchemeDetail->strCode;
                    }
                    else
                    {
                        detail.nRoundIndex = 0;
                        detail.strCode = lottery::GenereateCode(rfSubScheme);
                    }
                    detail.strOpenCode = rfHistoryCode.strCode;
                    detail.nMerchantID = rfSubScheme.nMerchantID;
                    detail.nSchemeID = rfSubScheme.nSchemeID;
                    detail.nSubSchemeID = rfSubScheme.nID;
                    detail.bWin = lottery::CheckWin(detail);
                    mapSchemeDetails[detail.nID] = detail;
                    pLastSchemeDetail = &mapSchemeDetails[detail.nID];

                    rfSubScheme.nIssues++;
                    if (detail.nRoundIndex == 0) rfSubScheme.nRounds++;
                    lottery::StatisticSubScheme(rfSubScheme, detail.bWin);
                }
                g_dbWrapper.AddSchemeDetails(mapSchemeDetails);
                g_dbWrapper.UpdateSubScheme(rfSubScheme);
            }
        }
        break;
    }
    return 0;
}

void CRoutine::Routine()
{
    static long tm = 0;
    long lnow = Time::GetCurDateTime().GetDateTime();
    if (lnow - tm < ROUTINE_INTERVAL) return;

    INFO_LOG(g_pLogger, "正在获取开奖... 上次获取时间: %ld", lnow);
    tm = lnow;

    lstring_list lostterys;
    g_dbWrapper.GetSchemeLotterys(lostterys);

    int issues = (m_mapAllHistoryCode.empty()) ? 50 : 1;
    for (lstring_list::iterator it = lostterys.begin(); it != lostterys.end(); it++)
        g_netManager.GetHistoryCodeRequest(*it, issues);
}
