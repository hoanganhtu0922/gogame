#pragma once
#include <raylib.h>
#include <vector>
#include <utility> 
#include <string>
#include "initial.h"
#include "font.h"
#include "Music.h"
#include "soundEffect.h"

class MenuButton{
public:
    MenuButton();
    
    MenuButton(float x_, float y_, int type_, std::string);
    bool IsHoverLeft();
    bool IsHoverRight();
    void SetTrian();
    void DrawRect();
    void DrawTrian();
    bool IsHover();
    int IsMouseOverTrian();
    void DrawButton();
    void DrawCenteredText(float x, float y, std::string str, Texture2D button, float texScale);

    float x, y;
    Image img;
    Texture2D button, MoveLeft, MoveRight, Banner;
    std::string label;
    int type = 0, cnt = 0, cyc = 0;
    std::vector <std::string> vers; 
    std::vector <Texture2D> Volume;
    soundEffect sound;
};
