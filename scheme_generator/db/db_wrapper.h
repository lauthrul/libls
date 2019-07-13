#pragma once

#include "db.h"

//////////////////////////////////////////////////////////////////////////
struct Scheme
{
    int nID;
    lstring strName;
    lstring strLottery;
    int nSubSchemes;
    double dMaxAccuracy;
    int nFavorites;
    int nMerchantID;
};

struct SubScheme
{
    int nID;
    lstring strName;
    lstring strLottery;
    lstring strPlayKind;
    lstring strPlayName;
    int nDWDPos;
    lstring strFormula;
    double dExpectedAccuracy;
    int nIssuesPerRound;
    int nIssues;                // for statistic. total issues
    int nRounds;                // for statistic. total rounds
    int nWinRounds;             // for statistic. win rounds
    int nCombWinRounds;         // for statistic. combo win rounds
    int nMaxCombWinRounds;      // for statistic. max combo win rounds
    int nCombLossRounds;        // for statistic. combo loss rounds
    int nMaxCombLossRounds;     // for statistic. max combo loss rounds
    double dAccuracy;           // for statistic. real accuracy
    int nMerchantID;
    int nSchemeID;

    SubScheme()
    {
        nIssues = 0;
        nRounds = 0;
        nWinRounds = 0;
        nCombWinRounds = 0;
        nMaxCombWinRounds = 0;
        nCombLossRounds = 0;
        nMaxCombLossRounds = 0;
        dAccuracy = 0;
    }
};

struct SchemeDetail
{
    int nID;
    lstring strLottery;
    lstring strIssue;
    lstring strPlayKind;
    lstring strPlayName;
    int nDWDPos;
    int nRoundIndex;
    int nRoundTotal;
    lstring strCode;
    lstring strOpenCode;
    bool bWin;
    double dAccuracy;
    int nMerchantID;
    int nSchemeID;
    int nSubSchemeID;
};

//////////////////////////////////////////////////////////////////////////
class CDBWrapper : public CDB
{
public:
    CDBWrapper();
    CDBWrapper(_lpcstr host, _lpcstr user, _lpcstr passowrd);
    ~CDBWrapper();

public:
    bool GetSchemeLotterys(__out lstring_list& lstLotterys);
//     lstring GetSchemeDetailLastIssue(int subscheme_id);
// 
//     bool GetAllSchemes(__out__ map<int, Scheme>& mapSchemes);
//     bool GetAllSubSchemes(__out__ map<int, SubScheme> mapSubSchemes);
//     bool GetAllSchemeDetails(__out__ map<int, SchemeDetail> mapSchemeDetails);
//     bool AddSchemeDetail(const SchemeDetail& detail);

    bool GetSubSchemesByLottery(__out map<int, SubScheme>& mapSubSchemes, _lpcstr lottery);
    bool GetSchemeDetailsBySubSchemeID(__out map<int, SchemeDetail>& mapSchemeDetails, int subscheme_id, _lpcstr from_issue);

    bool AddSchemeDetails(const map<int, SchemeDetail>& mapSchemeDetails);
    bool UpdateSchemeDetail(const SchemeDetail& schemeDetail);
    bool UpdateSubScheme(const SubScheme& subScheme);
};
