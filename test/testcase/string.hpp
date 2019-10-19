#include "testcase.hpp"
using namespace strtool;

void test_string()
{

    printf("===test for strtool===\n");
    printf("------------------\n");

    string arr1[] = {"", " \n  \t", "abc\tdef\n", "123456789012", "0.000", "0.456", "-123.856", "tRUe", "false", "0", "-1", };
    for (size_t i = 0; i < sizeof(arr1) / sizeof(string); i++)
    {
        const string& str = arr1[i];
        printf("[%s] ==>\n", str.c_str());
        printf("  is_empty: %d\n", strtool::is_empty(str));
        printf("  is_space: %d\n", is_space(str));
        printf("  is_upper: %d\n", is_upper(str));
        printf("  is_lower: %d\n", is_lower(str));
        printf("  is_digital: %d\n", is_digital(str));
        printf("  is_int: %d\n", is_int(str));
        printf("  is_float: %d\n", is_float(str));
        printf("  is_bool: %d\n", is_bool(str));
    }

    printf("------------------\n");
    string arr2[] = { "abc", "123456789012", "0.000", "0.456", "-123.856", "tRUe", "false", "0", "-1", };
    for (size_t i = 0; i < sizeof(arr2) / sizeof(string); i++)
    {
        const string& str = arr2[i];
        printf("[%s] ==>\n", str.c_str());
        printf("  upper: %s\n", upper(str).c_str());
        printf("  lower: %s\n", lower(str).c_str());
        printf("  to_int: %d\n", to_int(str));
        printf("  to_int64: %lld\n", to_int64(str));
        printf("  to_float: %f\n", to_float(str));
        printf("  to_bool: %s\n", to_bool(str) ? "true" : "false");
    }

    printf("------------------\n");
    double arr3[] = { 123456789012L, 0.000f, 0.456, -123.856, true, false, 0, -1, };
    for (size_t i = 0; i < sizeof(arr3) / sizeof(double); i++)
    {
        double value = arr3[i];
        cout << value << "==>" << endl;
        printf("  from_int: %s\n", from_int(value).c_str());
        printf("  from_int64: %s\n", from_int64(value).c_str());
        printf("  from_float: %s\n", from_float(value, 3).c_str());
        printf("  from_bool: %s\n", from_bool((bool)value).c_str());
    }

    printf("------------------\n");
    string arr4[] = { "ab,ab,abc,ccc,cba,ba,ba", "  \t\n \v ab,ab,abc,ccc,cba,ba,ba  \t\n \v", };
    for (size_t i = 0; i < sizeof(arr4) / sizeof(string); i++)
    {
        const string& str = arr4[i];
        printf("[%s] ==>\n", str.c_str());
        printf("  trim_left: %s\n", trim_left(str).c_str());
        printf("  trim_left(a): %s\n", trim_left(str, 'a').c_str());
        printf("  trim_left(ab,): %s\n", trim_left(str, "ab,").c_str());
        printf("  trim_right: %s\n", trim_right(str).c_str());
        printf("  trim_left(a): %s\n", trim_right(str, 'a').c_str());
        printf("  trim_left(ab,): %s\n", trim_right(str, "ab,").c_str());
        printf("  trim: %s\n", trim(str).c_str());
        printf("  trim(a): %s\n", trim(str, 'a').c_str());
        printf("  trim(ab,): %s\n", trim(str, "ab,").c_str());

        printf("  split(,): ");
        string_array arr; split(arr, str, ",");
        for (size_t j = 0; j < arr.size(); j++)
            printf("%s ", arr[j].c_str());
        printf("\n");
    }

    printf("------------------\n");
    string arr5[] = { "da3Fed134A", "123456789"};
    for (size_t i = 0; i < sizeof(arr5) / sizeof(string); i++)
    {
        const string& str = arr5[i];
        printf("[%s] ==>\n", str.c_str());

        printf("  hex_str_to_byte_array: ");
        lbyte_array arr = hex_str_to_byte_array(str);
        for (size_t i = 0; i < arr.size(); i++)
            printf("%x ", arr[i]);

        printf("\n  thex_str_to_byte_array: %s\n", byte_array_to_hex_str(arr).c_str());
    }
}
declare_test_case(test_string);
