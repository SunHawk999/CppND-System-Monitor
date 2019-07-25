#pragma once

#include <string>
#include <fstream>
#include <iostream>

using namespace std;

//Classic helper functions
class Util {
    public:
        static string ConvertToTime(long int input_seconds);
        static string GetProgressBar(string percent);
        static ifstream GetStream(string path);
};