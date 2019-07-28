#ifndef PROCESS_H
#define PROCESS_H

//#include <string>

#include "ProcessParser.h"

using namespace std;

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
    public:
        //constructor
        Process(string pid){
            this->pid = pid;
            this->user = ProcessParser::GetProcUser(pid);
            this->mem = ProcessParser::GetVmSize(pid);
            this->cmd = ProcessParser::GetCmd(pid);
            this->upTime = ProcessParser::GetProcUpTime(pid);
            this->cpu = ProcessParser::GetCpuPercent(pid);
        }

        void SetPid(string pid);
        string GetPid() const;
        string GetUser() const;
        string GetCmd() const;
        string GetUpTime() const;
        string GetProcess();
        int GetCpu() const;
        int GetMem() const;

    private:
        string pid, user, cmd, cpu, mem, upTime;
};

#endif