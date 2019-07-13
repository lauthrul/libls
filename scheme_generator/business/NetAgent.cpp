#include "stdafx.h"
#include "NetAgent.h"
#include "NetManager.h"

//////////////////////////////////////////////////////////////////////////
CNetAgent::CNetAgent(CNetManager* pManager) : CTaskAgent(pManager)
{
}

CNetAgent::~CNetAgent()
{

}

void CNetAgent::InitInvoker()
{
    AddInvoker(SInvoker(
                   TID_GET_LOTTERY_CFG,
                   "",
                   "CNetAgent::DoGetLotteryCfg",
                   (PTR_THREAD_IDLETHREAD_TASKHANDLER)&CNetAgent::DoGetLotteryCfg));
    AddInvoker(SInvoker(
                   TID_GET_HISTORY_CODE,
                   "",
                   "CNetAgent::DoGetHistoryCode",
                   (PTR_THREAD_IDLETHREAD_TASKHANDLER)&CNetAgent::DoGetHistoryCode));
}

int CNetAgent::CheckResult(int nHttpCode, _lpcstr lpstrResultText, __out SResultChecker& checker)
{
    int nRet = 0;
    lstring strResultText;
    lstring strMsg;
    CJsonValue jRoot;

    if ((nHttpCode < 200 || nHttpCode >= 300))
    {
        strMsg.format("请求服务器错误【%d】", nHttpCode);
        nRet = ERR_SERVER_ERROR;
        goto label_exit;
    }

    if (is_empty(lpstrResultText))
    {
        strMsg.format("服务器未返回数据");
        nRet = ERR_SERVER_DATA_EMTYP;
        goto label_exit;
    }

    strResultText = crypto::encoding_convert(lpstrResultText, "utf-8", "gb2312");
    if (!CJsonWrapper::Parse(strResultText, &jRoot))
    {
        strMsg.format("服务器返回数据错误：数据无法解析【%s】", strResultText.c_str());
        nRet = ERR_SERVER_DATA_INVALID;
        goto label_exit;
    }

    CJsonValue* pjCode = CJsonWrapper::GetNode(&jRoot, checker.strCodeField);
    if (pjCode == NULL)
    {
        strMsg.format("服务器返回数据错误：字段错误【%s】", strResultText.c_str());
        nRet = ERR_SERVER_DATA_INVALID;
        goto label_exit;
    }

    CJsonValue* pjMsg = CJsonWrapper::GetNode(&jRoot, checker.strMsgField);
    if (pjMsg != NULL && pjMsg->isString())
        strMsg.format("%s", pjMsg->asString().c_str());

    int nCode = 0;
    if (pjCode->isInt()) nCode = pjCode->asInt();
    else if (pjCode->isString()) nCode = atoi(pjCode->asCString());

    if (nCode == -1)
    {
        strMsg.format("您已掉线或未登录！");
        nRet = ERR_SERVER_OFFLINE;
        goto label_exit;
    }

    if (nCode != checker.nSuccessCode)
    {
        nRet = make_mask_err(nCode);
        goto label_exit;
    }

label_exit:
    checker.strErrMsg = strMsg;
    checker.jRoot = jRoot;
    return nRet;
}

int CNetAgent::DoGetLotteryCfg(_ldword pParam, _ldword& pResult)
{
    STask* pTask = (STask*)pParam;
    if (pTask == NULL) return -1;

    SHttpGetParam vParam(URL_GetLotteryCfg);
    SHttpResult vResult = CHttpClient::HttpGet(vParam);

    SResultChecker checker;
    checker.strCodeField = "code";
    checker.nSuccessCode = 1;
    checker.strMsgField = "msg";
    SLotteryCfgResp vResp;
    vResp.nResultCode = CheckResult(vResult.nCode, vResult.strData.c_str(), checker);
    if (vResp.nResultCode != 0)
    {
        int nRet = reverse_mask_err(vResp.nResultCode);
        ERROR_LOG(g_pLogger, "[%dms]get lottery cfg fail: [%d]-%s", vResult.nTimeSpend, nRet, checker.strErrMsg.c_str());
        vResp.strResultMsg.format("[%dms]获取彩种配置失败: [%d]-%s", vResult.nTimeSpend, nRet, checker.strErrMsg.c_str());
        goto label_exit;
    }
    else
    {
        CJsonValue* pValue = CJsonWrapper::GetNode(&checker.jRoot, "data/lotterys");
        if (pValue && pValue->isArray())
        {
            for (CJsonValue::iterator it = pValue->begin(); it != pValue->end(); it++)
            {
                SLotteryCfg cfg;
                cfg.strLotteryID = (*it)["code"].asString();
                cfg.strName = (*it)["name"].asString();
                vResp.mapLotteryCfg[cfg.strLotteryID] = cfg;
            }
        }

        INFO_LOG(g_pLogger, "[%dms]get lottery cfg success. counts: %d", vResult.nTimeSpend, vResp.mapLotteryCfg.size());
        vResp.strResultMsg.format("[%dms]获取奖彩种配置成功! 彩种数：%d", vResult.nTimeSpend, vResp.mapLotteryCfg.size());
    }

label_exit:
    pTask->pResp = new SLotteryCfgResp(vResp); // remember to delete
    pResult = (_ldword)pTask;
    return vResp.nResultCode;
}

int CNetAgent::DoGetHistoryCode(_ldword pParam, _ldword& pResult)
{
    STask* pTask = (STask*)pParam;
    if (pTask == NULL) return -1;

    SHistoryCodeReq* pReq = (SHistoryCodeReq*)pTask->pReq;
    if (pReq == NULL) return -1;

    lstring strUrl, strPost;
    strPost.format("gameid=%s&pageNum=1&size=%d", pReq->strLottery.c_str(), pReq->nIssueCounts);

    SHttpPostParam vParam(URL_GetLotteryOpenCode, strPost);
    SHttpResult vResult = CHttpClient::HttpPost(vParam);

    SResultChecker checker;
    checker.strCodeField = "success";
    checker.nSuccessCode = 1;
    checker.strMsgField = "msg";

    SHistroyCodeResp vResp;
    vResp.strLottery = pReq->strLottery;
    vResp.nResultCode = CheckResult(vResult.nCode, vResult.strData.c_str(), checker);
    if (vResp.nResultCode != 0)
    {
        int nRet = reverse_mask_err(vResp.nResultCode);
        ERROR_LOG(g_pLogger, "[%dms]get code history fail: [%d]-%s", vResult.nTimeSpend, nRet, checker.strErrMsg.c_str());
        vResp.strResultMsg.format("[%dms]获取开奖历史失败: [%d]-%s", vResult.nTimeSpend, nRet, checker.strErrMsg.c_str());
        goto label_exit;
    }
    else
    {
        CJsonValue* pValue = CJsonWrapper::GetNode(&checker.jRoot, "items");
        if (pValue != NULL)
        {
            string strIssue, strCode;
            for (CJsonValue::iterator it = pValue->begin(); it != pValue->end(); it++)
            {
                strCode = (*it)["code"].asString();
                strIssue = (*it)["issue"].asString();
                if (!strIssue.empty() && !strCode.empty())
                    vResp.mapHistoryCode[strIssue] = SHistoryCodeItem(strIssue, strCode);
            }
        }

        INFO_LOG(g_pLogger, "[%dms]get code history success. lottery: %s, counts: %d", vResult.nTimeSpend, vResp.strLottery.c_str(), vResp.mapHistoryCode.size());
        vResp.strResultMsg.format("[%dms]获取开奖历史成功!", vResult.nTimeSpend);
    }

label_exit:
    pTask->pResp = new SHistroyCodeResp(vResp); // remember to delete
    pResult = (DWORD)pTask;
    return vResp.nResultCode;
}
