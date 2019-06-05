#include "testcase.hpp"
using namespace lslib::utils;

void test_utils()
{
    lstring str = "da3fed134a";
    _lbyte_array arr = hex_str_to_hex(str);
    for (size_t i = 0; i < arr.size(); i++)
        printf("%x ", arr[i]);
    lstring strhex = hex_to_hex_str(arr);
    printf("\n%s ", strhex.c_str());
}
declare_test_case(test_utils);
