#pragma once

#include <algorithm>
#include <iostream>
#include <math.h>
#include <thread>
#include <chrono>
#include <iterator>
#include <string>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <dirent.h>
#include <time.h>
#include <unistd.h>
#include <constants.h>

using namespace std;

class ProcessParser{

    public:
        static string GetCmd(string pid);
        static vector<string> GetPidList();
        static string GetVmSize(string pid);
        static string GetCpuPercent(string pid);
        static long int GetSysUpTime();
        static string GetProcUpTime(string pid);
        static string GetProcUser(string pid);
        static vector<string> GetSysCpuPercent(string coreNumber = "");
        static float GetSysRamPercent();
        static string GetSysKernelVersion();
        static int GetNumberofCores();
        static int GetTotalThreads();
        static int GetTotalNumberofRunningProcesses();
        static int GetTotalNumberofProcesses();
        static string GetOsName();
        static float GetSysActiveCpuTime(vector<string> values);
        static float GetSysIdleCpuTime(vector<string> values);
        static string PrintCpuStats(vector<string> values1, vector<string> values2);

};