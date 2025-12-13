#pragma once
#include "initial.h"
int gridSize = 19;
int cellSize = 30;
int margin = 80;
int screenSize = margin * 2 + (gridSize - 1) * cellSize;
int SoundVol = 2;
int MusicVol = 2;
int vsAI = 0;
int userStone = 0;
int diff = 0;
int Itime = 10;
int typeboard = 0;
Image img;
Texture2D BackGround;

void InitializeGameParameters(int size) {
    gridSize = size;
    cellSize = 30 * 19 / (gridSize + 1);
    margin   = (screenSize - cellSize * (gridSize - 1)) / 2;
    //screenSize = margin * 2 + (gridSize - 1) * cellSize;
    img = LoadImage("../../assets/image/background.png");
    BackGround = LoadTextureFromImage(img);
    UnloadImage(img);
}