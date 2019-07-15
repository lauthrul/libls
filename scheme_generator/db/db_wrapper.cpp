#include "stdafx.h"
#include "db_wrapper.h"

//////////////////////////////////////////////////////////////////////////
CDBWrapper::CDBWrapper()
{
#ifdef MAKE_TEST_DATA
    MakeTestData();
#endif
}

CDBWrapper::CDBWrapper(_lpcstr host, _lpcstr user, _lpcstr passowrd) : CDB(host, user, passowrd)
{
}

CDBWrapper::~CDBWrapper()
{
}

#ifdef MAKE_TEST_DATA
void CDBWrapper::MakeTestData()
{
    // 计划
    Scheme sc;
    sc.nID = 1;
    sc.strName = "金鱼计划";
    sc.strLottery = "17";
    sc.nSubSchemes = 2;
    sc.nMerchantID = 1;
    m_mapSchemes[sc.nID] = sc;

    sc.nID = 2;
    sc.strName = "财神计划";
    sc.strLottery = "29";
    sc.nSubSchemes = 2;
    sc.nMerchantID = 1;
    m_mapSchemes[sc.nID] = sc;

    // 子计划
    SubScheme ssc;
    ssc.nID = 1;
    ssc.strName = "金鱼计划前三直选复式";
    ssc.strLottery = "17";
    ssc.strPlayKind = "qsm";
    ssc.strPlayName = "qsm_zx_fs";
    ssc.nDWDPos = 0;
    ssc.strFormula = "fx = random()";
    ssc.dExpectedAccuracy = 70;
    ssc.nIssuesPerRound = 2;
    ssc.nMerchantID = 1;
    ssc.nSchemeID = 1;
    m_mapSubSchemes[ssc.nID] = ssc;

    ssc.nID = 2;
    ssc.strName = "金鱼计划定位胆";
    ssc.strLottery = "17";
    ssc.strPlayKind = "dwd";
    ssc.strPlayName = "dwd_dwd_dwd";
    ssc.nDWDPos = 0;
    ssc.strFormula = "fx = random()";
    ssc.dExpectedAccuracy = 80;
    ssc.nIssuesPerRound = 1;
    ssc.nMerchantID = 1;
    ssc.nSchemeID = 1;
    m_mapSubSchemes[ssc.nID] = ssc;

    ssc.nID = 3;
    ssc.strName = "财神计划中三直选单式";
    ssc.strLottery = "29";
    ssc.strPlayKind = "zsm";
    ssc.strPlayName = "zsm_zx_ds";
    ssc.nDWDPos = 0;
    ssc.strFormula = "fx = random()";
    ssc.dExpectedAccuracy = 90;
    ssc.nIssuesPerRound = 3;
    ssc.nMerchantID = 1;
    ssc.nSchemeID = 2;
    m_mapSubSchemes[ssc.nID] = ssc;

    ssc.nID = 4;
    ssc.strName = "财神计划定位胆";
    ssc.strLottery = "29";
    ssc.strPlayKind = "dwd";
    ssc.strPlayName = "dwd_dwd_dwd";
    ssc.nDWDPos = 4;
    ssc.strFormula = "fx = random()";
    ssc.dExpectedAccuracy = 85;
    ssc.nIssuesPerRound = 2;
    ssc.nMerchantID = 1;
    ssc.nSchemeID = 2;
    m_mapSubSchemes[ssc.nID] = ssc;
}
#endif

bool CDBWrapper::GetSchemeLotterys(__out lstring_list& lstLotterys)
{
#ifdef MAKE_TEST_DATA
    for (map<int, Scheme>::iterator it = m_mapSchemes.begin(); it != m_mapSchemes.end(); it++)
    {
        const Scheme& sc = it->second;
        if (std::find(lstLotterys.begin(), lstLotterys.end(), sc.strLottery) == lstLotterys.end())
            lstLotterys.push_back(sc.strLottery);
    }
#else
    sql::ResultSet* res = ExcuteQuery("select distinct lottery from service_scheme where state = 1 and sub_schemes > 0");
    while (res != NULL && res->next())
        lstLotterys.push_back(res->getString("lottery"));
    delete res;
#endif
    return !lstLotterys.empty();
}

bool CDBWrapper::GetSubSchemesByLottery(__out map<int, SubScheme>& mapSubSchemes, _lpcstr lottery)
{
#ifdef MAKE_TEST_DATA
    for (map<int, SubScheme>::iterator it = m_mapSubSchemes.begin(); it != m_mapSubSchemes.end(); it++)
    {
        if (it->second.strLottery == lottery)
            mapSubSchemes[it->first] = it->second;
    }
#else
    lstring stmt;
    stmt.format("select * from service_subscheme where lottery = '%s';", lottery);
    sql::ResultSet* res = ExcuteQuery(stmt);
    while (res != NULL && res->next())
    {
        SubScheme sc;
        sc.nID = res->getInt("id");
        sc.strName = res->getString("name").c_str();
        sc.strLottery = res->getString("lottery").c_str();
        sc.strPlayKind = res->getString("play_kind").c_str();
        sc.strPlayName = res->getString("play_name").c_str();
        sc.nDWDPos = res->getInt("dwd_pos");
        sc.strFormula = res->getString("formula").c_str();
        sc.dExpectedAccuracy = res->getDouble("expected_accuracy");
        sc.nIssuesPerRound = res->getInt("issues_per_round");
        sc.nIssues = res->getInt("issues");
        sc.nRounds = res->getInt("rounds");
        sc.nWinRounds = res->getInt("win_rounds");
        sc.nCombWinRounds = res->getInt("comb_win_rounds");
        sc.nMaxCombWinRounds = res->getInt("max_comb_win_rounds");
        sc.nCombLossRounds = res->getInt("comb_loss_rounds");
        sc.nMaxCombLossRounds = res->getInt("max_comb_loss_rounds");
        sc.dAccuracy = res->getDouble("accuracy");
        sc.nMerchantID = res->getInt("merchant_id");
        sc.nSchemeID = res->getInt("scheme_id");
        mapSubSchemes[sc.nID] = sc;
    }
    delete res;
#endif
    return !mapSubSchemes.empty();
}

bool CDBWrapper::GetSchemeDetailsBySubSchemeID(__out map<int, SchemeDetail>& mapSchemeDetails, int subscheme_id, _lpcstr from_issue)
{
#ifdef MAKE_TEST_DATA
    for (map<int, SchemeDetail>::iterator it = m_mapSchemeDetails.begin(); it != m_mapSchemeDetails.end(); it++)
    {
        const SchemeDetail& sd = it->second;
        if (sd.nSubSchemeID == subscheme_id && sd.strIssue >= from_issue)
            mapSchemeDetails[it->first] = it->second;
    }
#else
    lstring stmt;
    stmt.format("select * from service_schemedetail where sub_scheme_id = %d and issue >= '%s'", subscheme_id, from_issue);
    sql::ResultSet* res = ExcuteQuery(stmt);
    while (res != NULL && res->next())
    {
        SchemeDetail detail;
        detail.nID = res->getInt("id");
        detail.strLottery = res->getString("lottery");
        detail.strIssue = res->getString("issue");
        detail.strPlayKind = res->getString("play_kind");
        detail.strPlayName = res->getString("play_name");
        detail.nRoundIndex = res->getInt("round_index");
        detail.nRoundTotal = res->getInt("round_total");
        detail.strCode = res->getString("code");
        detail.strOpenCode = res->getString("open_code");
        detail.bWin = res->getInt("win");
        detail.nMerchantID = res->getInt("merchant_id");
        detail.nSchemeID = res->getInt("scheme_id");
        detail.nSubSchemeID = res->getInt("sub_scheme_id");
        mapSchemeDetails[detail.nID] = detail;
    }
    delete res;
#endif
    return !mapSchemeDetails.empty();
}

bool CDBWrapper::AddSchemeDetails(const map<int, SchemeDetail>& mapSchemeDetails)
{
#ifdef MAKE_TEST_DATA
    int nIndex = m_mapSchemeDetails.size();
    for (map<int, SchemeDetail>::const_iterator it = mapSchemeDetails.begin(); it != mapSchemeDetails.end(); it++)
    {
        SchemeDetail sd = it->second;
        sd.nID = ++nIndex;
        if (m_mapSchemeDetails.find(sd.nID) == m_mapSchemeDetails.end())
            m_mapSchemeDetails[sd.nID] = sd;
    }
    return true;
#else
    //TODO: cannot perform multi-statements on one excute. need to be fixed!
    bool ret = true;
    lstring stmt;
//     stmt = "begin;";
    for (map<int, SchemeDetail>::const_iterator it = mapSchemeDetails.begin(); it != mapSchemeDetails.end(); it++)
    {
        const SchemeDetail& rfDetail = it->second;
        stmt./*append_*/format("insert into service_schemedetail ( \
                           lottery, issue, play_kind, play_name, dwd_pos, round_index, round_total, code, open_code, win, merchant_id, scheme_id, sub_scheme_id, add_time, update_time \
                           ) values ( '%s', '%s', '%s', '%s', %d, %d, %d, '%s', '%s', %d, %d, %d, %d, '%s', '%s' );",
                           rfDetail.strLottery.c_str(),
                           rfDetail.strIssue.c_str(),
                           rfDetail.strPlayKind.c_str(),
                           rfDetail.strPlayName.c_str(),
                           rfDetail.nDWDPos,
                           rfDetail.nRoundIndex,
                           rfDetail.nRoundTotal,
                           rfDetail.strCode.c_str(),
                           rfDetail.strOpenCode.c_str(),
                           rfDetail.bWin,
                           rfDetail.nMerchantID,
                           rfDetail.nSchemeID,
                           rfDetail.nSubSchemeID,
                           Time::GetCurDateTimeStr(true).c_str(),
                           Time::GetCurDateTimeStr(true).c_str());
        ret &= Excute(stmt);
    }
    return ret;
//     stmt.append("commit;");
//     return Excute(stmt);
#endif
}

bool CDBWrapper::UpdateSchemeDetailResult(const SchemeDetail& schemeDetail)
{
#ifdef MAKE_TEST_DATA
    map<int, SchemeDetail>::iterator it = m_mapSchemeDetails.find(schemeDetail.nID);
    if (it != m_mapSchemeDetails.end())
    {
        it->second = schemeDetail;
        return true;
    }
    return false;
#else
    lstring stmt;
    stmt.format("update service_schemedetail set \
                open_code = '%s', \
                win = %d \
                where id = %d",
                schemeDetail.strOpenCode.c_str(),
                schemeDetail.bWin,
                schemeDetail.nID);
    return Excute(stmt);
#endif
}

bool CDBWrapper::UpdateSubSchemeStatistic(const SubScheme& subScheme)
{
#ifdef MAKE_TEST_DATA
    map<int, SubScheme>::iterator it = m_mapSubSchemes.find(subScheme.nID);
    if (it != m_mapSubSchemes.end())
    {
        it->second = subScheme;
        return true;
    }
    return false;
#else
    lstring stmt;
    stmt.format("update service_subscheme set \
                issues = %d, \
                rounds = %d, \
                win_rounds = %d, \
                comb_win_rounds = %d, \
                max_comb_win_rounds = %d, \
                comb_loss_rounds = %d, \
                max_comb_loss_rounds = %d, \
                accuracy = %0.2f \
                where id = %d",
                subScheme.nIssues,
                subScheme.nRounds,
                subScheme.nWinRounds,
                subScheme.nCombWinRounds,
                subScheme.nMaxCombWinRounds,
                subScheme.nCombLossRounds,
                subScheme.nMaxCombLossRounds,
                subScheme.dAccuracy,
                subScheme.nID);
    return Excute(stmt);
#endif
}
