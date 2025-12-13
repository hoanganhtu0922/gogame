#pragma once
#define NOGDI
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define NOGDI
#define NOUSER             
#define NOSYSCOMMANDS
#define NOHELP

#include <iostream>
#include <string>
#include <vector>
#include "initial.h"
#include <windows.h>
#include "Music.h"


class KataGoController {
public:
    HANDLE hChildStd_IN_Rd = NULL;
    HANDLE hChildStd_IN_Wr = NULL;
    HANDLE hChildStd_OUT_Rd = NULL;
    HANDLE hChildStd_OUT_Wr = NULL;
    PROCESS_INFORMATION piProcInfo;
    std::string ReadResponse();
    void start();              // <-- keep only ONE
    ~KataGoController();

    void Send(std::string cmd);
    
    std::string transPoint(std::vector<int> v);
    std::pair<int,int> getAImove(std::vector<std::vector<int>> points, int b);
    std::pair<int,int> coord(std::string str);
    int to_int(std::string s);
};
