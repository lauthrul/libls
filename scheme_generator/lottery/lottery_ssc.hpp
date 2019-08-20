#pragma once
#include "lottery.hpp"

namespace lottery
{
    namespace ssc
    {
        //////////////////////////////////////////////////////////////////////////
#define find_str(str, patten)           (str.find(patten) != string::npos)
#define start_with_str(str, patten)     (str.find(patten) == 0)

#define is_zx(strPlayName)              (find_str(strPlayName, "_zx_"))
#define is_zux(strPlayName)             (find_str(strPlayName, "_zux_"))
#define is_ds(strPlayName)              (find_str(strPlayName, "ds"))
#define is_fs(strPlayName)              (find_str(strPlayName, "fs"))
#define is_zh(strPlayName)              (find_str(strPlayName, "zh"))
#define is_hz(strPlayName)              (find_str(strPlayName, "hz"))

#define is_wx(strPlayName)              (start_with_str(strPlayName, "wx_")) // 五星

#define is_qsx(strPlayName)             (start_with_str(strPlayName, "qsx_")) // 前四
#define is_hsx(strPlayName)             (start_with_str(strPlayName, "sx_")) // 后四
#define is_sx(strPlayName)              (is_hsx(strPlayName) || is_qsx(strPlayName)) //四星

#define is_qsm(strPlayName)             (start_with_str(strPlayName, "qsm_")) // 前三
#define is_zsm(strPlayName)             (start_with_str(strPlayName, "zsm_")) // 中三
#define is_hsm(strPlayName)             (start_with_str(strPlayName, "hsm_")) // 后三
#define is_sm(strPlayName)              (is_qsm(strPlayName) || is_zsm(strPlayName) || is_hsm(strPlayName)) // 三星

#define is_em(strPlayName)              (start_with_str(strPlayName, "em_")) // 二星
#define is_qem(strPlayName)             (is_em(strPlayName) && find_str(strPlayName, "_q")) // 前二
#define is_hem(strPlayName)             (is_em(strPlayName) && find_str(strPlayName, "_h")) // 后二

#define is_dwd(strPlayName)             (start_with_str(strPlayName, "dwd_")) // 定位胆

#define is_bdd(strPlayName)             (start_with_str(strPlayName, "bdd_")) // 不定胆

#define formula_random                  "fx = random()"

        //////////////////////////////////////////////////////////////////////////
        string ConvertDStoFS(const string& playname, int len, const string& ds)
        {
            typedef set<char> pos_t;

            pos_t pos[5];
            string_array arr;
            strtool::split(arr, ds, " ", false);
            for (size_t i = 0; i < arr.size(); i++)
            {
                for (size_t j = 0; j < MIN(arr[i].length(), 5); j++)
                {
                    const string& num = arr[i];
                    pos[j].insert(num[j]);
                }
            }

            string strret;
            for (size_t j = 0; j < len; j++)
                strret += string().assign(pos[j].begin(), pos[j].end()) + "-";
            return strtool::trim_right(strret, '-');
        }

        // 判奖-直选复式
        bool check_win_zx_fs(const string& code, const string& openCode, int range[2])
        {
            // code: 123456-123456-123456-123456-123456
            // openCode: 3,4,6,8,1
            string_array arr;        strtool::split(arr, code, "-");
            string compactOpenCode = strtool::replace(openCode, ",", "");
            compactOpenCode = compactOpenCode.substr(range[0], range[1] - range[0] + 1);
            for (size_t j = 0; j < arr.size(); j++)
            {
                if (arr[j].find(compactOpenCode[j]) == string::npos)
                    return false;
            }
            return true;
        }

        // 判奖-直选单式
        bool check_win_zx_ds(const string& code, const string& openCode, int range[2])
        {
            // code: 12345 12456 34215 65432 98507 ...
            // openCode: 3,4,6,8,1
            string_array arr;        strtool::split(arr, code, " ");
            string compactOpenCode = strtool::replace(openCode, ",", "");
            compactOpenCode = compactOpenCode.substr(range[0], range[1] - range[0] + 1);
            for (size_t j = 0; j < arr.size(); j++)
            {
                if (arr[j] == compactOpenCode)
                    return true;
            }
            return false;
        }

        // 判奖-直选组合
        bool check_win_zx_zh(const string& code, const string& openCode, int range[2])
        {
            // code: 123456-123456-123456-123456-123456
            // openCode: 3,4,6,8,1
            string_array arr;        strtool::split(arr, code, "-");
            string compactOpenCode = strtool::replace(openCode, ",", "");
            compactOpenCode = compactOpenCode.substr(range[0], range[1] - range[0] + 1);
            if ((*arr.rbegin()).find(*compactOpenCode.rbegin()) != string::npos)
                return true;
            return false;
        }

        // 判奖-直选和值
        bool check_win_zx_hz(const string& code, const string& openCode, int range[2])
        {
            // code: 0 1 2 ... 27
            // openCode: 3,4,6,8,1
            string_array arr;        strtool::split(arr, code, " ");
            string compactOpenCode = strtool::replace(openCode, ",", "");
            compactOpenCode = compactOpenCode.substr(range[0], range[1] - range[0] + 1);
            int sum = 0;
            for (size_t i = 0; i < compactOpenCode.length(); i++)
                sum += compactOpenCode[i] - '0';
            if (find(arr.begin(), arr.end(), strtool::from_int(sum)) != arr.end())
                return true;
            return false;
        }

        // 判奖-不定胆
        bool check_win_bdd(const string& code, const string& openCode, int range[2], int len)
        {
            // code: 0 1 2 ... 9
            // openCode: 3,4,6,8,1
            string_array arr;        strtool::split(arr, code, " ");
            string compactOpenCode = strtool::replace(openCode, ",", "");
            compactOpenCode = compactOpenCode.substr(range[0], range[1] - range[0] + 1);
            set<string> setCode;
            for (size_t i = 0; i < compactOpenCode.length(); i++)
            {
                string singleCode = string(1, compactOpenCode[i]);
                if (find(arr.begin(), arr.end(), singleCode) != arr.end())
                    setCode.insert(singleCode);
            }
            return (setCode.size() >= len);
        }

        // 判奖
        bool CheckWin(const SchemeDetail& detail)
        {
            // 五星
            if (is_wx(detail.strPlayName))
            {
                int nCodeRange[2] = {0, 4};
                if (is_zx(detail.strPlayName)) // 五星直选单式、复式、组合
                {
                    if (is_fs(detail.strPlayName))          return check_win_zx_fs(detail.strCode, detail.strOpenCode, nCodeRange);
                    else if (is_ds(detail.strPlayName))     return check_win_zx_ds(detail.strCode, detail.strOpenCode, nCodeRange);
                    else if (is_zh(detail.strPlayName))     return check_win_zx_zh(detail.strCode, detail.strOpenCode, nCodeRange);
                }
            }

            // 四星
            else if (is_sx(detail.strPlayName))
            {
                int nCodeRange[2] = { 0 };
                if (is_qsx(detail.strPlayName))
                {
                    // 前四
                    nCodeRange[0] = 0;
                    nCodeRange[1] = 3;
                }
                else if (is_hsx(detail.strPlayName))
                {
                    // 后四
                    nCodeRange[0] = 1;
                    nCodeRange[1] = 4;
                }

                if (is_zx(detail.strPlayName)) // 四星直选单式、复式、组合
                {
                    if (is_fs(detail.strPlayName))          return check_win_zx_fs(detail.strCode, detail.strOpenCode, nCodeRange);
                    else if (is_ds(detail.strPlayName))     return check_win_zx_ds(detail.strCode, detail.strOpenCode, nCodeRange);
                    else if (is_zh(detail.strPlayName))     return check_win_zx_zh(detail.strCode, detail.strOpenCode, nCodeRange);
                }
            }

            // 三星
            else if (is_sm(detail.strPlayName))
            {
                int nCodeRange[2] = { 0 };
                if (is_qsm(detail.strPlayName))
                {
                    // 前三
                    nCodeRange[0] = 0;
                    nCodeRange[1] = 2;
                }
                else if (is_zsm(detail.strPlayName))
                {
                    // 中三
                    nCodeRange[0] = 1;
                    nCodeRange[1] = 3;
                }
                else if (is_hsm(detail.strPlayName))
                {
                    // 后三
                    nCodeRange[0] = 2;
                    nCodeRange[1] = 4;
                }

                if (is_zx(detail.strPlayName)) // 三星直选单式、复式、和值
                {
                    if (is_fs(detail.strPlayName))          return check_win_zx_fs(detail.strCode, detail.strOpenCode, nCodeRange);
                    else if (is_ds(detail.strPlayName))     return check_win_zx_ds(detail.strCode, detail.strOpenCode, nCodeRange);
                    else if (is_hz(detail.strPlayName))     return check_win_zx_hz(detail.strCode, detail.strOpenCode, nCodeRange);
                }
            }

            // 二星
            else if (is_em(detail.strPlayName))
            {
                int nCodeRange[2] = { 0 };
                if (is_qem(detail.strPlayName))
                {
                    // 前二
                    nCodeRange[0] = 0;
                    nCodeRange[1] = 1;
                }
                else if (is_hem(detail.strPlayName))
                {
                    // 后二
                    nCodeRange[0] = 3;
                    nCodeRange[1] = 4;
                }

                if (is_zx(detail.strPlayName)) // 二星直选单式、复式、和值
                {
                    if (is_fs(detail.strPlayName))          return check_win_zx_fs(detail.strCode, detail.strOpenCode, nCodeRange);
                    else if (is_ds(detail.strPlayName))     return check_win_zx_ds(detail.strCode, detail.strOpenCode, nCodeRange);
                    else if (is_hz(detail.strPlayName))     return check_win_zx_hz(detail.strCode, detail.strOpenCode, nCodeRange);
                }
            }

            // 一星(定位胆)
            else if (is_dwd(detail.strPlayName))
            {
                int nCodeRange[2] = {detail.nDWDPos, detail.nDWDPos};
                return check_win_zx_ds(detail.strCode, detail.strOpenCode, nCodeRange);
            }

            // 不定胆
            else if (is_bdd(detail.strPlayName))
            {
                int len = 0;
                int nCodeRange[2] = { 0 };

                if (find_str(detail.strPlayName, "_qs1")) // 前三一码
                {
                    len = 1;
                    nCodeRange[0] = 0;
                    nCodeRange[1] = 2;
                }
                else if (find_str(detail.strPlayName, "_qs2")) // 前三二码
                {
                    len = 2;
                    nCodeRange[0] = 0;
                    nCodeRange[1] = 2;
                }
                else if (find_str(detail.strPlayName, "_z31")) // 中三一码
                {
                    len = 1;
                    nCodeRange[0] = 1;
                    nCodeRange[1] = 3;
                }
                else if (find_str(detail.strPlayName, "_z32")) // 中三二码
                {
                    len = 2;
                    nCodeRange[0] = 1;
                    nCodeRange[1] = 3;
                }
                else if (find_str(detail.strPlayName, "_hs1")) // 后三一码
                {
                    len = 1;
                    nCodeRange[0] = 2;
                    nCodeRange[1] = 4;
                }
                else if (find_str(detail.strPlayName, "_hs2")) // 后三二码
                {
                    len = 2;
                    nCodeRange[0] = 2;
                    nCodeRange[1] = 4;
                }
                else if (find_str(detail.strPlayName, "_q4x1")) // 前四一码
                {
                    len = 1;
                    nCodeRange[0] = 0;
                    nCodeRange[1] = 3;
                }
                else if (find_str(detail.strPlayName, "_q4x2")) // 前四二码
                {
                    len = 2;
                    nCodeRange[0] = 0;
                    nCodeRange[1] = 3;
                }
                else if (find_str(detail.strPlayName, "_4x1")) // 后四一码
                {
                    len = 1;
                    nCodeRange[0] = 1;
                    nCodeRange[1] = 4;
                }
                else if (find_str(detail.strPlayName, "_4x2")) // 后四二码
                {
                    len = 2;
                    nCodeRange[0] = 1;
                    nCodeRange[1] = 4;
                }
                else if (find_str(detail.strPlayName, "_5x2")) // 五星二码
                {
                    len = 2;
                    nCodeRange[0] = 0;
                    nCodeRange[1] = 4;
                }
                else if (find_str(detail.strPlayName, "_5x3")) // 五星三码
                {
                    len = 3;
                    nCodeRange[0] = 0;
                    nCodeRange[1] = 4;
                }
                return check_win_bdd(detail.strCode, detail.strOpenCode, nCodeRange, len);
            }

            return false;
        }

        // 做号-直选单式
        string gen_zx_ds_random(int nLen, double expectedAccuracy)
        {
            // code: 12345 12456 34215 65432 98507 ...
            int nTotalCounts = pow((double)10, nLen);
            int nCounts = expectedAccuracy * nTotalCounts / 100;
            int nRange[2] = {0, nTotalCounts - 1};
            string strFormat = strtool::format("%%0%dd", nLen);

            vector<int> vct;
            GenNumRandom(vct, nCounts, nLen, nRange, true, NULL);
            sort(vct.begin(), vct.end());
            return FormatNumbers(vct, strFormat, " ");
        }

        // 做号-直选复式
        string gen_zx_fs_random(int nLen, double expectedAccuracy)
        {
            // code: 123456-123456-123456-123456-123456
            int nTotalCounts = 10;
            int nCounts = expectedAccuracy * nTotalCounts / 100;
            int nRange[2] = {0, nTotalCounts - 1};

            string strret;
            for (int i = 0; i < nLen; i++)
            {
                vector<int> vct;
                GenNumRandom(vct, nCounts, nLen, nRange, true, NULL);
                sort(vct.begin(), vct.end());
                strret += FormatNumbers(vct, "%d", "") + "-";
            }
            return strtool::trim_right(strret, '-');
        }

        // 做号-直选组合
        string gen_zx_zh_random(int nLen, double expectedAccuracy)
        {
            // code: 123456-123456-123456-123456-123456
            return gen_zx_fs_random(nLen, expectedAccuracy);
        }

        // 做号-直选和值
        string gen_zx_hz_random(int nLen, double expectedAccuracy)
        {
            // code: 0 1 2 ... 27
            int nTotalCounts = 9 * nLen + 1;
            int nCounts = expectedAccuracy * nTotalCounts / 100;
            int nRange[2] = { 0, nTotalCounts - 1 };

            vector<int> vct;
            GenNumRandom(vct, nCounts, 2, nRange, true, NULL);
            sort(vct.begin(), vct.end());
            return FormatNumbers(vct, "%d", " ");
        }

        // 做号-不定胆
        string gen_bdd_random(int nLen, double expectedAccuracy)
        {
            // code: 0 1 2 ... 9
            int nTotalCounts = 10;
            int nCounts = expectedAccuracy * nTotalCounts / 100;
            nCounts = max(nCounts, nLen);
            int nRange[2] = { 0, nTotalCounts - 1 };

            vector<int> vct;
            GenNumRandom(vct, nCounts, 1, nRange, true, NULL);
            sort(vct.begin(), vct.end());
            return FormatNumbers(vct, "%d", " ");
        }

        // 做号
        string GenereateCode(const string& playName, const string& formula, double expectedAccuracy)
        {
            int nLen = 0;

            // 五星
            if (is_wx(playName))
            {
                nLen = 5;
                if (is_zx(playName)) // 五星直选单式、复式、组合
                {
                    if (formula == formula_random)
                    {
                        if (is_ds(playName))            return gen_zx_ds_random(nLen, expectedAccuracy);
                        else if (is_fs(playName))       return gen_zx_fs_random(nLen, expectedAccuracy);
                        else if (is_zh(playName))       return gen_zx_zh_random(nLen, expectedAccuracy);
                    }
                }
            }

            // 四星
            else if (is_sx(playName))
            {
                nLen = 4;
                if (is_zx(playName)) // 四星直选单式、复式、组合
                {
                    if (formula == formula_random)
                    {
                        if (is_ds(playName))            return gen_zx_ds_random(nLen, expectedAccuracy);
                        else if (is_fs(playName))       return gen_zx_fs_random(nLen, expectedAccuracy);
                        else if (is_zh(playName))       return gen_zx_zh_random(nLen, expectedAccuracy);
                    }
                }
            }

            // 三星
            else if (is_sm(playName))
            {
                nLen = 3;
                if (is_zx(playName)) // 三星直选单式、复式、和值
                {
                    if (formula == formula_random)
                    {
                        if (is_ds(playName))            return gen_zx_ds_random(nLen, expectedAccuracy);
                        else if (is_fs(playName))       return gen_zx_fs_random(nLen, expectedAccuracy);
                        else if (is_hz(playName))       return gen_zx_hz_random(nLen, expectedAccuracy);
                    }
                }
            }

            // 二星
            else if (is_em(playName))
            {
                nLen = 2;
                if (is_zx(playName)) // 二星直选单式、复式、和值
                {
                    if (formula == formula_random)
                    {
                        if (is_ds(playName))            return gen_zx_ds_random(nLen, expectedAccuracy);
                        else if (is_fs(playName))       return gen_zx_fs_random(nLen, expectedAccuracy);
                        else if (is_hz(playName))       return gen_zx_hz_random(nLen, expectedAccuracy);
                    }
                }
            }

            // 一星(定位胆)
            else if (is_dwd(playName))
            {
                nLen = 1;
                if (formula == formula_random)
                {
                    return gen_zx_ds_random(nLen, expectedAccuracy);
                }
            }

            // 不定胆
            else if (is_bdd(playName))
            {
                nLen = (*playName.rbegin()) - '0';
                if (formula == formula_random)
                {
                    return gen_bdd_random(nLen, expectedAccuracy);
                }
            }

            return "";
        }
    }
}