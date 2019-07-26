#include<string>
#include<vector>

#include "process.h"

using namespace std;

class ProcessContainer{
    public:
        ProcessContainer(){ this->RefreshList(); }
        void RefreshList();
        string PrintList();
        vector<string> GetList();

    private:
        vector<Process>_list;
};

