#pragma once
#include "lottery.hpp"

namespace lottery
{
    namespace ssc
    {
        lstring ConvertDStoFS(const lstring& playname, int len, const lstring& ds)
        {
            typedef set<char> pos_t;

            pos_t pos[5];
            lstring_array arr;
            ds.split(arr, " ", false);
            for (size_t i = 0; i < arr.size(); i++)
            {
                for (size_t j = 0; j < min(arr[i].length(), 5); j++)
                {
                    const lstring& num = arr[i];
                    pos[j].insert(num[j]);
                }
            }

            lstring strret;
            for (size_t j = 0; j < len; j++)
                strret += lstring().assign(pos[j].begin(), pos[j].end()) + "-";
            return strret.trim_right('-');
        }

        bool check_win_zx_fs(const lstring& code, const lstring& openCode, int range[2])
        {
            lstring_array arr;          code.split(arr, "-");
            lstring_array arr_open;     openCode.split(arr_open, ",");
            for (size_t j = range[0]; j <= min(arr.size() - 1, range[1]); j++)
            {
                if (arr[j].find(arr_open[j]) == string::npos)
                    return false;
            }
            return true;
        }

        bool check_win_zx_ds(const lstring& code, const lstring& openCode, int range[2])
        {
            for (size_t j = range[0]; j <= min(openCode.length() - 1, range[1]); j++)
            {
                if (code[j] != openCode[j])
                    return false;
            }
            return true;
        }

        bool CheckWin(const SchemeDetail& detail)
        {
            // ����
            // ����ֱѡ��ʽ����ʽ
            if (detail.strPlayName == "wx_zx_fs" || detail.strPlayName == "wx_zx_ds")
            {
                int nCodeRange[2] = {0, 4};
                if (detail.strPlayName.find("fs") != string::npos)      return check_win_zx_fs(detail.strCode, detail.strOpenCode, nCodeRange);
                else                                                    return check_win_zx_ds(detail.strCode, detail.strOpenCode, nCodeRange);
            }

            // ����
            // ����ֱѡ��ʽ����ʽ
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

            // ����
            // ����ֱѡ��ʽ����ʽ
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

            // ����
            // ����ֱѡ��ʽ����ʽ
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

            // һ��
            // ��λ��
            else if (detail.strPlayName == "dwd_dwd_dwd")
            {
                int nCodeRange[2] = {detail.nDWDPos, detail.nDWDPos};
                return check_win_zx_ds(detail.strCode, detail.strOpenCode, nCodeRange);
            }

            return false;
        }

        lstring gen_zx_ds_random(int nLen, double expectedAccuracy)
        {
            int nTotalCounts = pow((double)10, nLen);
            int nCounts = expectedAccuracy * nTotalCounts / 100;
            int nRange[2] = {0, nTotalCounts - 1};
            lstring strFormat; strFormat.format("%%0%dd", nLen);

            vector<int> vct;
            GenNumRandom(vct, nCounts, nLen, nRange, true, NULL);
            sort(vct.begin(), vct.end());
            return FormatNumbers(vct, strFormat, " ");
        }

        lstring gen_zx_fs_random(int nLen, double expectedAccuracy)
        {
            int nTotalCounts = 10;
            int nCounts = expectedAccuracy * nTotalCounts / 100;
            int nRange[2] = {0, nTotalCounts - 1};

            lstring strret;
            for (int i = 0; i < nLen; i++)
            {
                vector<int> vct;
                GenNumRandom(vct, nCounts, nLen, nRange, true, NULL);
                sort(vct.begin(), vct.end());
                strret += FormatNumbers(vct, "%d", "") + "-";
            }
            return strret.trim_right('-');
        }

        lstring GenereateCode(const lstring& playName, const lstring& formula, double expectedAccuracy)
        {
            int nCounts = 0;
            int nTotalCounts = 0;
            int nLen = 0;
            lstring strFormat;

            // ����
            if (playName == "wx_zx_ds" && playName == "wx_zx_fs") // ����ֱѡ��ʽ����ʽ
            {
                if (formula == "fx = random()")
                {
                    if (playName.find("ds") != lstring::npos)           return gen_zx_ds_random(5, expectedAccuracy);
                    else                                                return gen_zx_fs_random(5, expectedAccuracy);
                }
            }

            // ����
            else if (playName.find("sx_zx_ds") != string::npos || playName.find("sx_zx_fs") != string::npos) // ����ֱѡ��ʽ����ʽ
            {
                if (formula == "fx = random()")
                {
                    if (playName.find("ds") != lstring::npos)           return gen_zx_ds_random(4, expectedAccuracy);
                    else                                                return gen_zx_fs_random(4, expectedAccuracy);
                }
            }

            // ����
            else if (playName.find("sm_zx_ds") != string::npos || playName.find("sm_zx_fs") != string::npos) // ����ֱѡ��ʽ����ʽ
            {
                if (formula == "fx = random()")
                {
                    if (playName.find("ds") != lstring::npos)           return gen_zx_ds_random(3, expectedAccuracy);
                    else                                                return gen_zx_fs_random(3, expectedAccuracy);
                }
            }

            // ����
            else if (playName == "em_zx_qfs" || playName == "em_zx_qds" || playName == "em_zx_hfs" || playName == "em_zx_hds") // ����ֱѡ��ʽ����ʽ
            {
                if (formula == "fx = random()")
                {
                    if (playName.find("ds") != lstring::npos)           return gen_zx_ds_random(2, expectedAccuracy);
                    else                                                return gen_zx_fs_random(2, expectedAccuracy);
                }
            }

            // һ��
            else if (playName == "dwd_dwd_dwd") // ��λ��
            {
                if (formula == "fx = random()")
                {
                    return gen_zx_ds_random(1, expectedAccuracy);
                }
            }

            // ������


            return "";
        }
    }
}