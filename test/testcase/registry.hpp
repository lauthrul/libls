#include "testcase.hpp"
using namespace lslib::registry;

void test_registry()
{
    lstring path = "Software\\Microsoft\\Windows\\CurrentVersion\\Themes";
    lstring str;
    DWORD dw;

    str = registry_get_str(HKEY_CURRENT_USER, path, "CurrentTheme");
    cout << "registry_get_str(HKEY_CURRENT_USER, \"" << path << "\", \"CurrentTheme\") : " << str << endl;

    str = registry_get_str(HKEY_CURRENT_USER, path, "InstallTheme");
    cout << "registry_get_str(HKEY_CURRENT_USER, \"" << path << "\", \"InstallTheme\") : " << str << endl;

    dw = registry_get_dword(HKEY_CURRENT_USER, path, "ThemeChangesDesktopIcons");
    cout << "registry_get_dword(HKEY_CURRENT_USER, \"" << path << "\", \"ThemeChangesDesktopIcons\") : " << dw << endl;

    dw = registry_get_dword(HKEY_CURRENT_USER, path, "新值 #1");
    cout << "registry_get_dword(HKEY_CURRENT_USER, \"" << path << "\", \"新值 #1\") : " << dw << endl;

    dw = registry_get_dword(HKEY_CURRENT_USER, path, "新值 #2");
    cout << "registry_get_dword(HKEY_CURRENT_USER, \"" << path << "\", \"新值 #2\") : " << dw << endl;

    dw = registry_set_dword(HKEY_CURRENT_USER, path, "dword 1", 123);
    cout << "registry_set_dword(HKEY_CURRENT_USER, \"" << path << "\", \"dword 1\", 123) : " << dw << endl;

    dw = registry_set_dword(HKEY_CURRENT_USER, path, "dword 2", 0x123456);
    cout << "registry_set_dword(HKEY_CURRENT_USER, \"" << path << "\", \"dword 2\", 0x123456) : " << dw << endl;

    dw = registry_set_str(HKEY_CURRENT_USER, path, "sz 1", "abc 123 456 789");
    cout << "registry_set_str(HKEY_CURRENT_USER, \"" << path << "\", \"sz 1\", \"abc 123 456 789\") : " << dw << endl;

    dw = registry_set_str(HKEY_CURRENT_USER, path, "sz 2", "abc\r\n123\r\n456\r\n789");
    cout << "registry_set_str(HKEY_CURRENT_USER, \"" << path << "\", \"sz 2\", \"abc\r\n123\r\n456\r\n789\") : " << dw << endl;

    dw = registry_set_str(HKEY_CURRENT_USER, path + "\\test\\test", "CurrentTheme", "abc");
    cout << "registry_set_str(HKEY_CURRENT_USER, \"" << path + "\\test\\test" << "\", \"CurrentTheme\", \"abc\") : " << dw << endl;

    dw = registry_delete_value(HKEY_CURRENT_USER, path, "dword 1");
    cout << "registry_delete_value(HKEY_CURRENT_USER, \"" << path << "\", \"dword 1\") : " << dw << endl;

    dw = registry_delete_value(HKEY_CURRENT_USER, path, "dword 2");
    cout << "registry_delete_value(HKEY_CURRENT_USER, \"" << path << "\", \"dword 2\") : " << dw << endl;

    dw = registry_delete_value(HKEY_CURRENT_USER, path, "sz 1");
    cout << "registry_delete_value(HKEY_CURRENT_USER, \"" << path << "\", \"sz 1\") : " << dw << endl;

    dw = registry_delete_value(HKEY_CURRENT_USER, path, "sz 2");
    cout << "registry_delete_value(HKEY_CURRENT_USER, \"" << path << "\", \"sz 2\") : " << dw << endl;

    dw = registry_delete_value(HKEY_CURRENT_USER, path, "no exist");
    cout << "registry_delete_value(HKEY_CURRENT_USER, \"" << path << "\", \"no exist\") : " << dw << endl;

    path = "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\新项 #1";
    dw = registry_delete_key(HKEY_CURRENT_USER, path);
    cout << "registry_delete_key(HKEY_CURRENT_USER, \"" << path << ") : " << dw << endl;

    path = "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\test";
    dw = registry_delete_key(HKEY_CURRENT_USER, path);
    cout << "registry_delete_key(HKEY_CURRENT_USER, \"" << path << ") : " << dw << endl;
}
declare_test_case(test_registry);
