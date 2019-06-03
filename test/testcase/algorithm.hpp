#include "testcase.hpp"

lstring texts[] =
{
    "",
    "ÄãºÃ£¬hello world!",
    "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ",
    "RFC2822 is legacy standard of EMail. In section 2.1.1 of RFC2822, you can find reason as below: It also affects MIME. There are two limits that this standard places on the number of characters in a line. Each line of characters MUST be no more than 998 characters, and SHOULD be no more than 78 characters, excluding the CRLF. The 998 character limit is due to limitations in many implementations which send, receive, or store Internet Message Format messages that simply cannot handle more than 998 characters on a line. Receiving implementations would do well to handle an arbitrarily large number of characters in a line for robustness sake. However, there are so many implementations which (in compliance with the transport requirements of [RFC2821]) do not accept messages containing more than 1000 character including the CR and LF per line, it is important for implementations not to create such messages. The more conservative 78 character recommendation is to accommodate the many implementations of user interfaces that display these messages which may truncate, or disastrously wrap, the display of more than 78 characters per line, in spite of the fact that such implementations are non-conformant to the intent of this specification (and that of [RFC2821] if they actually cause information to be lost). Again, even though this limitation is put on messages, it is encumbant upon implementations which display messages to handle an arbitrarily large number of characters in a line (certainly at least up to the 998 character limit) for the sake of robustness."
};

lstring files[] =
{
    "",
    "./zlibwapi.dll",
    "zlibwapi_no_exist.dll",
    "C:\\Windows\\System32\\notepad.exe",
};

void test_md5()
{
    cout << "------------------" << endl;

    for (size_t i = 0; i < sizeof(texts) / sizeof(lstring); i++)
    {
        const lstring& str = texts[i];
        cout << "md5::md5(\"" << str << "\") = " << md5::md5(str, str.length()) << endl;
    }

    for (size_t i = 0; i < sizeof(files) / sizeof(lstring); i++)
    {
        const lstring& str = files[i];
        cout << "md5::file_md5(\"" << str << "\") = " << md5::file_md5(str) << endl;
    }
}

void test_sha1()
{
    cout << "------------------" << endl;

    for (size_t i = 0; i < sizeof(texts) / sizeof(lstring); i++)
    {
        const lstring& str = texts[i];
        cout << "sha1::sha1(\"" << str << "\") = " << sha1::sha1(str, str.length()) << endl;
    }

    for (size_t i = 0; i < sizeof(files) / sizeof(lstring); i++)
    {
        const lstring& str = files[i];
        cout << "sha1::file_sha1(\"" << str << "\") = " << sha1::file_sha1(str) << endl;
    }
}

void test_base64()
{
    cout << "------------------" << endl;

    lstring str_en, str_de;
    for (size_t i = 0; i < sizeof(texts) / sizeof(lstring); i++)
    {
        const lstring& str = texts[i];

        str_en = base64::encode(str, str.length());
        cout << "base64::encode(\"" << str << "\", -1) = " << str_en << endl;

        str_de = base64::decode(str_en, str_en.length(), NULL);
        cout << "base64::decode(\"" << str_en << "\") = " << str_de << endl;

        str_en = base64::encode(str, str.length(), 64);
        cout << "base64::encode(\"" << str << "\", 64) = " << str_en << endl;

        str_de = base64::decode(str_en, str_en.length(), NULL);
        cout << "base64::decode(\"" << str_en << "\") = " << str_de << endl;

        str_en = base64::encode(str, str.length(), 76);
        cout << "base64::encode(\"" << str << "\", 76) = " << str_en << endl;

        str_de = base64::decode(str_en, str_en.length(), NULL);
        cout << "base64::decode(\"" << str_en << "\") = " << str_de << endl;
    }
}

void test_algorithm()
{
    test_md5();
    test_sha1();
    test_base64();
}

declare_test_case(test_algorithm);
