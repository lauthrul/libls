#include "testcase.hpp"

void test_mmr()
{
    _lpbyte p = lsalloc(0);
    lsfree(p);

    p = lsalloc(9999999999999999999);
    lsfree(p);

    lsfree(0);
}
declare_test_case(test_mmr);
