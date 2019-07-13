/************************************************************************/
/* desc: Basic business data struct definations                         */
/* autor: neco                                                          */
/* date: 2018.02.05                                                     */
/************************************************************************/

#pragma once
#include "TaskDefine.h"

//////////////////////////////////////////////////////////////////////////
// 网络请求
typedef STaskReq        SNetReq;
typedef STaskResp       SNetResp;

struct SSimpleNetReq : public SNetReq
{
    lstring                 strParam;
    SSimpleNetReq() {}
    SSimpleNetReq(const lstring& param) : strParam(param) {}
    bool operator== (const SSimpleNetReq& rhs)
    {
        if (strParam == rhs.strParam)
            return true;
        return false;
    }
};

struct SSimpleNetResp : public SNetResp
{
    lstring                  strData;
    SSimpleNetResp() {}
    SSimpleNetResp(const lstring& data) : strData(data) {}
};

//////////////////////////////////////////////////////////////////////////
// 彩种配置
struct SLotteryCfg
{
    lstring strLotteryID;
    lstring strName;

    SLotteryCfg() {};
    SLotteryCfg(_lpcstr id, _lpcstr name) : strLotteryID(id), strName(name) {};
};
typedef map<lstring, SLotteryCfg>       MapLotteryCfg;     // <lottery id, SLotteryCfg>

struct SLotteryCfgResp : public SNetResp
{
    MapLotteryCfg           mapLotteryCfg;
};

//////////////////////////////////////////////////////////////////////////
// 获取历史开奖号
struct SHistoryCodeReq : public SNetReq
{
    lstring                 strLottery;
    int                     nIssueCounts;

    SHistoryCodeReq() {}
    SHistoryCodeReq(lstring lottery, int counts) : strLottery(lottery), nIssueCounts(counts) {}
    bool operator== (const SHistoryCodeReq& rhs)
    {
        if (strLottery == rhs.strLottery && nIssueCounts == rhs.nIssueCounts)
            return true;
        return false;
    }
};

struct SHistoryCodeItem
{
    lstring                  strCode;
    lstring                  strIssue;

    SHistoryCodeItem() {}
    SHistoryCodeItem(const lstring& issue, const lstring& code) : strIssue(issue), strCode(code) {}
};
typedef map<lstring, SHistoryCodeItem>       MapHistoryCode;     // <issue, SHistoryCodeItem>
typedef map<lstring, MapHistoryCode>         MapAllHistoryCode;  // <lottery, MapHistoryCode>


struct SHistroyCode
{
    lstring                 strLottery;
    lstring                 strCurIssue;
    MapHistoryCode          mapHistoryCode;
};

struct SHistroyCodeResp : public SNetResp, public SHistroyCode
{
};

//////////////////////////////////////////////////////////////////////////
// 当前奖期
struct SIssueInfo
{
    lstring                 strLottery;
    lstring                 strIssue;
    Time                    tmStart;
    Time                    tmEnd;
    Time                    tmCurrent;
};

struct SIssueInfoResp : public SNetResp, public SIssueInfo
{
};

//////////////////////////////////////////////////////////////////////////
// net error defines
enum
{
    ERR_MASK = 10000000,

    ERR_UNKOWN = -10000,
    ERR_SERVER_ERROR,
    ERR_SERVER_DATA_EMTYP,
    ERR_SERVER_DATA_INVALID,
    ERR_SERVER_INTVERNAL_ERROR,
    ERR_SERVER_OFFLINE,

    ERR_CUSTOM_BASE = -1000,
};

#define make_mask_err(err)              ((err) + ERR_MASK)
#define reverse_mask_err(maskerr)       ((maskerr) < 0) ? (maskerr) : ((maskerr) - ERR_MASK)

//////////////////////////////////////////////////////////////////////////
// task id defines
enum
{
    TID_GET_LOTTERY_CFG = TID_BASE + 1000,
    TID_GET_HISTORY_CODE,
    TID_GET_ISSUE_INFO,
};

//////////////////////////////////////////////////////////////////////////
// url defines
#define HOST_BACKEND                        "http://127.0.0.1:8000"
#define HOST_PTLINE                         "https://www.dsn98.net"
#define URL_GetLotteryCfg                   HOST_BACKEND "/service/scheme/get_lottery_cfg/?merchant_code=XYYL&uid=xxx"
#define URL_GetLotteryOpenCode              HOST_PTLINE "/dscagamesclient/issue.do?method=recentlyCode"
#define URL_GetLotteryCurrentIssue          HOST_PTLINE "/dscagamesclient/issue.do?method=current"
