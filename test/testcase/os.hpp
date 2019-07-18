#include "testcase.hpp"
using namespace lslib::os;

void path_test()
{
    // test for:
    //     LSLIB_API const _lchar get_slash();
    //     LSLIB_API const bool is_slash(_lchar c);
    //     LSLIB_API string path_get_dir(_lpcstr path);
    //     LSLIB_API string path_get_name(_lpcstr path);
    //     LSLIB_API string path_get_filename(_lpcstr path);
    //     LSLIB_API string path_get_ext(_lpcstr path);
    //     LSLIB_API string path_pretty(_lpcstr path);
    //     LSLIB_API string path_combine(_lpcstr path, _lpcstr join);
    //     LSLIB_API string path_make_absolute(_lpcstr path);
    //     LSLIB_API const bool is_exist(_lpcstr path);
    //     LSLIB_API const bool is_file(_lpcstr path);
    //     LSLIB_API const bool is_dir(_lpcstr path);
    //     LSLIB_API const bool is_absolute(_lpcstr path);

    //////////////////////////////////////////////////////////////////////////
    // get_slash, is_slash
    {
        printf("------------------get_slash: %c\n", get_slash());

        printf("------------------is_slash\n");
        bool ret;
        for (int i = 0; i < 128; i++)
        {
            ret = is_slash(i);
            if (ret) printf("[%c] is slash: %d\n", (_lchar)i, ret);
        }
    }

    // path related
    {
        string strsz[] =
        {
            "E:\\Mine\\Code\\lslib\\bin\\Debug",
            "E:\\Mine\\Code\\lslib\\bin\\Debug\\",
            "E:\\\\Mine\\\\Code\\\\lslib\\\\bin\\\\Debug\\\\",
            "E:/Mine/Code/lslib/bin/Debug",
            "E:/Mine/Code/lslib/bin/Debug/",
            "E:////Mine////Code////lslib////bin////Debug////",
            "E:////Mine////Code\\\\lslib////bin\\\\Debug////\\\\",
            "E:////Mine////Code\\\\lslib////bin\\\\test////\\\\",
            ".",
            "./",
            ".\\",
            ".////",
            ".\\\\",
            ".////\\\\",
            "..",
            "../Debug\\",
            "test.exe",
            "..//\\//Debug//\\test.exe",
            "E:\\Mine\\Code\\lslib\\bin\\Debug\\test.exe",
            "E:\\Mine\\Code\\lslib\\bin\\Debug\\test//test.exe",
            "c://windows/notepad.exe",
            "C:\\//Program Files\\//WinRAR//\\WinRAR.exe",
            "C:\\//Program Files\\//WinRAR//\\License.txt",
        };

        printf("------------------path related\n");
        for (int i = 0; i < sizeof(strsz) / sizeof(string); i++)
        {
            const string& str = strsz[i];
            printf("%s\n", str.c_str());
            printf("\tpath_get_dir       : %s\n", path_get_dir(str.c_str()).c_str());
            printf("\tpath_get_name      : %s\n", path_get_name(str.c_str()).c_str());
            printf("\tpath_get_filename  : %s\n", path_get_filename(str.c_str()).c_str());
            printf("\tpath_get_ext       : %s\n", path_get_ext(str.c_str()).c_str());
            printf("\tpath_pretty        : %s\n", path_pretty(str.c_str()).c_str());
            printf("\tpath_make_absolute : %s\n", path_make_absolute(str.c_str()).c_str());
            printf("\tis_exist           : %d\n", is_exist(str.c_str()));
            printf("\tis_file            : %d\n", is_file(str.c_str()));
            printf("\tis_dir             : %d\n", is_dir(str.c_str()));
            printf("\tis_absolute        : %d\n", is_absolute(str.c_str()));
            printf("\n");
        }
    }

    {
        string strsz[] =
        {
            "E:\\Mine\\Code\\lslib\\bin\\Debug",
            "a/b/c",

            "E:\\\\Mine\\\\Code\\\\lslib\\\\bin\\\\Debug\\\\",
            "/a/b/c",

            "E:/Mine/Code/lslib/bin/Debug/",
            "../../a/b/c",

            "E:////Mine////Code\\\\lslib////bin\\\\Debug////\\\\",
            "E:////a////b\\\\c////bin\\\\test////\\\\",

            "/a/b/c/",
            "./",

            "E:////Mine////Code\\\\lslib////bin\\\\Debug////\\\\",
            ".////",

            ".\\\\",
            ".////\\\\",

            "..",
            "../Debug\\",

            "test.exe",
            "..//\\//Debug//\\test.exe",

            "E:\\Mine\\Code\\lslib\\bin\\Debug\\test.exe",
            "E:\\Mine\\Code\\lslib\\bin\\Debug\\test//test.exe",
        };

        printf("------------------path_combine\n");
        for (int i = 0; i < sizeof(strsz) / sizeof(string) - 1; i += 2)
        {
            const string& str = strsz[i];
            printf("(%s, %s) : %s\n", strsz[i].c_str(), strsz[i+1].c_str(), path_combine(strsz[i].c_str(), strsz[i+1].c_str()).c_str());
        }
    }
}

void file_test()
{
    // test for
    //     LSLIB_API const int copy(_lpcstr path, _lpcstr target);
    //     LSLIB_API const int move(_lpcstr path, _lpcstr target);
    //     LSLIB_API const bool rename(_lpcstr path, _lpcstr target);
    //     LSLIB_API const int mkdir(_lpcstr path);
    //     LSLIB_API const int rm(_lpcstr path);
    //     LSLIB_API void enumerate_files(__out__ enum_file_array& array_files, _lpcstr path, _lpcstr extention, _lpcstr filter, bool recurse/* = false*/);

    //////////////////////////////////////////////////////////////////////////
    // copy
    {
        printf("------------------cp\n");

        string strsz[] =
        {
            "C:\\Program Files\\WinRAR",
            "testfolder\\",

            "C:\\Program Files\\WinRAR\\",
            "testfolder\\Pictures\\",

            "C:\\Program Files\\WinRAR\\",
            "testfolder\\Pictures-cp",

            "c://windows/notepad.exe",
            "testfolder\\",

            "c://windows/notepad.exe",
            "testfolder\\notepad-cp.exe",
        };

        for (int i = 0; i < sizeof(strsz) / sizeof(string) - 1; i += 2)
        {
            const string& source = strsz[i];
            const string& target = strsz[i + 1];
            printf("copy: %s -> %s === [%d]\n", source.c_str(), target.c_str(), copy(source.c_str(), target.c_str()));
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // enumerate_files
    {
        string str = "testfolder";
        printf("------------------enumerate_files(%s)\n", str.c_str());

        enum_file_array arr;
        os::enumerate_files(arr, str.c_str(), NULL, NULL, true);
        for (size_t i = 0; i < arr.size(); i++)
        {
            const enum_file& f = arr[i];
            printf("file info: %s\n\t %s\n\t %s\n\t %s\n\t %s\n\t %d\n", f.fullPath.c_str(), f.filePath.c_str(), f.fileName.c_str(), f.name.c_str(), f.extName.c_str(), f.size);
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // move
    {
        printf("------------------move\n");

        string strsz[] =
        {
            "testfolder\\Pictures\\",
            "testfolder\\Pictures-move\\",

            "testfolder\\Pictures-cp",
            "testfolder\\Pictures-cp-move",

            "testfolder\\notepad.exe",
            "testfolder\\notepad-move.exe",

            "testfolder\\notepad-cp.exe",
            "testfolder\\notepad-cp-move.exe",

            "testfolder\\Pictures-move\\",
            "d:/testfolder\\",

            "d:/testfolder\\",
            "/testfolder\\Pictures-move\\",
        };

        for (int i = 0; i < sizeof(strsz) / sizeof(string) - 1; i += 2)
        {
            const string& source = strsz[i];
            const string& target = strsz[i + 1];
            printf("move: %s -> %s === [%d]\n", source.c_str(), target.c_str(), move(source.c_str(), target.c_str()));
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // rename
    {
        printf("------------------rename\n");

        string strsz[] =
        {
            "testfolder\\Pictures-cp-move",
            "testfolder\\Pictures-cp-rename",

            "testfolder\\notepad-move.exe",
            "testfolder\\notepad-rename.exe",

            "testfolder\\notepad-cp-move.exe",
            "testfolder\\notepad-cp-rename.exe",

            "/testfolder\\Pictures-move\\",
            "/testfolder\\Pictures-move-rename\\",

            "testfolder\\notepad-cp-rename.exe",
            "/testfolder\\notepad-rename.exe",
        };

        for (int i = 0; i < sizeof(strsz) / sizeof(string) - 1; i += 2)
        {
            const string& source = strsz[i];
            const string& target = strsz[i + 1];
            printf("rename: %s -> %s === [%d]\n", source.c_str(), target.c_str(), os::rename(source.c_str(), target.c_str()));
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // rm
    {
        printf("------------------rm\n");

        string strsz[] =
        {
            "testfolder\\notepad.exe",
            "testfolder\\notepad-cp.exe",
            "testfolder\\notepad-rename",
            "testfolder\\Pictures\\",
            "testfolder\\Pictures-cp",
            "testfolder\\Pictures-cp-rename",
            "testfolder",
            "/testfolder"
        };

        for (int i = 0; i < sizeof(strsz) / sizeof(string); i++)
        {
            const string& str = strsz[i];
            printf("rm: %s === [%d]\n", str.c_str(), rm(str.c_str()));
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // mkdir
    {
        printf("------------------mkdir\n");

        string strsz[] =
        {
            "testfolder/",
            "testfolder\\subfolder\\",
            "testfolder\\a\\b\\c",
            "testfolder\\a",
            "testfolder",
            "/a/b/c",
            "c:/a/b/c",
            "/a",
            "c:/a",
            "/testfolder"
        };

        for (int i = 0; i < sizeof(strsz) / sizeof(string); i++)
        {
            const string& str = strsz[i];
            printf("mkdir: %s === [%d]\n", str.c_str(), os::mkdir(str.c_str()));
        }

        printf("------------------rm\n");
        for (int i = 0; i < sizeof(strsz) / sizeof(string); i++)
        {
            const string& str = strsz[i];
            printf("rm: %s === [%d]\n", str.c_str(), rm(str.c_str()));
        }
    }
}

void system_path_test()
{
    printf("------------------system related path\n");
    printf("get_module_file_path  : %s\n", get_module_file_path().c_str());
    printf("get_module_path       : %s\n", get_module_path().c_str());
    printf("get_module_name       : %s\n", get_module_name().c_str());

#ifdef _MSC_VER
    printf("get_special_folder_path(CSIDL_COMMON_APPDATA) : %s\n", get_special_folder_path(0x0023).c_str());
    printf("get_app_data_path                             : %s\n", get_app_data_path().c_str());
    printf("get_module_app_data_path                      : %s\n", get_module_app_data_path(false).c_str());
    printf("get_desktop_path       : %s\n", get_desktop_path().c_str());
    printf("get_quick_launch_path  : %s\n", get_quick_launch_path().c_str());
    printf("get_program_files_path : %s\n", get_program_files_path().c_str());
#endif
}

void shell_test()
{
#ifdef _MSC_VER
    printf("------------------shell related\n");

    string str = "C:\\//Program Files\\//WinRAR//\\";
    open_dir(str.c_str(), NULL);
    open_dir(str.c_str(), "WinRAR.exe");

    string_array arr_files;
    open_file_select_dialog(arr_files, "请选择文件",
        "常见图片格式\0*.jpg;*.jpeg;*.png;*.bmp\0"
        " JPG图片(*.jpg)\0*.jpg;*.jpeg\0"
        " PNG图片(*.png)\0*.png\0"
        " BMP图片(*.bmp)\0*.bmp\0"
        " 所有文件(*.*)\0*.*\0", true, NULL);
    for (size_t i = 0; i < arr_files.size(); i++)
    {
        cout << "open_file_select_dialog[" << i << "]: " << arr_files[i].c_str() << endl;
    }

    open_folder_select_dialog(str, NULL, NULL);
    cout << "open_folder_select_dialog: " << str.c_str() << endl;
#endif
}

void other_os_test()
{
#ifdef _MSC_VER
    {
        printf("------------------other functions\n");

        os_type ot = get_os_type();
        printf("get_os_type(): %d\n", ot);

        string str = enum_str(os_type, ot);
        printf("enum_str(os_type, %d): %s\n", ot, str.c_str());

        ot = enum_from_str(os_type, str.c_str());
        printf("enum_from_str(os_type, %s): %d\n", str.c_str(), ot);
    }
    {
        string strsz[] =
        {
            "E:\\Mine\\Code\\lslib\\bin\\Debug",
            "E:\\Mine\\Code\\lslib\\bin\\Debug\\",
            "E:\\\\Mine\\\\Code\\\\lslib\\\\bin\\\\Debug\\\\",
            "E:/Mine/Code/lslib/bin/Debug",
            "E:/Mine/Code/lslib/bin/Debug/",
            "E:////Mine////Code////lslib////bin////Debug////",
            "E:////Mine////Code\\\\lslib////bin\\\\Debug////\\\\",
            "E:////Mine////Code\\\\lslib////bin\\\\test////\\\\",
            ".",
            "./",
            ".\\",
            ".////",
            ".\\\\",
            ".////\\\\",
            "..",
            "../Debug\\",
            "test.exe",
            "..//\\//Debug//\\test.exe",
            "E:\\Mine\\Code\\lslib\\bin\\Debug\\test.exe",
            "E:\\Mine\\Code\\lslib\\bin\\Debug\\test//test.exe",
            "c://windows/notepad.exe",
            "C:\\//Program Files\\//WinRAR//\\WinRAR.exe",
            "C:\\//Program Files\\//WinRAR//\\License.txt",
        };

        printf("get_product_version\n");
        for (int i = 0; i < sizeof(strsz) / sizeof(string); i++)
        {
            const string& str = strsz[i];
            printf("\t %s === %s\n", str.c_str(), get_product_version(str.c_str()).c_str());
        }
    }
#endif
}

void test_os()
{
    path_test();
    file_test();
    system_path_test();
    shell_test();
    other_os_test();
}
declare_test_case(test_os);
