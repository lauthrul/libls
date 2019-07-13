/************************************************************************/
/* desc: Basic business data struct definations                         */
/* autor: neco                                                          */
/* date: 2018.02.05                                                     */
/************************************************************************/

#pragma once
#include "TaskDefine.h"

//////////////////////////////////////////////////////////////////////////
// ��������
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
// ��������
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
// ��ȡ��ʷ������
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
    MapHistoryCode          mapHistoryCode;
};

struct SHistroyCodeResp : public SNetResp, public SHistroyCode
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
};

//////////////////////////////////////////////////////////////////////////
// url defines
#define URL_GetLotteryCfg                   "http://127.0.0.1:8000/service/scheme/get_lottery_cfg/?merchant_code=XYYL&uid=xxx"
#define URL_GetLotteryOpenCode              "https://www.ds98.net/dscagamesclient/issue.do?method=recentlyCode"
