#include "testcase.hpp"
using namespace lslib::process;

void test_process()
{
    {
        string sz[] =
        {
            "System",
            "smss.exe",
            "svchost.exe",
            "YNoteCefRender.exe",
            "spotify.exe",
            "everything.exe",
            "TGitCache.exe",
            "TortoiseGitProc.EXE",
            "Telegram.exe",
            "explorer.exe",
        };
        for (size_t i = 0; i < sizeof(sz) / sizeof(string); i++)
        {
            const string& str = sz[i];
            cout << "get_process_id_by_name(\"" << str << "\") : " << get_process_id_by_name(str.c_str()) << endl;
            cout << "get_process_path_by_name(\"" << str << "\") : " << get_process_path_by_name(str.c_str()) << endl;
        }
    }

    {
        int sz[] = {
            11760,
            10612,
        };
        for (size_t i = 0; i < sizeof(sz) / sizeof(int); i++)
        {
            int pid = sz[i];
            cout << "get_process_path_by_id(" << pid << ") : " << get_process_path_by_id(pid) << endl;
        }
    }

    if (0)
    {
        string sz[] =
        {
            "explorer.exe",
            "notepad.exe",
            "notepad++.exe",
        };
        for (size_t i = 0; i < sizeof(sz) / sizeof(string); i++)
        {
            const string& str = sz[i];
            cout << "kill_process_by_name(\"" << str << "\") : " << kill_process_by_name(str.c_str()) << endl;
        }
    }
}
declare_test_case(test_process);
