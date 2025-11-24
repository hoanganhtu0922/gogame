#include "table.h"
void table::run() {
    Image img = LoadImage("../../assets/image/blackWin.png");
    ImageFormat(&img, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    blackWin = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImage("../../assets/image/whiteWin.png");
    ImageFormat(&img, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    whiteWin = LoadTextureFromImage(img);
    UnloadImage(img);
    BACK.initialize((float)(screenSize - 519.0f * 0.35f) / 2, 550.f, 0, "EXIT");
   
}

void table::drawTable(int black) {
    if (black) {
        DrawTextureEx(blackWin, {(float)(screenSize / 2 - blackWin.width * 0.5f), (float)(screenSize / 2 - blackWin.height * 0.5f)}, 0.0f, 1.0f, WHITE);
    } else {
        DrawTextureEx(whiteWin, {(float)(screenSize / 2 - whiteWin.width * 0.5f), (float)(screenSize / 2 - whiteWin.height * 0.5f)}, 0.0f, 1.0f, WHITE);    
    }

    
    BACK.DrawButton();

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && BACK.IsHover()) {
        exit = 1;
    }
}