#pragma once

#include<vector>
#include<string>

#include "ProcessParser.h"

using namespace std;

class SysInfo{
    private:
        vector<string> lastCpuStats;
        vector<string> currentCpuStats;
        vector<string> coreStats;
        vector<vector<string>> lastCpuCoresStats;
        vector<vector<string>> currentCpuCoresStats;
        string cpuPercent;
        float memPercent;
        string osName;
        string kernelVer;
        long upTime;
        int totalProc;
        int runningProc;
        int threads;

    public:
        /*Get initial info about system
        Initial data for individual cores is set
        System data is set */
        SysInfo(){
            this->GetOtherCores(ProcessParser::GetNumberofCores());
            this->SetLastCpuMeasures();
            this->SetAttributes();
            this->osName = ProcessParser::GetOsName();
            this->kernelVer = ProcessParser::GetSysKernelVersion();
        }

        void SetAttributes();
        void SetLastCpuMeasures();
        string GetMemPercent() const;
        long GetUpTime() const;
        string GetThreads() const;
        string GetTotalProc() const;
        string GetRunningProc() const;
        string GetKernelVersion() const;
        string GetOsName() const;
        string GetCpuPercent() const;
        void GetOtherCores(int _size);
        void SetCpuCoresStats();
        vector<string> GetCoresStats() const;
};