#include <string>
#include <fstream>
#include <iostream>

#include "util.h"

using namespace std;

/* input: Seonds input, most likely unix time for input

    return a string of a more readable time stamp
*/
string Util::ConvertToTime(long int input_seconds){

    long minutes = input_seconds / 60;
    long hours = minutes / 60;
    long seconds = int(input_seconds % 60);
    minutes = int(minutes % 60);
    string result = to_string(hours) + ":" + to_string(minutes) + ":" + to_string(seconds);

    return result;
}

/*Construct string for given percentage of the progress bar
50 bars are uniformely stretched 0 - 100% 
meaning: every 2% is 1 bar. */
string Util::GetProgressBar(string percent){
    string result = "0% ";
    int _size = 50;
    int boundaries = (stof(percent)/100) * _size;

    for (int i=0; i<_size; i++){
        if(i<=boundaries){
            result += "|";
        }
        else{
            result += " ";
        }
    }

    result += " " + percent.substr(0,5) + " /100%";
    return result;
}

/*Wrapper for creating streams, used for error handeling */
ifstream Util::GetStream(string path){
    ifstream stream(path);
    if(!stream){
        throw runtime_error("Non - existing PID for " + path + "!");
    }

    return stream;
}
