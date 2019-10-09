#include "testcase.hpp"
using namespace lslib::utils;

void test_utils()
{
    string str = gen_uuid();
    printf(str.c_str());
}
declare_test_case(test_utils);
