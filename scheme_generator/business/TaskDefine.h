#pragma once

//////////////////////////////////////////////////////////////////////////
// 任务请求
struct STaskReq
{
    long                    lReqTime;       // program use. request time
    long                    lDealTime;      // program use. deal time
    void*                   pParam;         // task param

    STaskReq() : lReqTime(Time::GetCurDateTime().GetDateTime()), lDealTime(0), pParam(NULL) {}
    virtual ~STaskReq() {}
};

// 任务响应
struct STaskResp
{
    long                    lRespTime;      // program use. response time
    int                     nResultCode;    // program use
    lstring                 strResultMsg;   // program use
    void*                   pResultData;    // result data

    STaskResp() : lRespTime(Time::GetCurDateTime().GetDateTime()), nResultCode(0), pResultData(NULL) {}
    virtual ~STaskResp() {}
};


//////////////////////////////////////////////////////////////////////////
#define TID_BASE            1000
