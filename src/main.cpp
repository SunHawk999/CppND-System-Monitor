#include<iostream>

#include "ncurses_display.h"
#include "system.h"


int main() {
  //Object that contains relevant methods and attributes regarding system details
  //std::cout<<"main test\n";
  SysInfo system;
  //std::cout<<"test SysInfo\n";
  //Object that contains list of current processes, Container for Process Class
  ProcessContainer procs;
  //std::cout<<"test ProcessContainer\n";
  NCursesDisplay::PrintMain(system, procs);
}