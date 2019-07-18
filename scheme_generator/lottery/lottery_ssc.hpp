#pragma once
#include "lottery.hpp"

namespace lottery
{
    namespace ssc
    {
        string ConvertDStoFS(const string& playname, int len, const string& ds)
        {
            typedef set<char> pos_t;

            pos_t pos[5];
            string_array arr;
            strtool::split(arr, ds.c_str(), " ", false);
            for (size_t i = 0; i < arr.size(); i++)
            {
                for (size_t j = 0; j < min(arr[i].length(), 5); j++)
                {
                    const string& num = arr[i];
                    pos[j].insert(num[j]);
                }
            }

            string strret;
            for (size_t j = 0; j < len; j++)
                strret += string().assign(pos[j].begin(), pos[j].end()) + "-";
            return strtool::trim_right(strret.c_str(), '-');
        }

        bool check_win_zx_fs(const string& code, const string& openCode, int range[2])
        {
            // code: 123456-123456-123456-123456-123456
            // openCode: 3,4,6,8,1
            string_array arr;        strtool::split(arr, code.c_str(), "-");
            string compactOpenCode = strtool::replace(openCode.c_str(), ",", "");
            compactOpenCode = compactOpenCode.substr(range[0], range[1] - range[0] + 1);
            for (size_t j = 0; j < arr.size(); j++)
            {
                if (arr[j].find(compactOpenCode[j]) == string::npos)
                    return false;
            }
            return true;
        }

        bool check_win_zx_ds(const string& code, const string& openCode, int range[2])
        {
            // code: 12345 12456 34215 65432 98507 ...
            // openCode: 3,4,6,8,1
            string_array arr;        strtool::split(arr, code.c_str(), " ");
            string compactOpenCode = strtool::replace(openCode.c_str(), ",", "");
            compactOpenCode = compactOpenCode.substr(range[0], range[1] - range[0] + 1);
            for (size_t j = 0; j < arr.size(); j++)
            {
                if (arr[j] == compactOpenCode)
                    return true;
            }
            return false;
        }

        bool CheckWin(const SchemeDetail& detail)
        {
            // 五星
            // 五星直选单式、复式
            if (detail.strPlayName == "wx_zx_fs" || detail.strPlayName == "wx_zx_ds")
            {
                int nCodeRange[2] = {0, 4};
                if (detail.strPlayName.find("fs") != string::npos)      return check_win_zx_fs(detail.strCode, detail.strOpenCode, nCodeRange);
                else                                                    return check_win_zx_ds(detail.strCode, detail.strOpenCode, nCodeRange);
            }

            // 四星
            // 四星直选单式、复式
            else if (detail.strPlayName.find("sx_zx_fs") != string::npos || detail.strPlayName.find("sx_zx_ds") != string::npos)
            {
                int nCodeRange[2];
                if (detail.strPlayName.find("qsx") == 0)
                {
                    nCodeRange[0] = 0;
                    nCodeRange[1] = 3;
                }
                else
                {
                    nCodeRange[0] = 1;
                    nCodeRange[1] = 4;
                }
                if (detail.strPlayName.find("fs") != string::npos)      return check_win_zx_fs(detail.strCode, detail.strOpenCode, nCodeRange);
                else                                                    return check_win_zx_ds(detail.strCode, detail.strOpenCode, nCodeRange);
            }

            // 三星
            // 三星直选单式、复式
            else if (detail.strPlayName.find("sm_zx_fs") != string::npos || detail.strPlayName.find("sm_zx_ds") != string::npos)
            {
                int nCodeRange[2];
                if (detail.strPlayName.find("qsm") == 0)
                {
                    nCodeRange[0] = 0;
                    nCodeRange[1] = 2;
                }
                else if (detail.strPlayName.find("zsm") == 0)
                {
                    nCodeRange[0] = 1;
                    nCodeRange[1] = 3;
                }
                else
                {
                    nCodeRange[0] = 2;
                    nCodeRange[1] = 4;
                }
                if (detail.strPlayName.find("fs") != string::npos)      return check_win_zx_fs(detail.strCode, detail.strOpenCode, nCodeRange);
                else                                                    return check_win_zx_ds(detail.strCode, detail.strOpenCode, nCodeRange);
            }

            // 二星
            // 二星直选单式、复式
            else if (detail.strPlayName == "em_zx_qfs" || detail.strPlayName == "em_zx_hfs" || detail.strPlayName == "em_zx_qds" || detail.strPlayName == "em_zx_hds")
            {
                int nCodeRange[2];
                if (detail.strPlayName.find("em_zx_q") == 0)
                {
                    nCodeRange[0] = 0;
                    nCodeRange[1] = 1;
                }
                else
                {
                    nCodeRange[0] = 3;
                    nCodeRange[1] = 4;
                }
                if (detail.strPlayName.find("fs") != string::npos)      return check_win_zx_fs(detail.strCode, detail.strOpenCode, nCodeRange);
                else                                                    return check_win_zx_ds(detail.strCode, detail.strOpenCode, nCodeRange);
            }

            // 一星
            // 定位胆
            else if (detail.strPlayName == "dwd_dwd_dwd")
            {
                int nCodeRange[2] = {detail.nDWDPos, detail.nDWDPos};
                return check_win_zx_ds(detail.strCode, detail.strOpenCode, nCodeRange);
            }

            return false;
        }

        string gen_zx_ds_random(int nLen, double expectedAccuracy)
        {
            int nTotalCounts = pow((double)10, nLen);
            int nCounts = expectedAccuracy * nTotalCounts / 100;
            int nRange[2] = {0, nTotalCounts - 1};
            string strFormat = strtool::format("%%0%dd", nLen);

            vector<int> vct;
            GenNumRandom(vct, nCounts, nLen, nRange, true, NULL);
            sort(vct.begin(), vct.end());
            return FormatNumbers(vct, strFormat, " ");
        }

        string gen_zx_fs_random(int nLen, double expectedAccuracy)
        {
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
            return strtool::trim_right(strret.c_str(), '-');
        }

        string GenereateCode(const string& playName, const string& formula, double expectedAccuracy)
        {
            int nCounts = 0;
            int nTotalCounts = 0;
            int nLen = 0;
            string strFormat;

            // 五星
            if (playName == "wx_zx_ds" && playName == "wx_zx_fs") // 五星直选单式、复式
            {
                if (formula == "fx = random()")
                {
                    if (playName.find("ds") != string::npos)           return gen_zx_ds_random(5, expectedAccuracy);
                    else                                                return gen_zx_fs_random(5, expectedAccuracy);
                }
            }

            // 四星
            else if (playName.find("sx_zx_ds") != string::npos || playName.find("sx_zx_fs") != string::npos) // 四星直选单式、复式
            {
                if (formula == "fx = random()")
                {
                    if (playName.find("ds") != string::npos)           return gen_zx_ds_random(4, expectedAccuracy);
                    else                                                return gen_zx_fs_random(4, expectedAccuracy);
                }
            }

            // 三星
            else if (playName.find("sm_zx_ds") != string::npos || playName.find("sm_zx_fs") != string::npos) // 三星直选单式、复式
            {
                if (formula == "fx = random()")
                {
                    if (playName.find("ds") != string::npos)           return gen_zx_ds_random(3, expectedAccuracy);
                    else                                                return gen_zx_fs_random(3, expectedAccuracy);
                }
            }

            // 二星
            else if (playName == "em_zx_qfs" || playName == "em_zx_qds" || playName == "em_zx_hfs" || playName == "em_zx_hds") // 二星直选单式、复式
            {
                if (formula == "fx = random()")
                {
                    if (playName.find("ds") != string::npos)           return gen_zx_ds_random(2, expectedAccuracy);
                    else                                                return gen_zx_fs_random(2, expectedAccuracy);
                }
            }

            // 一星
            else if (playName == "dwd_dwd_dwd") // 定位胆
            {
                if (formula == "fx = random()")
                {
                    return gen_zx_ds_random(1, expectedAccuracy);
                }
            }

            // 不定胆


            return "";
        }
    }
}