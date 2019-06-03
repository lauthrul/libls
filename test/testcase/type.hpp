#include "testcase.hpp"

void test_type()
{
    char c;
    char c1;
    _lchar lc;
    _lchar lc1;

    c  = '1';
    c = '123';

    lc = '2';
    lc = '234';

    c1 = lc;
    assert(c1 == '4');

    lc1 = c;
    assert(lc1 == '3');
}
declare_test_case(test_type);
