#pragma once
#include <raylib.h>
#include "initial.h"
#include "Button.h"

class table{ 
public:
    void run();
    void drawTable(int black);
    Texture2D blackWin, whiteWin;
    int exit = 0;
};