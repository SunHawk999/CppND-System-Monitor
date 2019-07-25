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

#include "ProcessParser.h"
#include "util.cpp"

using namespace std;

/*Retrives memory status of a specific process from proc/[PID]/status

args: pid - 'process ID' used for proc/[PID]/status

returns: A string that states the process's memory usage in kilobytes*/
string ProcessParser::GetVmSize(string pid){
    string line;
    //Declaring search attribute for file
    string name = "VmData";
    float result;

    //Opening stream for specific file
    ifstream stream = Util::GetStream(Path::basePath() + pid + Path::statusPath());
    //Searching line by line
    while(getline(stream, line)){
        if(line.compare(0, name.size(), name) == 0){
            // slicing string line on ws for values using sstream
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);
            //conversion from kB to gB
            result = (stof(values[1])/float(1024));
            break;
        }
    }
    return to_string(result);
}

/*Parse data from /proc to calculate cpu usauge of certain processes */
string ProcessParser::GetCpuPercent(string pid){
    string line;
    float result;

    //Get data from pid path, and put into a vector of strings   
    ifstream stream = Util::GetStream(Path::basePath() + pid + "/" + Path::statPath());
    getline(stream, line);
    string str = line;
    istringstream buf(str);
    istream_iterator<string> beg(buf), end;
    vector<string> values(beg, end);

    //Now get the relevant info from the vector
    float utime = stof(ProcessParser::GetProcUpTime(pid));
    float stime = stof(values[14]);
    float cutime = stof(values[15]);
    float cstime = stof(values[16]);
    float starttime = stof(values[21]);

    float uptime = ProcessParser::GetSysUpTime();

    float freq = sysconf(_SC_CLK_TCK);

    float total_time = utime + stime + cutime + cstime;
    float seconds = uptime - (starttime/freq);
    result = 100.0 * ((total_time/freq)/seconds);

    return to_string(result);
}

/*Get the systems frequency for caculating the process uptime*/
string ProcessParser::GetProcUpTime(string pid){
    string line;
    
    ifstream stream = Util::GetStream(Path::basePath() + pid + "/" + Path::statPath());
    getline(stream, line);
    string str = line;
    istringstream buf(str);
    istream_iterator<string> beg(buf), end;
    vector<string> values(beg, end);

    return to_string(float(stof(values[13]) / sysconf(_SC_CLK_TCK)));
}

/*Get system uptime from /proc */
long int ProcessParser::GetSysUpTime(){
    string line;

    //Get uptime from uptime path 
    ifstream stream = Util::GetStream(Path::basePath() + Path::upTimePath());

    getline(stream, line);
    string str = line;
    istringstream buf(str);
    istream_iterator<string> beg(buf), end;
    vector<string> values(beg, end);

    return stoi(values[0]);
}

/*Get the process user from proc/[PID]/status */
string ProcessParser::GetProcUser(string pid){
    string line;
    string name = "Uid:";
    string result = "";
    ifstream stream = Util::GetStream(Path::basePath() + pid + Path::statusPath());

    //Get the uid for the user   
    while(getline(stream, line)){
        //Look for uid tag
        if(line.compare(0, name.size(), name) == 0){
            string str = line;
            istringstream buf(str);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);
            //set found value to result
            result = values[1];
            break;
        }
    }

    //Get the name of the user with the found uid
    stream = Util::GetStream("/etc/passwd");
    name = ("x:" + result);

    while(getline(stream, line)){
        //Look for username with result
        if(line.find(name) != string::npos){
            result = line.substr(0, line.find(":"));
            return result;
        }
    }
    return "";
}

vector<string> ProcessParser::GetPidList(){

    DIR* dir;

    //Scan /proc dir for all directories with numbers as their names
    //If a found directory has numbers for their name, then store the name into a
    //a vector as a list of machine pids
    vector<string> storedPids;
    if(!(dir = opendir("/proc"))){
        throw runtime_error(strerror(errno));
    }

    while(dirent* dirp = readdir(dir)){
        //is this a directory?
        if(dirp->d_type != DT_DIR){
            continue;
        }

        //Is every character in the name a digit?
        if(all_of(dirp->d_name, dirp->d_name + strlen(dirp->d_name), [](char c) {return isdigit(c); })){
            storedPids.push_back(dirp->d_name);
        }
    }

    //Validating process of directory closing
    if(closedir(dir)){
        throw runtime_error(strerror(errno));
    }
    return storedPids;
}

//Retrieve command that executed the process
string ProcessParser::GetCmd(string pid){
    string line;
    //TODO: Look into what a cmdPath would be, could it be the cmdLine, or something else?
    ifstream stream = Util::GetStream(Path::basePath() + pid + Path::cmdPath());
    getline(stream, line);
    return line;

}

//Retrieve the number of CPU cores of the host
int ProcessParser::GetNumberofCores(){
    string line;
    string name;
    ifstream stream = Util::GetStream(Path::basePath() + "cpuinfo");
    while(getline(stream, line)){
        if(line.compare(0, name.size(), name) == 0){
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);
            return stoi(values[3]);
        }
    }
    return 0;
}

/*Read raw data from /proc/stat file, which contains info on overall cpu usage,
as well as stats for individual cores. This will be used by other functions*/
vector<string> ProcessParser::GetSysCpuPercent(string coreNumber){
    string line;
    string name = "cpu" + coreNumber;
    ifstream stream = Util::GetStream(Path::basePath() + Path::statPath());

    while(getline(stream, line)){
        if(line.compare(0, name.size(),name) == 0){
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);

            return values;
        }
    }
    return vector<string>();
}

/*Caculate active cpu time from GetSysCpuPercent,

TODO: Figure out how GetSysCpuPercent goes into this */
float ProcessParser::GetSysActiveCpuTime(vector<string> values){

    return (stof(values[S_USER]) +
            stof(values[S_NICE]) +
            stof(values[S_IRQ]) + 
            stof(values[S_SOFTIRQ]) +
            stof(values[S_STEAL]) + 
            stof(values[S_GUEST]) + 
            stof(values[S_GUEST_NICE]));
}

/*Calculate idle cpu time from GetSysCpuPercent */
float ProcessParser::GetSysIdleCpuTime(vector<string> values){
    return (stof(values[S_IDLE]) + 
            stof(values[S_IOWAIT]));
}

/*Calculates cpu usuage, either overall or for a selected core
args: previous and current time */
string ProcessParser::PrintCpuStats(vector<string> prevTime, vector<string> currTime){

    float activeTime = GetSysActiveCpuTime(currTime) - GetSysActiveCpuTime(prevTime);
    float idleTime = GetSysIdleCpuTime(currTime) - GetSysIdleCpuTime(prevTime);
    float totalTime = activeTime + idleTime;
    float result = 100.0*(activeTime / totalTime);
    return to_string(result);

}

/*Calculate RAM usuage in percentages from total and free memory, along with 
buffers */
float ProcessParser::GetSysRamPercent(){
    string line;
    string name1 = "MemAvailable:";
    string name2 = "MemFree:";
    string name3 = "Buffers:";
    string value;
    int result;

    ifstream stream = Util::GetStream(Path::basePath() + Path::memInfoPath());
    float total_mem = 0;
    float free_mem = 0;
    float buffers = 0;

    while(getline(stream, line)){
        if(total_mem != 0 && free_mem != 0)
            break;

        if(line.compare(0, name1.size(), name1) == 0){
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);
            total_mem = stof(values[1]);
        }

        if(line.compare(0, name2.size(), name2) == 0){
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);
            free_mem = stof(values[1]);
        }

        if(line.compare(0, name3.size(), name3) == 0){
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);
            buffers = stof(values[1]);
        }
    }
    //Calculate usuage
    return float(100.0 * (1-(free_mem / (total_mem - buffers))));
}

/*Get data on kernel version */
string ProcessParser::GetSysKernelVersion(){
    string line;
    string name = "Linux Version:";

    ifstream stream = Util::GetStream(Path::basePath() + Path::versionPath());
    while(getline(stream, line)){
        if(line.compare(0, name.size(), name) == 0){
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values;
            return values[2];
        }
    }
    return "";
}

/*Find the name of the operating system */
string ProcessParser::GetOsName(){
    string line;
    string name = "PRETTY_NAME=";

    ifstream stream = Util::GetStream("/etc/os-release");
    while(getline(stream, line)){
        if(line.compare(0, name.size(), name) == 0){
            size_t found = line.find("=");
            found++;
            string result = line.substr(found);
            result.erase(remove(result.begin(), result.end(), '"'), result.end());
            return result;
        }
    }
    return "";
}

/*Calculate total thread count, rather than reading it from a 
single file */
int ProcessParser::GetTotalThreads(){
    string line;
    int result = 0;
    string name = "Threads:";
    vector<string>_list = ProcessParser::GetPidList();

    for(int i = 0; i<_list.size(); i++){
        string pid = _list[i];
        //Getting every process and reading their number of threads
        ifstream stream = Util::GetStream(Path::basePath() + pid + Path::statusPath());
        while(getline(stream, line)){
            if(line.compare(0, name.size(), name) == 0){
                istringstream buf(line);
                istream_iterator<string> beg(buf), end;
                vector<string> values(beg, end);
                result += stoi(values[1]);
                break;
            }
        }
    }
    return result;
}

int ProcessParser::GetTotalNumberofProcesses(){
    string line;
    int result = 0;
    string name = "processes";
    ifstream stream = Util::GetStream(Path::basePath() + Path::statPath());
    while(getline(stream, line)){
        if(line.compare(0, line.size(), name) == 0){
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);
            result += stoi(values[1]);
            break;
        }
    }
    return result;
}

int ProcessParser::GetTotalNumberofRunningProcesses(){
    string line;
    int result = 0;
    string name = "procs_running";
    ifstream stream = Util::GetStream(Path::basePath() + Path::statPath());
    while(getline(stream, line)){
        if(line.compare(0, line.size(), name) == 0){
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);
            result += stoi(values[1]);
            break;
        }
    }
    return result;
}