#pragma once

#include <string>
#include <fstream>
#include <iostream>

using namespace std;

//Classic helper functions
class Util {
    public:
        static string convertToTime(long int input_seconds);
        static string getProgressBar(string percent);
        static ifstream getStream(string path);
};