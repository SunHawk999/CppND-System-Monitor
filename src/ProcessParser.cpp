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
    string value;
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
    string value;
    float result;

    //Get data from pid path, and put into a vector of strings   
    ifstream stream = Util::GetStream(Path::basePath() + pid + Path::statPath());
    getline(stream, line);
    string str = line;
    istringstream buf(line);
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