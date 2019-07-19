#pragma once

#include "db.h"

//////////////////////////////////////////////////////////////////////////
struct Scheme
{
    int nID;
    string strName;
    string strLottery;
    int nSubSchemes;
    double dMaxAccuracy;
    int nFavorites;
    int nMerchantID;

    Scheme()
    {
        nID = 0;
        nSubSchemes = 0;
        dMaxAccuracy = 0;
        nFavorites = 0;
        nMerchantID = 0;
    }
};

struct SubScheme
{
    int nID;
    string strName;
    string strLottery;
    string strPlayKind;
    string strPlayName;
    int nDWDPos;
    string strFormula;
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
        nID = 0;
        nDWDPos = 0;
        dExpectedAccuracy = 0;
        nIssuesPerRound = 0;
        nIssues = 0;
        nRounds = 0;
        nWinRounds = 0;
        nCombWinRounds = 0;
        nMaxCombWinRounds = 0;
        nCombLossRounds = 0;
        nMaxCombLossRounds = 0;
        dAccuracy = 0;
        nMerchantID = 0;
        nSchemeID = 0;
    }
};

struct SchemeDetail
{
    int nID;
    string strLottery;
    string strIssue;
    string strPlayKind;
    string strPlayName;
    int nDWDPos;
    int nRoundIndex;
    int nRoundTotal;
    string strCode;
    string strOpenCode;
    bool bWin;
    int nMerchantID;
    int nSchemeID;
    int nSubSchemeID;

    SchemeDetail()
    {
        nID = 0;
        nDWDPos = 0;
        nRoundIndex = 0;
        nRoundTotal = 0;
        bWin = false;
        nMerchantID = 0;
        nSchemeID = 0;
        nSubSchemeID = 0;
    }
};

//////////////////////////////////////////////////////////////////////////
class CDBWrapper : public CDB
{
public:
    CDBWrapper();
    CDBWrapper(_lpcstr host, _lpcstr user, _lpcstr passowrd, _lpcstr db);
    virtual ~CDBWrapper();

public:
    bool GetSchemeLotterys(__out__ string_list& lstLotterys);
    bool GetSubSchemesByLottery(__out__ map<int, SubScheme>& mapSubSchemes, _lpcstr lottery);
    bool GetSchemeDetailsBySubSchemeID(__out__ map<int, SchemeDetail>& mapSchemeDetails, int subscheme_id, _lpcstr from_issue);
    bool AddSchemeDetails(const map<int, SchemeDetail>& mapSchemeDetails);
    bool UpdateSchemeDetailResult(const SchemeDetail& schemeDetail);
    bool UpdateSubSchemeStatistic(const SubScheme& subScheme);

protected:
#ifdef MAKE_TEST_DATA
    void MakeTestData();

    map<int, Scheme>        m_mapSchemes;
    map<int, SubScheme>     m_mapSubSchemes;
    map<int, SchemeDetail>  m_mapSchemeDetails;
#endif
};
