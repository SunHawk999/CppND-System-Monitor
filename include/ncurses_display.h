#ifndef NCURSES_DISPLAY_H
#define NCURSES_DISPLAY_H

#include "ProcessContainer.h"
#include "SysInfo.h"

namespace NCursesDisplay {
void WriteSysInfoToConsole(SysInfo system, WINDOW* sys_win);
void GetProcessListToConsole(ProcessContainer process, WINDOW* win);
void PrintMain(SysInfo system, ProcessContainer process);
std::string ProgressBar(float percent);
};  // namespace NCursesDisplay

#endif