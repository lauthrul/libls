#pragma once
#include <math.h>

namespace lottery
{
    bool GenNumRandom(__out__ vector<int>& vctNum, __inout__ int& nCounts, int nNumLen, int pRange[2], bool bCheckDumplicate, const vector<int> *pvctExclude /*= NULL*/)
    {
        if (nCounts <= 0 || nNumLen <= 0 || nNumLen > 9) // nNumLen最大为9，因为int类型最大支持10^9位数的数字, 10^10以上将溢出
            return false;

        // 最终产生的随机数范围
        int nRange[2] = {0};
        if (pRange != NULL)
        {
            nRange[0] = pRange[0];
            nRange[1] = pRange[1];
        }
        else
        {
            nRange[0] = 0;
            nRange[1] = pow((double)10, (double)nNumLen) - 1; // nItemLen位数字的最大值
        }
        if (nCounts > nRange[1])
            nCounts = nRange[1] + 1;

        // 0 ~ nRange[1]范围内每个数占用标记，bit位表示，占用后标记为1
        _lbyte *btFlag = NULL;
        int nSize = nRange[1]/8 + 1;
        btFlag = (_lbyte *)malloc(nSize);
        memset(btFlag, 0, nSize);
        // 设置占用标记
        int nOccupiedCounts = 0;
        if (pvctExclude != NULL)
        {
            int nNum = 0;
            for (vector<int>::const_iterator it = pvctExclude->begin(); it != pvctExclude->end(); it++)
            {
                nNum = *it;
                if (nNum < 0 || nNum > nRange[1])
                    continue;
                btFlag[nNum/8] |= (1 << (nNum % 8));
                nOccupiedCounts++;
            }
        }

        int nRandom = 0, nNum = 0;
        static int sSrand = 0;
        srand(time(NULL) + sSrand++);
        while (vctNum.size() < nCounts && nOccupiedCounts < nRange[1] - nRange[0] + 1)
        {
            if (nRange[1] > RAND_MAX) // 如果随机数范围大于RAND_MAX(32768, 1byte最大值), 则使用2byte移位来支持
                nRandom = rand() << 8 | rand();
            else
                nRandom = rand();
            nNum = (nRandom % (nRange[1] - nRange[0] + 1)) + nRange[0]; // 使产生的随机数位于nRange[0] ~ nRange[1]范围内
            if (bCheckDumplicate)
            {
                if (0 != (btFlag[nNum/8] & (1 << (nNum % 8)))) //nNum的记录在第nNum/8个字节的第nNum%8位
                    continue;

                btFlag[nNum/8] |= (1 << (nNum % 8));
                nOccupiedCounts++;
            }
            vctNum.push_back(nNum);
        }

        free(btFlag);
        return true;
    }

    string FormatNumbers(const vector<int>& vctNums, const string& strFormat, const string& strSplit)
    {
        string strOut;
        for (size_t i = 0; i < vctNums.size(); i++)
        {
            strOut += strtool::format(strFormat.c_str(), vctNums[i]);
            strOut += strSplit;
        }
        if (strSplit.length() > 0)
            strOut = strOut.substr(0, strOut.length() - strSplit.length());
        return strOut;
    }
}
