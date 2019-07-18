#pragma once

//////////////////////////////////////////////////////////////////////////
namespace lottery
{
    string GetLotteryTemplate(_lpcstr lotteryID);
    bool CheckWin(const SchemeDetail& detail);
    void StatisticSubScheme(__inout__ SubScheme& subScheme, const SchemeDetail& schemeDetail);
    string GenereateCode(const SubScheme& subScheme);
}
