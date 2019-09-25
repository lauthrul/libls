#pragma once

//////////////////////////////////////////////////////////////////////////
namespace lslib
{
    /// ��������
    struct STaskReq
    {
        time_t                  tReqTime;       // program use. request time
        time_t                  tDealTime;      // program use. deal time
        void*                   pParam;         // task param

        STaskReq() : tReqTime(Time::CurrentTimeStamp()), tDealTime(0), pParam(NULL) {}
        virtual ~STaskReq() {}
    };

    /// ������Ӧ
    struct STaskResp
    {
        time_t                  tRespTime;      // program use. response time
        int                     nResultCode;    // program use
        string                  strResultMsg;   // program use
        void*                   pResultData;    // result data

        STaskResp() : tRespTime(Time::CurrentTimeStamp()), nResultCode(0), pResultData(NULL) {}
        virtual ~STaskResp() {}
    };

    /// ����ID����
#define TID_BASE            1000

}
