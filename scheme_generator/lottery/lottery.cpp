#include "stdafx.h"
#include "lottery.h"
#include "lottery_ssc.hpp"
#include "lottery_11x5.hpp"
#include "lottery_k3.hpp"
#include "lottery_pk10.hpp"
#include "lottery_3d.hpp"
#include "lottery_pl35.hpp"

//////////////////////////////////////////////////////////////////////////
#define LOTTERY_TEMPLATE_SSC            "ssc"
#define LOTTERY_TEMPLATE_11X5           "11x5"
#define LOTTERY_TEMPLATE_K3             "k3"
#define LOTTERY_TEMPLATE_PK10           "pk10"
#define LOTTERY_TEMPLATE_3D             "3d"
#define LOTTERY_TEMPLATE_PL35           "pl35"

namespace lottery
{
    lstring GetLotteryTemplate(const lstring& lotteryID)
    {
        //TODO: test
        return LOTTERY_TEMPLATE_SSC;
    }

    bool CheckWin(const SchemeDetail& detail)
    {
        if (detail.strOpenCode.empty())
            return false;

        lstring tempate = GetLotteryTemplate(detail.strLottery);
        if (tempate == LOTTERY_TEMPLATE_SSC)            return lottery::ssc::CheckWin(detail);
        else if (tempate == LOTTERY_TEMPLATE_11X5)      return lottery::n11x5::CheckWin(detail);
        else if (tempate == LOTTERY_TEMPLATE_K3)        return lottery::k3::CheckWin(detail);
        else if (tempate == LOTTERY_TEMPLATE_PK10)      return lottery::pk10::CheckWin(detail);
        else if (tempate == LOTTERY_TEMPLATE_3D)        return lottery::n3d::CheckWin(detail);
        else if (tempate == LOTTERY_TEMPLATE_PL35)      return lottery::pl35::CheckWin(detail);
        return false;
    }

    void StatisticSubScheme(__inout SubScheme& subScheme, const SchemeDetail& schemeDetail)
    {
        if (schemeDetail.bWin)
        {
            subScheme.nWinRounds++;
            subScheme.nCombWinRounds++;
            subScheme.nCombLossRounds = 0;
            subScheme.nMaxCombWinRounds = max(subScheme.nMaxCombWinRounds, subScheme.nCombWinRounds);
        }
        else if (schemeDetail.nRoundIndex >= schemeDetail.nRoundTotal - 1)
        {
            subScheme.nCombWinRounds = 0;
            subScheme.nCombLossRounds++;
            subScheme.nMaxCombLossRounds = max(subScheme.nMaxCombLossRounds, subScheme.nCombLossRounds);
        }
        subScheme.dAccuracy = (double)subScheme.nWinRounds / subScheme.nRounds * 100;
    }

    lstring GenereateCode(const SubScheme& subScheme)
    {
        lstring tempate = GetLotteryTemplate(subScheme.strLottery);
        if (tempate == LOTTERY_TEMPLATE_SSC)            return lottery::ssc::GenereateCode(subScheme.strPlayName, subScheme.strFormula, subScheme.dExpectedAccuracy);
        else if (tempate == LOTTERY_TEMPLATE_11X5)      return lottery::n11x5::GenereateCode(subScheme.strPlayName, subScheme.strFormula, subScheme.dExpectedAccuracy);
        else if (tempate == LOTTERY_TEMPLATE_K3)        return lottery::k3::GenereateCode(subScheme.strPlayName, subScheme.strFormula, subScheme.dExpectedAccuracy);
        else if (tempate == LOTTERY_TEMPLATE_PK10)      return lottery::pk10::GenereateCode(subScheme.strPlayName, subScheme.strFormula, subScheme.dExpectedAccuracy);
        else if (tempate == LOTTERY_TEMPLATE_3D)        return lottery::n3d::GenereateCode(subScheme.strPlayName, subScheme.strFormula, subScheme.dExpectedAccuracy);
        else if (tempate == LOTTERY_TEMPLATE_PL35)      return lottery::pl35::GenereateCode(subScheme.strPlayName, subScheme.strFormula, subScheme.dExpectedAccuracy);
        return "";
    }
}
