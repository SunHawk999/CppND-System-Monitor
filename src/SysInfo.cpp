#include<vector>
#include<string>

#include "SysInfo.h"
#include "ProcessParser.h"

using namespace std;

//Wrappers around ProcessParser methods
string SysInfo::GetCpuPercent() const{
    return this->cpuPercent;
}

string SysInfo::GetMemPercent() const{
    return to_string(this->memPercent);
}

long SysInfo::GetUpTime() const{
    return this->upTime;
}

string SysInfo::GetKernelVersion() const{
    return this->kernelVer;
}

string SysInfo::GetTotalProc() const{
    return to_string(this->totalProc);
}

string SysInfo::GetRunningProc() const{
    return to_string(this->runningProc);
}

string SysInfo::GetThreads() const{
    return to_string(this->threads);
}

string SysInfo::GetOsName() const{
    return this->osName;
}

void SysInfo::SetLastCpuMeasures(){
    this->lastCpuStats = ProcessParser::GetSysCpuPercent();
}

/*Initilize attributes of the SysInfo class */
void SysInfo::GetOtherCores(int _size){
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