#pragma warning(disable:4251)
#pragma warning(disable:4805)
#pragma warning(disable:4800)
#pragma warning(disable:4996)

#define USE_LOG4CXX
#define USE_LIBCURL
#define USE_LIBICONV

#include <libls.h>
using namespace lslib;

//////////////////////////////////////////////////////////////////////////

typedef string(*fn_crypto) (lpbyte, size_t);
typedef string(*fn_crypto2) (lpbyte, size_t, int*);

void print_usage()
{
    lpcstr usage =  "usage: \n"
                    "    cryptotool <crypto> [options...] <data>\n"
                    "param:\n"
                    "   <crypto>            crypto type to be perform. should be one of belows:\n"
                    "                           [md5|sha1|sha224|sha256|sha384|sha512|\n"
                    "                           base64_encode|base64_decode|\n"
                    "                           des_encrypt|des_decrypt|3des_encrypt|3des_decrypt|\n"
                    "                           aes_encrypt|aes_decrypt\n"
                    "                           url_encode|url_decode|\n"
                    "                           encoding_convert]\n"
                    "   [options...]        detail as below\n"
                    "   <data>              data to be perform\n"
                    "options: \n"
                    "    -f|--file          crypto from a specificed file path\n"
                    "    -o|--output        specific a output file path\n"
                    "    -m|--mode          crypt mode: [ecb|cbc]\n"
                    "    -p|--padding       padding mode: [none|zore|pkcs5|pkcs7]\n"
                    "    -k|--key           crypt key\n"
                    "    -kl|--keylen       crypt key length\n"
                    "                           for des, no need to specific, always be 8byte(64bit)\n"
                    "                           for aes, must be one of [64|128|192|256]bit\n"
                    "    -iv                crypt iv. for cbc mode\n"
                    "                           for des. must be length of 8byte\n"
                    "                           for aes. must be length of 16byte\n"
                    "    -F|--from          from charset. for encoding_convert\n"
                    "    -T|--to            to charset. for encoding_convert\n";

    printf(usage);
}

int main(int argc, lpcstr argv[])
{
    int ret = 0;
    string crypto;
    string file;
    string output;
    string mode;
    string padding;
    string key;
    string keylen;
    string iv;
    string from;
    string to;
    string data;
    int datalen = 0;
    string result;
    int resultlen = 0;

    if (argc < 2) ret = -1;
    else
    {
        crypto = argv[1];

#define get_param(param) \
    if (i + 1 >= argc) \
        { \
        ret = -1; \
        break; \
        } \
    param = argv[++i]; \

        int i = 2;
        while (i < argc)
        {
            if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--file") == 0)
            {
                get_param(file);
            }
            else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0)
            {
                get_param(output);
            }
            else if (strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--mode") == 0)
            {
                get_param(mode);
            }
            else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--padding") == 0)
            {
                get_param(padding);
            }
            else if (strcmp(argv[i], "-k") == 0 || strcmp(argv[i], "--key") == 0)
            {
                get_param(key);
            }
            else if (strcmp(argv[i], "-kl") == 0 || strcmp(argv[i], "--keylen") == 0)
            {
                get_param(keylen);
            }
            else if (strcmp(argv[i], "-iv") == 0)
            {
                get_param(iv);
            }
            else if (strcmp(argv[i], "-F") == 0 || strcmp(argv[i], "--from") == 0)
            {
                get_param(from);
            }
            else if (strcmp(argv[i], "-T") == 0 || strcmp(argv[i], "--to") == 0)
            {
                get_param(to);
            }
            else
            {
                if (data.empty())
                {
                    data = argv[i];
                    datalen = data.length();
                }
            }
            i++;
        }
    }

    if (ret == -1)
    {
        print_usage();
        return ret;
    }

    if (!file.empty())
    {
        lpbyte pdata = os::get_file_buffer(file.c_str(), (lpdword)&datalen);
        data.assign((lpcstr)pdata, datalen);
        os::release_file_buffer(pdata);
    }

    fn_crypto fn = NULL;
    fn_crypto2 fn2 = NULL;

    if (crypto == "md5")                    fn = crypto::md5;
    else if (crypto == "sha1")              fn = crypto::sha1;
    else if (crypto == "sha224")            fn = crypto::sha224;
    else if (crypto == "sha256")            fn = crypto::sha256;
    else if (crypto == "sha384")            fn = crypto::sha384;
    else if (crypto == "sha512")            fn = crypto::sha512;
    else if (crypto == "base64_encode")     fn2 = crypto::base64_encode;
    else if (crypto == "base64_decode")     fn2 = crypto::base64_decode;
    else if (crypto == "url_encode")        fn2 = crypto::url_encode;
    else if (crypto == "url_decode")        fn2 = crypto::url_decode;

    if (fn != NULL)
    {
        result = fn((lpbyte)data.c_str(), datalen);
        resultlen = result.length();
        goto label_out;
    }
    if (fn2 != NULL)
    {
        result = fn2((lpbyte)data.c_str(), datalen, &resultlen);
        goto label_out;
    }

    if (crypto == "des_encrypt" || crypto == "des_decrypt" || crypto == "3des_encrypt" || crypto == "3des_decrypt" || crypto == "aes_encrypt" || crypto == "aes_decrypt")
    {
        do
        {
            if (mode != "ecb" && mode != "cbc")
            {
                ret = -2;
                break;
            }
            int nkeylen = strtool::to_int(keylen);
            if (crypto == "aes_encrypt" || crypto == "aes_decrypt")
            {
                if (nkeylen != 128 && nkeylen != 192 && nkeylen != 256)
                {
                    ret = -2;
                    break;
                }
            }
            crypto::crypto_padding_mode ep;
            if (padding == "none")          ep = crypto::crypto_nopadding;
            else if (padding == "zore")     ep = crypto::crypto_zeropadding;
            else if (padding == "pkcs5")    ep = crypto::crypto_pkcs5padding;
            else if (padding == "pkcs7")    ep = crypto::crypto_pkcs7padding;
            else
            {
                ret = -2;
                break;
            }

            if (crypto.find("des_") == 0)
            {
                if (crypto.find("_encrypt") != string::npos)
                {
                    if (mode == "ecb")
                        result = crypto::des_encrypt((lpbyte)data.c_str(), datalen, key.c_str(), ep, &resultlen);
                    else
                        result = crypto::des_encrypt_cbc((lpbyte)data.c_str(), datalen, key.c_str(), ep, iv.c_str(), &resultlen);
                }
                else if (crypto.find("_decrypt") != string::npos)
                {
                    if (mode == "ecb")
                        result = crypto::des_decrypt((lpbyte)data.c_str(), datalen, key.c_str(), ep, &resultlen);
                    else
                        result = crypto::des_decrypt_cbc((lpbyte)data.c_str(), datalen, key.c_str(), ep, iv.c_str(), &resultlen);
                }
            }
            else if (crypto.find("3des_") == 0)
            {
                if (crypto.find("_encrypt") != string::npos)
                {
                    if (mode == "ecb")
                        result = crypto::three_des_encrypt((lpbyte)data.c_str(), datalen, key.c_str(), ep, &resultlen);
                    else
                        result = crypto::three_des_encrypt_cbc((lpbyte)data.c_str(), datalen, key.c_str(), ep, iv.c_str(), &resultlen);
                }
                else if (crypto.find("_decrypt") != string::npos)
                {
                    if (mode == "ecb")
                        result = crypto::three_des_decrypt((lpbyte)data.c_str(), datalen, key.c_str(), ep, &resultlen);
                    else
                        result = crypto::three_des_decrypt_cbc((lpbyte)data.c_str(), datalen, key.c_str(), ep, iv.c_str(), &resultlen);
                }
            }
            else
            {
                if (crypto.find("_encrypt") != string::npos)
                {
                    if (mode == "ecb")
                        result = crypto::aes_encrypt((lpbyte)data.c_str(), datalen, key.c_str(), (crypto::crypto_key_bits)nkeylen, ep, &resultlen);
                    else
                        result = crypto::aes_encrypt_cbc((lpbyte)data.c_str(), datalen, key.c_str(), (crypto::crypto_key_bits)nkeylen, ep, iv.c_str(), &resultlen);
                }
                else if (crypto.find("_decrypt") != string::npos)
                {
                    if (mode == "ecb")
                        result = crypto::aes_decrypt((lpbyte)data.c_str(), datalen, key.c_str(), (crypto::crypto_key_bits)nkeylen, ep, &resultlen);
                    else
                        result = crypto::aes_decrypt_cbc((lpbyte)data.c_str(), datalen, key.c_str(), (crypto::crypto_key_bits)nkeylen, ep, iv.c_str(), &resultlen);
                }
            }

        }
        while (0);

        if (ret == -2)
        {
            string str;
            if (crypto == "des_encrypt")    str = "usage:\n des_encrypt -m [ecb|cbc] -k <8bytes key> -p [none|zore|pkcs5|pkcs7] [-f] [-o [output]] <data>";
            if (crypto == "des_decrypt")    str = "usage:\n des_decrypt -m [ecb|cbc] -k <8bytes key> -p [none|zore|pkcs5|pkcs7] [-f] [-o [output]] <data>";
            if (crypto == "3des_encrypt")   str = "usage:\n 3des_encrypt -m [ecb|cbc] -k <24bytes key> -p [none|zore|pkcs5|pkcs7] [-f] [-o [output]] <data>";
            if (crypto == "3des_decrypt")   str = "usage:\n 3des_decrypt -m [ecb|cbc] -k <24bytes key> -p [none|zore|pkcs5|pkcs7] [-f] [-o [output]] <data>";
            if (crypto == "aes_encrypt")    str = "usage:\n aes_encrypt -m [ecb|cbc] -k <key> -kl [128|192|256] -p [none|zore|pkcs5|pkcs7] [-f] [-o [output]] <data>";
            if (crypto == "aes_decrypt")    str = "usage:\n aes_decrypt -m [ecb|cbc] -k <key> -kl [128|192|256] -p [none|zore|pkcs5|pkcs7] [-f] [-o [output]] <data>";
            printf(str.c_str());
            return 0;
        }
        else goto label_out;
    }
    else if (crypto == "encoding_convert")
    {
        result = crypto::encoding_convert(data.c_str(), datalen, from.c_str(), to.c_str(), &resultlen);
        goto label_out;
    }

label_out:
    if (ret == 0)
    {
        if (!output.empty())        os::save_buffer_to_file((lpbyte)result.c_str(), resultlen, output.c_str(), 0);
        else                        printf("%s\n", result.c_str());
        return 0;
    }
}
