#include "testcase.hpp"

enum enum_test { ea, eb, ec};
declare_enum_str(enum_test,);
enum_str_begin(enum_test)
    enum_str_item(ea)
    enum_str_item(eb)
    enum_str_item(ec)
enum_str_end(enum_test)

void test_enum()
{
    cout << "enum enum_test { ea, eb, ec}" << endl;

    string str = enum_str(enum_test, ea);
    cout << "enum_str(enum_test, ea) = \"" << str << "\"" << endl;

    enum_test e = enum_from_str(enum_test, "eb");
    cout << "enum_from_str(enum_test, \"eb\") = " << e << endl;
}

void test_type()
{
    char c;
    char c1;
    lchar lc;
    lchar lc1;

    c  = '1';
    c = '123';

    lc = '2';
    lc = '234';

    c1 = lc;
    assert(c1 == '4');

    lc1 = c;
    assert(lc1 == '3');

    test_enum();
}
declare_test_case(test_type);
