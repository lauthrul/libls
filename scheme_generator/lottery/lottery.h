#pragma once

//////////////////////////////////////////////////////////////////////////
namespace lottery
{
    lstring GetLotteryTemplate(_lpcstr lotteryID);
    bool CheckWin(const SchemeDetail& detail);
    void StatisticSubScheme(__inout SubScheme& subScheme, bool bWin);
    lstring GenereateCode(const SubScheme& subScheme);
}
