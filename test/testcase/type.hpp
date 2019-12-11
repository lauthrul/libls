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

void test_type_size()
{
    printf("lint8:    %d\n", sizeof(lint8));
    printf("lint16:   %d\n", sizeof(lint16));
    printf("lint32:   %d\n", sizeof(lint32));
    printf("lint64:   %d\n", sizeof(lint64));
    printf("luint8:   %d\n", sizeof(luint8));
    printf("luint16:  %d\n", sizeof(luint16));
    printf("luint32:  %d\n", sizeof(luint32));
    printf("luint64:  %d\n", sizeof(luint64));
//     printf("lvoid:    %d\n", sizeof(lvoid));
    printf("lbool:    %d\n", sizeof(lbool));
    printf("lint:     %d\n", sizeof(lint));
    printf("lfloat:   %d\n", sizeof(lfloat));
    printf("ldouble:  %d\n", sizeof(ldouble));
    printf("luint:    %d\n", sizeof(luint));
    printf("lbyte:    %d\n", sizeof(lbyte));
    printf("lword:    %d\n", sizeof(lword));
    printf("ldword:   %d\n", sizeof(ldword));
    printf("luchar:   %d\n", sizeof(luchar));
    printf("lchar:    %d\n", sizeof(lchar));
    printf("lwchar:   %d\n", sizeof(lwchar));

    printf("lpint8:   %d\n", sizeof(lpint8));
    printf("lpint16:  %d\n", sizeof(lpint16));
    printf("lpint32:  %d\n", sizeof(lpint32));
    printf("lpint64:  %d\n", sizeof(lpint64));
    printf("lpuint8:  %d\n", sizeof(lpuint8));
    printf("lpuint16: %d\n", sizeof(lpuint16));
    printf("lpuint32: %d\n", sizeof(lpuint32));
    printf("lpuint64: %d\n", sizeof(lpuint64));
    printf("lpvoid:   %d\n", sizeof(lpvoid));
    printf("lpbool:   %d\n", sizeof(lpbool));
    printf("lpint:    %d\n", sizeof(lpint));
    printf("lpfloat:  %d\n", sizeof(lpfloat));
    printf("lpdouble: %d\n", sizeof(lpdouble));
    printf("lpuint:   %d\n", sizeof(lpuint));
    printf("lpbyte:   %d\n", sizeof(lpbyte));
    printf("lpword:   %d\n", sizeof(lpword));
    printf("lpdword:  %d\n", sizeof(lpdword));
    printf("lpustr:   %d\n", sizeof(lpustr));
    printf("lpcustr:  %d\n", sizeof(lpcustr));
    printf("lpstr:    %d\n", sizeof(lpstr));
    printf("lpcstr:   %d\n", sizeof(lpcstr));
    printf("lpwstr:   %d\n", sizeof(lpwstr));
    printf("lpcwstr:  %d\n", sizeof(lpcwstr));
}

void test_quick_define()
{
    quick_define_map(int, string);
    int_string_map mp;
    mp[0] = "00";
    mp[1] = "11";
    mp[2] = "22";
}

void test_type()
{
    test_enum();
    test_type_size();
    test_quick_define();
}

declare_test_case(test_type);
