#include "testcase.hpp"
#include <iomanip>
using namespace lslib::math;
using namespace lslib::os;

Time start, end;
double delay;

void test_efficiency()
{
    //////////////////////////////////////////////////////////////////////////
    int iv[] = { 1, 2, 3, 4, 5, 6 };
    int iv_len = sizeof(iv) / sizeof(int);
    int nu = 3;
    vector<int> v;
    v.assign(iv, iv + iv_len);
    vector<vector<int>> result;

    //
    int sum = 0;
    for (int loop = 0; loop < 100000; loop++)
    {
//         sum += loop; continue;
        permutation(v, nu, result);

//         cout << "permutation ( ";
//         for (int i = 0; i < iv_len; i++)
//             cout << iv[i] << ", ";
//         cout << ") in " << nu << " : [" << result.size() << "]" << endl;
//         for (int i = 0; i < result.size(); i++)
//         {
//             cout << "\t{ ";
//             for (int j = 0; j < result[i].size(); j++)
//                 cout << result[i][j] << " ";
//             cout << " }" << endl;
//         }

        //
        result.clear();
        combination(v, nu, result);

//         cout << "combination ( ";
//         for (int i = 0; i < iv_len; i++)
//             cout << iv[i] << ", ";
//         cout << ") in " << nu << " : [" << result.size() << "]" << endl;
//         for (int i = 0; i < result.size(); i++)
//         {
//             cout << "\t{ ";
//             for (int j = 0; j < result[i].size(); j++)
//                 cout << result[i][j] << " ";
//             cout << " }" << endl;
//         }
    }
    cout << "------------------" << endl;
};

// void produce_next(const string& pattern, int next[], int len)
// {
//     int j = 0;
//     next[j] = j;
//     for (int i = 1; i < len; ++i)
//     {
//         while (j > 0 && pattern.at(i) != pattern.at(j))
//             j = next[j - 1];
//         if (pattern.at(i) == pattern.at(j))
//             ++j;
//         next[i] = j;
// 
//     }
// }
// bool KMP(const string& str, const string& pattern, int* index = NULL)
// {
//     if (str.size() == 0 || pattern.size() == 0)
//         return false;
//     int m = str.size(), n = pattern.size();
//     int* next = new int[n];
//     produce_next(pattern, next, n);
//     int j = 0;
//     for (int i = 0; i < m; ++i)
//     {
//         while (j > 0 && str.at(i) != pattern.at(j))
//             j = next[j - 1];
//         if (str.at(i) == pattern.at(j))
//             ++j;
//         if (j == n)
//         {
//             if (index != NULL)
//                 *index = i - j + 1;
//             return true;
//         }
//     }
//     return false;
// }

int kmp(const string& str, const string& pattern)
{
    if (pattern.empty()) return 0;

    int* next = new int[pattern.size()];
    std::auto_ptr<int> piptr(next);

    memset(next, 0, pattern.size());
    for (int i = 1, k = 0; i < pattern.size(); ++i)
    {
        while (k && pattern[k] != pattern[i]) k = next[k - 1];
        if (pattern[k] == pattern[i]) ++k;
        next[i] = k;
    }

    for (int i = 0, k = 0; i < str.size(); ++i)
    {
        while (k && pattern[k] != str[i]) k = next[k - 1];
        if (pattern[k] == str[i]) ++k;
        if (k == pattern.size()) return i - k + 1;
    }

    return -1;
}

class COrder
{
    public:
        string code;
        string method;
        int counts;
        double price_unit;
        int times;
        double cost;
        double award;
        double profit;
        double odds;
        bool win;

        static map<string, double> profitmap;

        bool test_win(const string& open_code)
        {
            if (method == "wx_zx_fs")
            {
                string_array arr;
                code.split(arr, "-");
                for (size_t j = 0; j < min(arr.size(), 5); j++)
                {
                    if (arr[j].find(open_code[j]) == string::npos)
                        return false;
                }
                return true;
            }
            else if (method == "wx_zx_ds")
            {
                return code.find(open_code) != string::npos;
                return kmp(code, open_code) > 0;
            }
            return false;
        }

        void open(const string& open_code)
        {
            win = test_win(open_code);

            cost = times * counts * price_unit;
            award = win ?  times * (odds * price_unit / 2) : 0;
            profit = award - cost;

            profitmap[open_code] += profit;
        }
};
map<string, double> COrder::profitmap;

void profite_calc()
{
    list<COrder> orders;

    COrder order;
    order.method = "wx_zx_fs";
    order.code = "123-456-789-012-345";
    order.counts = 243;
    order.price_unit = 2;
    order.times = 1;
    order.odds = 180000;
    orders.push_back(order);

    order.method = "wx_zx_ds";
    int rand_value = 0;
    string strcode;
    for (size_t i = 0; i < 10; i++)
    {
        rand_value = rand() % 10;
        order.counts = rand_value;
        order.code.clear();
        for (size_t j = 0; j < order.counts; j++)
        {
            rand_value = rand() % 100000;
            order.code += strcode.format("%05d", rand_value) + " ";
        }
        order.code.trim_right();
        orders.push_back(order);
    }

    string stropen;
    for (list<COrder>::iterator it = orders.begin(); it != orders.end(); it++)
    {
//         printf("\r%d", distance(orders.begin(), it));
//         fflush(stdout);
        start = Time::GetCurDateTime();
        for (int i = 0; i < 100000; i++)
        {
            stropen.format("%05d", i);
            it->open(stropen);
        }
        end = Time::GetCurDateTime();
        delay = end.GetDateTime() - start.GetDateTime() + (end.GetMilliSec() - start.GetMilliSec()) / (float)1000;
        printf("index: %d, counts: %d, cost: %0.3f s\n", distance(orders.begin(), it), it->counts, delay);
    }

    pair<string, double> min_profile_open_code = *COrder::profitmap.begin();
    for (map<string, double>::iterator it = COrder::profitmap.begin();
            it != COrder::profitmap.end(); it++)
    {
        if (min_profile_open_code.second > it->second)
        {
            min_profile_open_code.second = it->second;
            min_profile_open_code.first = it->first;
        }
    }
    cout << "best open code: " << min_profile_open_code.first << ", use total profit: " << min_profile_open_code.second << endl;
}

void test_math()
{
    double sz[] =
    {
        0.1234567890123456789,
        12.345678901234567890,
        234.56789012345678901,
        3456.7890123456789012,
        45678.901234567890123,
        567890.12345678901234,
        6789012.3456789012345,
        78901234.567890123456,
        890123456.78901234567,
        9012345678.9012345678,
    };

    for (int i = 0; i < sizeof(sz) / sizeof(double); i++)
    {
        cout << "Round(" << setiosflags(ios::fixed) << setprecision(11) << sz[i] << ", " << i << "): " << Round(sz[i], i) << endl;
        cout << "Round(" << setiosflags(ios::fixed) << setprecision(11) << -sz[i] << ", " << i << "): " << Round(-sz[i], i) << endl;
        cout << "Floor(" << setiosflags(ios::fixed) << setprecision(11) << sz[i] << ", " << i << "): " << Floor(sz[i], i) << endl;
        cout << "Floor(" << setiosflags(ios::fixed) << setprecision(11) << -sz[i] << ", " << i << "): " << Floor(-sz[i], i) << endl;
        cout << "Ceil(" << setiosflags(ios::fixed) << setprecision(11) << sz[i] << ", " << i << "): " << Ceil(sz[i], i) << endl;
        cout << "Ceil(" << setiosflags(ios::fixed) << setprecision(11) << -sz[i] << ", " << i << "): " << Ceil(-sz[i], i) << endl;
        cout << "------------------" << endl;
    }

    Time start = Time::GetCurDateTime();
    cout << start.GetCurDateTimeStr(true) << " start ..." << endl;

    profite_calc();

    Time end = Time::GetCurDateTime();
    cout << start.GetCurDateTimeStr(true) << " end. cost: "
         << end.GetDateTime() - start.GetDateTime() + (end.GetMilliSec() - start.GetMilliSec()) / (float)1000 << " seconds."  << endl;

}
declare_test_case(test_math);