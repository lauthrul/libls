#include "stdafx.h"
#include "db_wrapper.h"

//////////////////////////////////////////////////////////////////////////
CDBWrapper::CDBWrapper()
{
#ifdef MAKE_TEST_DATA
    MakeTestData();
#endif
}

CDBWrapper::CDBWrapper(_lpcstr host, _lpcstr user, _lpcstr passowrd, _lpcstr db) : CDB(host, user, passowrd, db)
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

bool CDBWrapper::GetSchemeLotterys(__out__ string_list& lstLotterys)
{
#ifdef MAKE_TEST_DATA
    for (map<int, Scheme>::iterator it = m_mapSchemes.begin(); it != m_mapSchemes.end(); it++)
    {
        const Scheme& sc = it->second;
        if (std::find(lstLotterys.begin(), lstLotterys.end(), sc.strLottery) == lstLotterys.end())
            lstLotterys.push_back(sc.strLottery);
    }
#else
    MYSQL_RES* res = ExcuteQuery("select distinct lottery from service_subscheme where state = 1 and lottery in ( select  distinct lottery from service_scheme where state = 1)");
    if (res == NULL) return false;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)))
    {
        lstLotterys.push_back(row[0]);
    }
    mysql_free_result(res);
#endif
    return !lstLotterys.empty();
}

bool CDBWrapper::GetSubSchemesByLottery(__out__ map<int, SubScheme>& mapSubSchemes, _lpcstr lottery)
{
#ifdef MAKE_TEST_DATA
    for (map<int, SubScheme>::iterator it = m_mapSubSchemes.begin(); it != m_mapSubSchemes.end(); it++)
    {
        if (it->second.strLottery == lottery)
            mapSubSchemes[it->first] = it->second;
    }
#else
    string stmt = strtool::format("select id, name, lottery, play_kind, play_name, dwd_pos, formula, expected_accuracy, issues_per_round, \
                                  issues, rounds, win_rounds, comb_win_rounds, max_comb_win_rounds, comb_loss_rounds, max_comb_loss_rounds, \
                                  accuracy, merchant_id, scheme_id from service_subscheme where lottery = '%s';", lottery);
    MYSQL_RES* res = ExcuteQuery(stmt.c_str());
    if (res == NULL) return false;

    int num_fields = mysql_num_fields(res);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)))
    {
        SubScheme sc;
        sc.nID = strtool::to_int(row[0]);
        sc.strName = row[1]; // crypto::encoding_convert(row[1], "utf-8", "gb2312");
        sc.strLottery = row[2];
        sc.strPlayKind = row[3];
        sc.strPlayName = row[4];
        sc.nDWDPos = strtool::to_int(row[5]);
        sc.strFormula = row[6];
        sc.dExpectedAccuracy = strtool::to_float(row[7]);
        sc.nIssuesPerRound = strtool::to_int(row[8]);
        sc.nIssues = strtool::to_int(row[9]);
        sc.nRounds = strtool::to_int(row[10]);
        sc.nWinRounds = strtool::to_int(row[11]);
        sc.nCombWinRounds = strtool::to_int(row[12]);
        sc.nMaxCombWinRounds = strtool::to_int(row[13]);
        sc.nCombLossRounds = strtool::to_int(row[14]);
        sc.nMaxCombLossRounds = strtool::to_int(row[15]);;
        sc.dAccuracy = strtool::to_float(row[16]);
        sc.nMerchantID = strtool::to_int(row[17]);
        sc.nSchemeID = strtool::to_int(row[18]);
        mapSubSchemes[sc.nID] = sc;
    }
    mysql_free_result(res);
#endif
    return !mapSubSchemes.empty();
}

bool CDBWrapper::GetSchemeDetailsBySubSchemeID(__out__ map<int, SchemeDetail>& mapSchemeDetails, int subscheme_id, _lpcstr from_issue)
{
#ifdef MAKE_TEST_DATA
    for (map<int, SchemeDetail>::iterator it = m_mapSchemeDetails.begin(); it != m_mapSchemeDetails.end(); it++)
    {
        const SchemeDetail& sd = it->second;
        if (sd.nSubSchemeID == subscheme_id && sd.strIssue >= from_issue)
            mapSchemeDetails[it->first] = it->second;
    }
#else
    string stmt = strtool::format("select id, lottery, issue, play_kind, play_name, dwd_pos, round_index, round_total, code, open_code, win, \
                                  merchant_id, scheme_id, sub_scheme_id from service_schemedetail where sub_scheme_id = %d and issue >= '%s'",
                                  subscheme_id, from_issue);
    MYSQL_RES* res = ExcuteQuery(stmt.c_str());
    if (res == NULL) return false;

    int num_fields = mysql_num_fields(res);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)))
    {
        SchemeDetail detail;
        detail.nID = strtool::to_int(row[0]);
        detail.strLottery = row[1];
        detail.strIssue = row[2];
        detail.strPlayKind = row[3];
        detail.strPlayName = row[4];
        detail.nDWDPos = strtool::to_int(row[5]);
        detail.nRoundIndex = strtool::to_int(row[6]);
        detail.nRoundTotal = strtool::to_int(row[7]);
        detail.strCode = row[8];
        detail.strOpenCode = row[9];
        detail.bWin = strtool::to_int(row[10]);
        detail.nMerchantID = strtool::to_int(row[11]);
        detail.nSchemeID = strtool::to_int(row[12]);
        detail.nSubSchemeID = strtool::to_int(row[13]);
        mapSchemeDetails[detail.nID] = detail;
    }
    mysql_free_result(res);
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
    bool ret = true;
    string stmt;
    for (map<int, SchemeDetail>::const_iterator it = mapSchemeDetails.begin(); it != mapSchemeDetails.end(); it++)
    {
        const SchemeDetail& rfDetail = it->second;
        stmt += strtool::format("insert into service_schemedetail ( \
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
    }
    return ExCuteNoQuery(stmt.c_str());
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
    string stmt = strtool::format("update service_schemedetail set \
                                  open_code = '%s', \
                                  win = %d \
                                  where id = %d",
                                  schemeDetail.strOpenCode.c_str(),
                                  schemeDetail.bWin,
                                  schemeDetail.nID);
    return ExCuteNoQuery(stmt.c_str());
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
    string stmt = strtool::format("update service_subscheme set \
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
    return ExCuteNoQuery(stmt.c_str());
#endif
}
