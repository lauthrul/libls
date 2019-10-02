#include "testcase.hpp"

void test_string()
{
    lpcstr p = NULL;
    string str = p;
    str = string((lpstr)NULL);
    str = string("\0 123");
}
declare_test_case(test_string);
