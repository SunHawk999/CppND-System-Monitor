#pragma once

#include <string>

using namespace std;

//Need an enum class?

//Need to know where these paths lead to and where they come from

//Use these functions in order to call these strings
class Path{
    public:
        static string basePath(){
            return "/proc/";
        }

        static string commandLine(){
            return "/cmdline";
        }

        static string statusPath(){
            return "/status";
        }

        static string statPath(){
            return "stat";
        }

        static string upTimePath(){
            return "uptime";
        }

        static string memInfoPath(){
            return "meminfo";
        }

        static string versionPath(){
            return "version";
        }

};
