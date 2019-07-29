#include<vector>
#include<string>
#include<iostream>

#include "SysInfo.h"
#include "ProcessParser.h"
#include "util.h"

using namespace std;

//Wrappers around ProcessParser methods
string SysInfo::GetCpuPercent() const{
    //cout<<"Test GetCpuPercent()\n";
    return this->cpuPercent;
}

string SysInfo::GetMemPercent() const{
    //cout<<"Test GetMemPercent()\n";
    return to_string(this->memPercent);
}

long SysInfo::GetUpTime() const{
    //cout<<"Test GetUpTime()\n";
    return this->upTime;
}

string SysInfo::GetKernelVersion() const{
    //cout<<"Test GetKernelVersion\n";
    return this->kernelVer;
}

string SysInfo::GetTotalProc() const{
    //cout<<"Test GetTotalProc\n";
    return to_string(this->totalProc);
}

string SysInfo::GetRunningProc() const{
    //cout<<"Test GetRunningProc\n";
    return to_string(this->runningProc);
}

string SysInfo::GetThreads() const{
    //cout<<"Test GetThreads\n";
    return to_string(this->threads);
}

string SysInfo::GetOsName() const{
    //cout<<"Test GetOsName\n";
    return this->osName;
}

void SysInfo::SetLastCpuMeasures(){
    //cout<<"Test SetLastCpuMeasures\n";
    this->lastCpuStats = ProcessParser::GetSysCpuPercent();
}

/*Initilize attributes of the SysInfo class */
void SysInfo::GetOtherCores(int _size){
    //cout<<"Test GetOthercores\n";
    //When number of cores is detected, vectors are modified to fit the incoming data
    this->coreStats = vector<string>();
    this->coreStats.resize(_size);

    this->lastCpuCoresStats = vector<vector<string>>();
    this->lastCpuCoresStats.resize(_size);

    this->currentCpuCoresStats = vector<vector<string>>();
    this->currentCpuCoresStats.resize(_size);

    for(int i = 0; i < _size; i++){
        this->lastCpuCoresStats[i] = ProcessParser::GetSysCpuPercent(to_string(i));
    }
}

/*Updates and creates new datasets for CPU calculation */
void SysInfo::SetCpuCoresStats(){
    //Getting data from files
    //cout<<"Test GetCpuCoresStats()\n";
    for(int i = 0; i < int(this->currentCpuCoresStats.size()); i++){
        this->currentCpuCoresStats[i] = ProcessParser::GetSysCpuPercent(to_string(i));
    }
    //After acquiring data, calculate every core percentage of usuage
    for(int i = 0; i < int(this->currentCpuCoresStats.size()); i++){
        this->coreStats[i] = ProcessParser::PrintCpuStats(this->lastCpuCoresStats[i], this->currentCpuCoresStats[i]);
    }
    this->lastCpuCoresStats = this->currentCpuCoresStats;
}

/*Initialize or refresh an object */
void SysInfo::SetAttributes(){
    //Getting parsed data
    //cout<<"Test SetAttributes()\n";
    this->memPercent = ProcessParser::GetSysRamPercent();
    this->upTime = ProcessParser::GetSysUpTime();
    this->totalProc = ProcessParser::GetTotalNumberofProcesses();
    this->runningProc = ProcessParser::GetTotalNumberofRunningProcesses();
    this->threads = ProcessParser::GetTotalThreads();
    this->currentCpuStats = ProcessParser::GetSysCpuPercent();
    this->cpuPercent = ProcessParser::PrintCpuStats(this->lastCpuStats, this->currentCpuStats);
    this->lastCpuStats = this->currentCpuStats;
    this->SetCpuCoresStats();
}

/*Creates a string that represents a progress bar */
vector<string> SysInfo::GetCoresStats() const{
   // cout<<"Test GetCoresStats()\n";
    vector<string> result = vector<string>();
    for(int i = 0; i < int(this->coreStats.size()); i++){
        string temp = ("cpu" + to_string(i) + ": ");
        float check = stof(this->coreStats[i]);
        if(!check || this->coreStats[i] == "nan"){
            return vector<string>();
        }
        temp += Util::GetProgressBar(this->coreStats[i]);
        result.push_back(temp);
    }
    return result;
}