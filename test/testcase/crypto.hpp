#include "testcase.hpp"

lstring texts[] =
{
    "",
    "abc",
    "The quick brown fox jumps over the lazy dog",
    "无敌超人",
    "你好，hello world!",
    "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ",
//     "RFC2822 is legacy standard of EMail. In section 2.1.1 of RFC2822, you can find reason as below: It also affects MIME. There are two limits that this standard places on the number of characters in a line. Each line of characters MUST be no more than 998 characters, and SHOULD be no more than 78 characters, excluding the CRLF. The 998 character limit is due to limitations in many implementations which send, receive, or store Internet Message Format messages that simply cannot handle more than 998 characters on a line. Receiving implementations would do well to handle an arbitrarily large number of characters in a line for robustness sake. However, there are so many implementations which (in compliance with the transport requirements of [RFC2821]) do not accept messages containing more than 1000 character including the CR and LF per line, it is important for implementations not to create such messages. The more conservative 78 character recommendation is to accommodate the many implementations of user interfaces that display these messages which may truncate, or disastrously wrap, the display of more than 78 characters per line, in spite of the fact that such implementations are non-conformant to the intent of this specification (and that of [RFC2821] if they actually cause information to be lost). Again, even though this limitation is put on messages, it is encumbant upon implementations which display messages to handle an arbitrarily large number of characters in a line (certainly at least up to the 998 character limit) for the sake of robustness."
};

lstring files[] =
{
    "",
//     "./zlibwapi.dll",
//     "zlibwapi_no_exist.dll",
//     "C:\\Windows\\System32\\notepad.exe",
};

void test_md5()
{
    cout << "------------------" << endl;

    for (size_t i = 0; i < sizeof(texts) / sizeof(lstring); i++)
    {
        const lstring& str = texts[i];
        cout << "crypto::md5(\"" << str << "\") = " << crypto::md5(str, str.length()) << endl;
    }

    for (size_t i = 0; i < sizeof(files) / sizeof(lstring); i++)
    {
        const lstring& str = files[i];
        cout << "crypto::file_md5(\"" << str << "\") = " << crypto::file_md5(str) << endl;
    }
}

void test_sha1()
{
    cout << "------------------" << endl;

    for (size_t i = 0; i < sizeof(texts) / sizeof(lstring); i++)
    {
        const lstring& str = texts[i];
        cout << "crypto::sha1(\"" << str << "\") = " << crypto::sha1(str, str.length()) << endl;
    }

    for (size_t i = 0; i < sizeof(files) / sizeof(lstring); i++)
    {
        const lstring& str = files[i];
        cout << "crypto::file_sha1(\"" << str << "\") = " << crypto::file_sha1(str) << endl;
    }
}

void test_sha256()
{
    cout << "------------------" << endl;

    for (size_t i = 0; i < sizeof(texts) / sizeof(lstring); i++)
    {
        const lstring& str = texts[i];
        cout << "crypto::sha256(\"" << str << "\") = " << crypto::sha256(str, str.length()) << endl;
    }

    for (size_t i = 0; i < sizeof(files) / sizeof(lstring); i++)
    {
        const lstring& str = files[i];
        cout << "crypto::file_sha256(\"" << str << "\") = " << crypto::file_sha256(str) << endl;
    }
}

void test_base64()
{
    cout << "------------------" << endl;

    lstring str_en, str_de;
    for (size_t i = 0; i < sizeof(texts) / sizeof(lstring); i++)
    {
        const lstring& str = texts[i];

        str_en = crypto::base64_encode(str, str.length());
        cout << "crypto::base64_encode(\"" << str << "\") = " << str_en << endl;

        str_de = crypto::base64_decode(str_en, str_en.length(), NULL);
        cout << "crypto::base64_decode(\"" << str_en << "\") = " << str_de << endl;
    }
}

void print_hex(_lpcstr str, int len)
{
    for (int j = 0; j < len; j++)
        printf("%02x", (unsigned char)str[j]);
}

void test_aes()
{
    cout << "------------------" << endl;
    lstring key = "1234567812345678";
    _lchar iv[] = "1234567812345678"; // {0}; //{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    printf("** test_aes\n");
    printf("   key: %s\n", key.c_str());
    printf("   iv: (hex) ");
    print_hex(iv, 16);
    printf("\n\n");

    lstring str_en, str_de;
    int len_en, len_de;
    for (size_t i = 0; i < sizeof(texts) / sizeof(lstring); i++)
    {
        const lstring& str = texts[i];
        printf("---plan text: %s\n", str.c_str());

        printf("   ** [aes] **\n");

        printf("     padding: crypto_pkcs7padding\n");
        str_en = crypto::aes_encode(str, str.length(), key, crypto::crypto_bit128, crypto::crypto_pkcs7padding, &len_en);
        printf("     encrypt: (hex) ");
        print_hex(str_en, len_en);
        printf("\n");

        str_de = crypto::aes_decode(str_en, len_en, key, crypto::crypto_bit128, crypto::crypto_pkcs7padding, &len_de);
        printf("     decrypt: %s\n", str_de.c_str());
        printf("       (hex): ");
        print_hex(str_de, len_de);
        printf("\n\n");

        printf("     padding: crypto_zeropadding\n");
        str_en = crypto::aes_encode(str, str.length(), key, crypto::crypto_bit128, crypto::crypto_zeropadding, &len_en);
        printf("     encrypt: (hex) ");
        print_hex(str_en, len_en);
        printf("\n");

        str_de = crypto::aes_decode(str_en, len_en, key, crypto::crypto_bit128, crypto::crypto_zeropadding, &len_de);
        printf("     decrypt: %s\n", str_de.c_str());
        printf("       (hex): ");
        print_hex(str_de, len_de);
        printf("\n\n");

        printf("   ** [cbc] **\n");

        printf("     padding: crypto_pkcs7padding\n");
        str_en = crypto::aes_encode_cbc(str, str.length(), key, crypto::crypto_bit128, crypto::crypto_pkcs7padding, iv, &len_en);
        printf("     encrypt: (hex) ");
        print_hex(str_en, len_en);
        printf("\n");

        str_de = crypto::aes_decode_cbc(str_en, len_en, key, crypto::crypto_bit128, crypto::crypto_pkcs7padding, iv, &len_de);
        printf("     decrypt: %s\n", str_de.c_str());
        printf("       (hex): ");
        print_hex(str_de, len_de);
        printf("\n\n");

        printf("     padding: crypto_zeropadding\n");
        str_en = crypto::aes_encode_cbc(str, str.length(), key, crypto::crypto_bit128, crypto::crypto_zeropadding, iv, &len_en);
        printf("     encrypt: (hex) ");
        print_hex(str_en, len_en);
        printf("\n");

        str_de = crypto::aes_decode_cbc(str_en, len_en, key, crypto::crypto_bit128, crypto::crypto_zeropadding, iv, &len_de);
        printf("     decrypt: %s\n", str_de.c_str());
        printf("       (hex): ");
        print_hex(str_de, len_de);
        printf("\n\n\n");
    }
}

void test_url_encode()
{
    cout << "------------------" << endl;

    lstring texts[] = 
    {
        "this is text with !, #, $, %, +, @, :, =, ?",
        "this is text with CHINESE character：你好！",
    };
    lstring stren, strde;
    for (size_t i = 0; i < sizeof(texts) / sizeof(lstring); i++)
    {
        const lstring& text = texts[i];

        stren = crypto::url_encode(text, text.length());
        cout << "crypto::url_encode(\"" << text << "\") : = " << stren << endl;

        strde = crypto::url_decode(stren, stren.length());
        cout << "crypto::url_decode(\"" << stren << "\") : = " << strde << endl;
    }
}

void test_crypto()
{
    test_md5();
    test_sha1();
    test_sha256();
    test_base64();
    test_aes();
//     test_url_encode();
}

declare_test_case(test_crypto);
