#include <unistd.h>
#include <string>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

//#include "process.h"
//#include "processor.h"
#include "system.h"

#include "ProcessParser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
//Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }

// Return the system's kernel identifier (string)
std::string System::Kernel() { return ProcessParser::GetSysKernelVersion(); }

// Return the system's memory utilization
float System::MemoryUtilization() { return ProcessParser::GetSysRamPercent(); }

// Return the operating system name
std::string System::OperatingSystem() { return ProcessParser::GetOsName(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return ProcessParser::GetTotalNumberofRunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return ProcessParser::GetTotalNumberofProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return ProcessParser::GetSysUpTime(); }