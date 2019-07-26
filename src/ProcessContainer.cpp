#include <string>
#include <vector>

#include "ProcessContainer.h"

using namespace std;

void ProcessContainer::RefreshList(){
    vector<string> pids = ProcessParser::GetPidList();
    this->_list.clear();
    for (auto pid : pids){
        Process proc(pid);
        this->_list.push_back(proc);
    }
}

string ProcessContainer::PrintList(){
    string result = "";
    //_lists?
    for(auto i : _list){
        result += i.GetProcess();
    }
    return result;
}

vector<string> ProcessContainer::GetList(){
    vector<string> values;
    //Fetch 10 processes
    for(int i = (this->_list.size()-10); i < this->_list.size(); i++){
        values.push_back(this->_list[i].GetProcess());
    }
    return values;
}