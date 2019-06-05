#include "testcase.hpp"

void test_string()
{
    _lpcstr p = NULL;
    lstring str = p;
    str = lstring((_lpstr)NULL);
    str = lstring("\0 123");
}
declare_test_case(test_string);
