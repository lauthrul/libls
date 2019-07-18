#include "testcase.hpp"

void zip_file_test()
{
    string strsz[] =
    {
        "./log4cxx.xml",
        "logcfg.xml",
        "./log/logapp.log",
    };

    printf("------------------zip_file_test\n");
    for (int i = 0; i < sizeof(strsz) / sizeof(string); i++)
    {
        const string& str = strsz[i];
        string dst_zip = os::path_get_filename(str.c_str()) + ".zip";
        int ret = zip::zip_file(dst_zip.c_str(), str.c_str(), "123456");
        printf("zip file: %s -> %s === [%d]\n", str.c_str(), dst_zip.c_str(), ret);
    }
}

int zip_progress(void* clientp, int dltotal, int dlnow, int error)
{
    printf("\t %d/%d [%d]\n", dlnow, dltotal, error);
    return 0;
}

void zip_folder_test()
{
    string strsz[] =
    {
        "./log",
        "download_test",
        "C:\\Program Files (x86)\\Windows Mail",
    };

    printf("------------------zip_folder_test\n");
    for (int i = 0; i < sizeof(strsz) / sizeof(string); i++)
    {
        const string& str = strsz[i];
        string dst_zip = os::path_get_name(str.c_str()) + ".zip";
        int ret = zip::zip_folder(dst_zip.c_str(), str.c_str(), NULL, zip_progress);
        printf("zip folder: %s -> %s === [%d]\n", str.c_str(), dst_zip.c_str(), ret);
    }
}

void unzip_test()
{
    string strsz[] =
    {
        "./log4cxx.zip",
        "logcfg.zip",
        "./logapp.zip",
        "log.zip",
        "download_test.zip",
        "Windows Mail.zip",
    };

    printf("------------------unzip_test\n");
    for (int i = 0; i < sizeof(strsz) / sizeof(string); i++)
    {
        const string& str = strsz[i];
        const char* dst_dir = "unzip";
        int ret = zip::unzip(str.c_str(), dst_dir, "123456", zip_progress);
        printf("unzip: %s -> %s === [%d]\n", str.c_str(), dst_dir, ret);
    }
}

void test_zip()
{
    zip_file_test();
    zip_folder_test();
    unzip_test();
}
declare_test_case(test_zip);
