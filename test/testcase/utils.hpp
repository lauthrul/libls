#include "testcase.hpp"
using namespace lslib::utils;

void test_utils()
{
    string str = "da3fed134a";
    lbyte_array arr = hex_str_to_byte_array(str);
    for (size_t i = 0; i < arr.size(); i++)
        printf("%x ", arr[i]);
    string strhex = byte_array_to_hex_str(arr);
    printf("\n%s ", strhex.c_str());
}
declare_test_case(test_utils);
