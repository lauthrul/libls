#pragma once

#include "lslib.h"
using namespace lslib;

#define singile_test 1

typedef void (*fn_test) (void);

declare_stl_obj(fn_test);

fn_test_list test_cases;

template<class T>
class test_wrapper
{
    public:
        test_wrapper(T t)
        {
#if singile_test
            test_cases.clear();
#endif
            test_cases.push_back(t);
        }
};

#define declare_test_case(fn)   test_wrapper<fn_test> fn##_wrapper(fn);
