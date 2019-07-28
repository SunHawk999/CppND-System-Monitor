#include "ncurses_display.h"
#include "system.h"

int main() {
  //Object that contains relevant methods and attributes regarding system details
  SysInfo system;
  //Object that contains list of current processes, Container for Process Class
  ProcessContainer procs;

  NCursesDisplay::PrintMain(system, procs);
}