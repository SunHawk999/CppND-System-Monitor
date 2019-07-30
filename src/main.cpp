#include<iostream>

#include "ncurses_display.h"
#include "system.h"


int main() {

  //Object that contains list of current processes, Container for Process Class
  ProcessContainer process;

  //Object that contains relevant methods and attributes regarding system details
  SysInfo system;
  
  NCursesDisplay::PrintMain(system, process);
}