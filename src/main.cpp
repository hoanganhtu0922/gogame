#include <raylib.h>
#include "Board.h"
#include "IMG.h"
#include "soundEffect.h"
#include "initial.h"
#include "Menu.h"
#include "LoadGame.cpp"
#include <iostream>
#include <vector>
#include <queue>

int main() {
    InitAudioDevice();
    // Make window resizable and set a sensible minimum size
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenSize, screenSize, "Go Board");
    SetWindowMinSize(screenSize / 2, screenSize / 2);

    Menu menu;
    MenuChoice choice = menu.Run();

    if (choice == MenuChoice::Start1v1 || choice == MenuChoice::Continue) {
        Board gameBoard;
        if (choice == MenuChoice::Continue)
            gameBoard = LoadGame("SaveGame.txt");
            
        gameBoard.LoopGame();
    } else {
        CloseAudioDevice();
        CloseWindow();
    }

    return 0;
}
