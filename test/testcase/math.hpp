#include "testcase.hpp"
#include <iomanip>
using namespace lslib::math;
using namespace lslib::os;

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

    //////////////////////////////////////////////////////////////////////////
    int iv[] = { 1, 2, 3, 4, 5, 6 };
    int iv_len = sizeof(iv) / sizeof(int);
    int nu = 3;
    vector<int> v;
    v.assign(iv, iv + iv_len);
    vector<vector<int>> result;

    //
    permutation(v, nu, result);

    cout << "permutation ( ";
    for (int i = 0; i < iv_len; i++)
        cout << iv[i] << ", ";
    cout << ") in " << nu << " : [" << result.size() << "]" << endl;
    for (int i = 0; i < result.size(); i++)
    {
        cout << "\t{ ";
        for (int j = 0; j < result[i].size(); j++)
            cout << result[i][j] << " ";
        cout << " }" << endl;
    }

    //
    result.clear();
    combination(v, nu, result);

    cout << "combination ( ";
    for (int i = 0; i < iv_len; i++)
        cout << iv[i] << ", ";
    cout << ") in " << nu << " : [" << result.size() << "]" << endl;
    for (int i = 0; i < result.size(); i++)
    {
        cout << "\t{ ";
        for (int j = 0; j < result[i].size(); j++)
            cout << result[i][j] << " ";
        cout << " }" << endl;
    }

    cout << "------------------" << endl;
}
declare_test_case(test_math);