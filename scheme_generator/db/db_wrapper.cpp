#include "stdafx.h"
#include "db_wrapper.h"

//////////////////////////////////////////////////////////////////////////
CDBWrapper::CDBWrapper()
{
}

CDBWrapper::CDBWrapper(_lpcstr host, _lpcstr user, _lpcstr passowrd) : CDB(host, user, passowrd)
{
}

CDBWrapper::~CDBWrapper()
{
}

bool CDBWrapper::GetSchemeLotterys(__out lstring_list& lstLotterys)
{
    sql::ResultSet* res = ExcuteQuery("SELECT DISTINCT lottery FROM service_scheme WHERE state = 1");
    while (res != NULL && res->next())
        lstLotterys.push_back(res->getString("lottery"));
    delete res;
    return !lstLotterys.empty();
}

// lslib::lstring CDBWrapper::GetSchemeDetailLastIssue(int subscheme_id)
// {
//     lstring issue;
//     lstring stmt;
//     stmt.format("select issue from service_schemedetail where sub_scheme_id = %d order by update_time desc limit 1", subscheme_id);
//     sql::ResultSet* res = ExcuteQuery(stmt);
//     if (res != NULL && res->next())
//         issue = res->getString("issue");
//     delete res;
//     return issue;
// }
//
// bool CDBWrapper::GetAllSchemes(__out__ map<int, Scheme>& mapSchemes)
// {
//     sql::ResultSet* res = ExcuteQuery("SELECT * FROM service_scheme");
//     while (res != NULL && res->next())
//     {
//         Scheme sc;
//         sc.nID = res->getInt("id");
//         sc.strName = crypto::encoding_convert(res->getString("name").c_str(), "utf-8", "gb2312");
//         sc.strLottery = crypto::encoding_convert(res->getString("lottery").c_str(), "utf-8", "gb2312");
//         sc.nSubSchemes = res->getInt("sub_schemes");
//         sc.dMaxAccuracy = res->getDouble("max_accuracy");
//         sc.nFavorites = res->getInt("favorites");
//         sc.nMerchantID = res->getInt("merchant_id");
//         mapSchemes[sc.nID] = sc;
//     }
//     delete res;
//     return !mapSchemes.empty();
// }
//
// bool CDBWrapper::GetAllSubSchemes(__out__ map<int, SubScheme> mapSubSchemes)
// {
//     sql::ResultSet* res = ExcuteQuery("SELECT * FROM service_subscheme");
//     while (res != NULL && res->next())
//     {
//         SubScheme sc;
//         sc.nID = res->getInt("id");
//         sc.strName = crypto::encoding_convert(res->getString("name").c_str(), "utf-8", "gb2312");
//         sc.strPlayKind = crypto::encoding_convert(res->getString("play_kind").c_str(), "utf-8", "gb2312");
//         sc.strPlayName = crypto::encoding_convert(res->getString("play_name").c_str(), "utf-8", "gb2312");
//         sc.strFormula = crypto::encoding_convert(res->getString("formula").c_str(), "utf-8", "gb2312");
//         sc.nIssues = res->getInt("issues");
//         sc.nRounds = res->getInt("rounds");
//         sc.dAccuracy = res->getDouble("accuracy");
//         sc.nMerchantID = res->getInt("merchant_id");
//         sc.nSchemeID = res->getInt("scheme_id");
//         mapSubSchemes[sc.nID] = sc;
//     }
//     delete res;
//     return !mapSubSchemes.empty();
// }
//
// bool CDBWrapper::AddSchemeDetail(const SchemeDetail& detail)
// {
//     lstring stmt;
//     stmt.format("INSERT INTO service_schemedetail(lottery, issue, play_kind, play_name, round_index, round_total, code, open_code, win, accuracy, merchant_id, scheme_id, sub_scheme_id)"
//         "VALUES(%d, '%s', '%s', '%s', %d, %d, '%s', '%s', %d, %0.2f, %d, %d, %d)",
//         detail.nID,
//         detail.strIssue.c_str(),
//         detail.strPlayKind.c_str(),
//         detail.strPlayName.c_str(),
//         detail.nRoundIndex,
//         detail.nRoundTotal,
//         detail.strCode.c_str(),
//         detail.strOpenCode.c_str(),
//         detail.bWin,
//         detail.dAccuracy,
//         detail.nMerchantID,
//         detail.nSchemeID,
//         detail.nSubSchemeID);
//     return Excute(stmt);
// }

bool CDBWrapper::GetSubSchemesByLottery(__out map<int, SubScheme>& mapSubSchemes, _lpcstr lottery)
{
    sql::ResultSet* res = ExcuteQuery("select * from service_subscheme where scheme_id in (select id from service_scheme where lottery = 17);");
    while (res != NULL && res->next())
    {
        SubScheme sc;
        sc.nID = res->getInt("id");
        sc.strName = res->getString("name").c_str();
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
    return !mapSubSchemes.empty();
}

bool CDBWrapper::GetSchemeDetailsBySubSchemeID(__out map<int, SchemeDetail>& mapSchemeDetails, int subscheme_id, _lpcstr from_issue)
{
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
        detail.dAccuracy = res->getInt("accuracy");
        detail.nMerchantID = res->getInt("merchant_id");
        detail.nSchemeID = res->getInt("scheme_id");
        detail.nSubSchemeID = res->getInt("sub_scheme_id");
        mapSchemeDetails[detail.nID] = detail;
    }
    delete res;
    return !mapSchemeDetails.empty();
}

bool CDBWrapper::AddSchemeDetails(const map<int, SchemeDetail>& mapSchemeDetails)
{
    lstring stmt;
    stmt = "begin;";
    for (map<int, SchemeDetail>::const_iterator it = mapSchemeDetails.begin(); it != mapSchemeDetails.end(); it++)
    {
        const SchemeDetail& rfDetail = it->second;
        stmt.append_format("insert into service_schemedetail ( \
                           lottery, issue, play_kind, play_name, round_index, round_total, code, open_code, win, accuracy, merchant_id, scheme_id, sub_scheme_id \
                           ) values ( '%s', '%s', '%s', '%s', %d, %d, '%s', '%s', %d, %0.2f, %d, %d, %d );",
                           rfDetail.strLottery.c_str(),
                           rfDetail.strIssue.c_str(),
                           rfDetail.strPlayKind.c_str(),
                           rfDetail.strPlayName.c_str(),
                           rfDetail.nRoundIndex,
                           rfDetail.nRoundTotal,
                           rfDetail.strCode.c_str(),
                           rfDetail.strOpenCode.c_str(),
                           rfDetail.bWin,
                           rfDetail.dAccuracy,
                           rfDetail.nMerchantID,
                           rfDetail.nSchemeID,
                           rfDetail.nSubSchemeID);
    }
    stmt.append("commit;");
    return Excute(stmt);
}

bool CDBWrapper::UpdateSchemeDetail(const SchemeDetail& schemeDetail)
{
    lstring stmt;
    stmt.format("update service_schemedetail set \
                lottery = '%s',\
                issue = '%s', \
                play_kind = '%s', \
                play_name = '%s', \
                round_index = %d, \
                round_total = %d, \
                code = '%s', \
                open_code = '%s', \
                win = %d, \
                accuracy = %0.2f, \
                merchant_id = %d, \
                scheme_id = %d, \
                sub_scheme_id = %d \
                where id = %d",
                schemeDetail.strLottery.c_str(),
                schemeDetail.strIssue.c_str(),
                schemeDetail.strPlayKind.c_str(),
                schemeDetail.strPlayName.c_str(),
                schemeDetail.nRoundIndex,
                schemeDetail.nRoundTotal,
                schemeDetail.strCode.c_str(),
                schemeDetail.strOpenCode.c_str(),
                schemeDetail.bWin,
                schemeDetail.dAccuracy,
                schemeDetail.nMerchantID,
                schemeDetail.nSchemeID,
                schemeDetail.nSubSchemeID,
                schemeDetail.nID);
    return Excute(stmt);
}

bool CDBWrapper::UpdateSubScheme(const SubScheme& subScheme)
{
    lstring stmt;

    return Excute(stmt);
}
