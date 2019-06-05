#include "testcase.hpp"
using namespace lslib::os;

void test_os()
{
    cout << "get_slash: " << get_slash() << endl;
    for (int i = 0; i < 128; i++)
    {
        if (is_slash(i))
            cout << "[" <<  i << "]" << (_lchar)i << " is_slash: " << is_slash(i) << endl;
    }
    cout << "------------------" << endl;

    lstring strsz[] =
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
    for (int i = 0; i < sizeof(strsz) / sizeof(lstring); i++)
    {
        const lstring& str = strsz[i];
        cout << "path_get_dir(\"" << str << "\"): " << path_get_dir(str).c_str() << endl;
        cout << "path_get_name(\"" << str << "\"): " << path_get_name(str).c_str() << endl;
        cout << "path_get_filename(\"" << str << "\"): " << path_get_filename(str).c_str() << endl;
        cout << "path_get_ext(\"" << str << "\"): " << path_get_ext(str).c_str() << endl;
        cout << "path_pretty(\"" << str << "\"): " << path_pretty(str).c_str() << endl;
        cout << "path_make_absolute(\"" << str << "\"): " << path_make_absolute(str).c_str() << endl;
        cout << "is_exist(\"" << str << "\"): " << is_exist(str) << endl;
        cout << "is_file(\"" << str << "\"): " << is_file(str) << endl;
        cout << "is_dir(\"" << str << "\"): " << is_dir(str) << endl;
        cout << "------------------" << endl;
    }

    lstring str1 = "testfolder";
    lstring str2 = str1 + "/subfolder/subfolder/";
    lstring str3 = "c://windows/notepad.exe";
    lstring str4 = "C:/Users/Public/Pictures/Sample Pictures/\\";

    cout << "path_combine(\"" << str4.c_str() << "\", \"" << str1.c_str()  << "\"): " << os::path_combine(str4, str1) << endl;
    cout << "path_combine(\"" << str4.c_str() << "\", \"" << str2.c_str()  << "\"): " << os::path_combine(str4, str2) << endl;
    cout << "path_combine(\"" << str4.c_str() << "\", \"" << "..\\//..\\combine"  << "\"): " << os::path_combine(str4, "..\\//..\\combine") << endl;
    cout << "path_combine(\"" << str4.c_str() << "\", \"" << "\\..\\..\\combine"  << "\"): " << os::path_combine(str4, "\\..\\..\\combine") << endl;
    cout << "path_combine(\"" << str4.c_str() << "\", \"" << str3.c_str()  << "\"): " << os::path_combine(str4, str3) << endl;
    cout << "------------------" << endl;

#if 0
    cout << "mkdir(\"" << str1.c_str() << "\"): " << mkdir(str1) << endl;
    cout << "mkdir(\"" << str2.c_str() << "\"): " << mkdir(str2) << endl;
    cout << "copy(\"" << str3.c_str() << "\", \"" << str1  << "\"): " << os::copy(str3, str1) << endl;
    cout << "copy(\"" << str3.c_str() << "\", \"" << str1 + "/notepad_copy.exe"  << "\"): " << os::copy(str3, str1 + "/notepad_copy.exe") << endl;
    cout << "copy(\"" << str4.c_str() << "\", \"" << str1  << "\"): " << os::copy(str4, str1) << endl;
    cout << "copy(\"" << str4.c_str() << "\", \"" << str1 + "/Sample Pictures Copy"  << "\"): " << os::copy(str4, str1 + "/Sample Pictures Copy") << endl;
    cout << "move(\"" << str1 + "/notepad_copy.exe" << "\", \"" << str2  << "\"): " << os::move(str1 + "/notepad_copy.exe", str2) << endl;
    cout << "move(\"" << str1 + "/Sample Pictures Copy" << "\", \"" << str2  << "\"): " << os::move(str1 + "/Sample Pictures Copy", str2) << endl;
    cout << "rename(\"" << str2 + "/notepad_copy.exe" << "\", \"" << str2 + "/notepad_renamed.exe"  << "\"): " << os::rename(str2 + "/notepad_copy.exe", str2 + "/notepad_renamed.exe") << endl;
    cout << "rename(\"" << str2 + "/Sample Pictures Copy" << "\", \"" << str2 + "/Sample Pictures Renamed"  << "\"): " << os::rename(str2 + "/Sample Pictures Copy",
            str2 + "/Sample Pictures Renamed") << endl;
    cout << "rename(\"" << str2 + "/notepad_renamed.exe" << "\", \"" << str1 + "/notepad_renamed.exe"  << "\"): " << os::rename(str2 + "/notepad_renamed.exe", str1 + "/notepad_renamed.exe") << endl;
    cout << "rm(\"" << str2.c_str() << "\"): " << rm(str2) << endl;
    cout << "rm(\"" << str1.c_str() << "\"): " << rm(str1) << endl;
    cout << "------------------" << endl;
#endif

    cout << "get_module_file_path(): " << get_module_file_path() << endl;
    cout << "get_module_path(): " << get_module_path() << endl;
    cout << "get_module_name(): " << get_module_name() << endl;
    cout << "get_special_folder_path(CSIDL_COMMON_APPDATA): " << get_special_folder_path(0x0023) << endl;
    cout << "get_app_data_path(): " << get_app_data_path() << endl;
    cout << "get_module_app_data_path(false): " << get_module_app_data_path(false) << endl;
    cout << "get_desktop_path(): " << get_desktop_path() << endl;
    cout << "get_quick_launch_path(): " << get_quick_launch_path() << endl;
    cout << "get_program_files_path(): " << get_program_files_path() << endl;
    cout << "------------------" << endl;

#if 0
    lstring str5 = "C:\\//Program Files\\//WinRAR//\\";
    open_dir(str5, NULL);
    open_dir(str5, "WinRAR.exe");

    lstring_array arr_files;
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

    open_folder_select_dialog(str5, NULL, NULL);
    cout << "open_folder_select_dialog: " << str5.c_str() << endl;
    cout << "------------------" << endl;
#endif

    os_type ot = get_os_type();
    cout << "get_os_type(): " << ot << endl;
    str1 = enum_str(os_type, ot);
    cout << "enum_str(os_type, " << ot << "): " << str1 << endl;
    ot = enum_from_str(os_type, str1);
    cout << "enum_from_str(os_type, \"" << str1 << "\"): " << ot << endl;
    cout << "------------------" << endl;

    for (int i = 0; i < sizeof(strsz) / sizeof(lstring); i++)
    {
        const lstring& str = strsz[i];
        cout << "get_product_version(\"" << str <<  "\"): " << get_product_version(str) << endl;
    }
    cout << "------------------" << endl;
}
declare_test_case(test_os);
