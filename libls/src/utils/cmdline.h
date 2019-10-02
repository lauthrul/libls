#ifndef __CMDLINE_H__
#define __CMDLINE_H__
#pragma once

namespace lslib
{
    namespace utils
    {
        //////////////////////////////////////////////////////////////////////////
        // common cmd line
#define CMD_PLUGIN_MODE         "--plugin"          // --plugin <hwnd>                  ;plugin mode. called by other progress with HWND <hwnd>
#define CMD_SILENT_MODE         "--silent"          // --silent                         ;silent mode. no message show up
#define CMD_LAUNCHER            "--lauch"           // --lauch "<program[,param][,delay]>"      ;lauch a program with param (if given) after delay time(ms, if given)

        // betclient cmd line
#define CMD_GET_AGENT           "--get_agent"       // --get_agent                      ;get current setting agentid
#define CMD_SET_AGENT           "--set_agent"       // --set_agent <agentid>            ;for agent to set agentid
#define CMD_UPDATE_CFG          "--update_cfg"      // --update_cfg [cfgfile]           ;for update.exe to update config.dat with specificed cfgfile (if given)
#define CMD_CLEAN_PWD           "--clean_pwd"       // --clean_pwd                      ;clean password cache

        // update cmd line
#define CMD_RESTART             "--restart"         // --restart                        ;restart program
#define CMD_CLEANUP             "--cleanup"         // --cleanup                        ;clean up after update

        //////////////////////////////////////////////////////////////////////////
        enum ERuntine
        {
            RUNTINE_ERORR = -1,
            RUNTINE_CONTINUE,
            RUNTINE_FINISH,
        };

        //////////////////////////////////////////////////////////////////////////
        class LSLIB_API ICmdRuntine
        {
        public:
            static int ParserCmdLine(lpcstr cmd, __out__ string_array& outarr);
            virtual ERuntine RunCmdLine(const string_array& arr) = 0;
        };

    } // utils

}// lslib

#endif
