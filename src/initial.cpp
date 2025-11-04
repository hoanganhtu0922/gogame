#pragma once
#include "initial.h"
int gridSize = 19;
int cellSize = 30;
int margin = 80;
int screenSize = margin * 2 + (gridSize - 1) * cellSize;

void InitializeGameParameters(int size) {
    gridSize = size;
    cellSize = 30 * 19 / (gridSize + 1);
    margin   = (screenSize - cellSize * (gridSize - 1)) / 2;
    //screenSize = margin * 2 + (gridSize - 1) * cellSize;
}