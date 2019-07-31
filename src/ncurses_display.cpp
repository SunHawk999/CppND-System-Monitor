#include <curses.h>
#include <chrono>
#include <string>
#include <thread>
#include <vector>
#include <iostream>

#include "util.h"
#include "ncurses_display.h"
#include "system.h"

using std::string;

void NCursesDisplay::WriteSysInfoToConsole(SysInfo system, WINDOW* sys_window){
    system.SetAttributes();

    mvwprintw(sys_window, 2, 2, ("OS: "+system.GetOsName()).c_str());
    mvwprintw(sys_window, 3, 2, ("Kernel_Version: "+system.GetKernelVersion()).c_str());
    mvwprintw(sys_window, 4, 2, "Cpu: ");
    wattron(sys_window, COLOR_PAIR(1));
    wprintw(sys_window, Util::GetProgressBar(system.GetCpuPercent()).c_str());
    wattroff(sys_window, COLOR_PAIR(1));
    mvwprintw(sys_window, 5, 2, "Cores: ");
    wattron(sys_window, COLOR_PAIR(1));
    vector<string> val = system.GetCoresStats();
    for(int i = 0; i < int(val.size()); i++){
        mvwprintw(sys_window, 6 + i, 2, val[i].c_str());
    }
    wattroff(sys_window, COLOR_PAIR(1));
    mvwprintw(sys_window, 10, 2, "Memory: ");
    wattron(sys_window, COLOR_PAIR(2));
    wprintw(sys_window, Util::GetProgressBar(system.GetMemPercent()).c_str());
    wattroff(sys_window, COLOR_PAIR(2));
    mvwprintw(sys_window, 12, 2, ("Total_Processes: " + system.GetTotalProc()).c_str());
    mvwprintw(sys_window, 13, 2, ("Running_Processes: " + system.GetRunningProc()).c_str());
    mvwprintw(sys_window, 14, 2, ("Up_Time: " + Util::ConvertToTime(system.GetUpTime())).c_str());
}

void NCursesDisplay::GetProcessListToConsole(ProcessContainer process, WINDOW* win){
    process.RefreshList();
    wattron(win, COLOR_PAIR(2));
    mvwprintw(win, 1, 2, "PID:");
    mvwprintw(win, 1, 12, "User:");
    mvwprintw(win, 1, 20, "Cpu[%]");
    mvwprintw(win, 1, 32, "Ram[mB]");
    mvwprintw(win, 1, 40, "Uptime:");
    mvwprintw(win, 1, 50, "Cmd:");
    wattroff(win, COLOR_PAIR(2));
    vector<std::string> processes = process.GetList();
    for(int i = 0; i < 10; i++){
      mvwprintw(win, 2 + i, 2, processes[i].c_str());
    }
}

void NCursesDisplay::PrintMain(SysInfo system, ProcessContainer process){
  initscr(); //start curses mode
  noecho();   //not printing input values
  cbreak();   //terminating on classic ctrl+c
  start_color(); //enable colorchange of text
  int yMax, xMax;
  getmaxyx(stdscr, yMax, xMax);   //Get size of windows measured in lines
  WINDOW *sys_win = newwin(17, xMax-1, 0, 0);
  WINDOW *proc_win = newwin(15, xMax-1, yMax-(ProcessParser::GetNumberofCores())-29, 0);

  init_pair(1, COLOR_BLUE, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);

  while(true){
    
    box(sys_win, 0, 0);
    box(proc_win, 0, 0);
    NCursesDisplay::WriteSysInfoToConsole(system, sys_win);
    NCursesDisplay::GetProcessListToConsole(process, proc_win);
    wrefresh(sys_win);
    wrefresh(proc_win);
    refresh();
    sleep(1);
  }
  endwin();
}