#include<iostream>

#include "ncurses_display.h"
#include "system.h"


int main() {
  //Object that contains relevant methods and attributes regarding system details
  //std::cout<<"main test\n";
  
  //Object that contains list of current processes, Container for Process Class
  ProcessContainer process;
  //std::cout<<"test ProcessContainer\n";
  
  SysInfo system;
  //std::cout<<"test SysInfo\n";
  
  NCursesDisplay::PrintMain(system, process);
}