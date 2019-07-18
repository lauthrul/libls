#include "testcase.hpp"

void test_string()
{
    _lpcstr p = NULL;
    string str = p;
    str = string((_lpstr)NULL);
    str = string("\0 123");
}
declare_test_case(test_string);
