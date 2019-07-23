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