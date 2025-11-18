#pragma once 
#include <raylib.h>

class MusicBackGround{
public:
    void setinitial();
    void Run();
    Music bgm;
    int vol = 2;
};

extern MusicBackGround PlayMusicBackGround;