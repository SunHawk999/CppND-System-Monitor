#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

//Return this process's ID
string Process::GetPid() const {return this->pid;}

//Set this process ID
//void Process::SetPid() const {this->pid = pid;}


string Process::GetProcess(){
    this->mem = ProcessParser::GetVmSize(this->pid);
    this->upTime = ProcessParser::GetProcUpTime(this->pid);
    this->cpu = ProcessParser::GetCpuPercent(this->pid);

    return (this->pid + "   " 
                      + this->user
                      + "   "
                      + this->cpu.substr(0,5)
                      + "   "
                      + this->mem.substr(0,5)
                      + "   "
                      + this->upTime
                      + "   "
                      + this->cmd.substr(0,30)
                      + "...");
}