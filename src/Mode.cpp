#pragma once
//#include "Menu.h"
#include "Mode.h"

static void DrawCenteredText(const char* text, float fontSize, int y, Color color) {
    Vector2 sz = MeasureTextEx(uiFont, text, fontSize, 1.0f);
    DrawTextEx(uiFont, text, {(float)(screenSize - sz.x)/2.0f, (float)y}, fontSize, 1.0f, color);
}

void Mode::Run() {
    SetTargetFPS(60);
    EnsureUIFont();

    // Render to a fixed virtual square and scale to window
    RenderTexture2D target = LoadRenderTexture(screenSize, screenSize);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    //Button on main menu 
    float XMiddle = (float)(screenSize  - 519.0f * 0.35f) / 2;
    MenuButton Type(XMiddle, 100.f, 1, "1 VS 1");
    MenuButton Board(XMiddle, 200.f, 1, "19 x 19");
    MenuButton Diff(XMiddle, 400.f, 1, "EASY");
    MenuButton Timer(XMiddle, 300.f, 1, "10 MINUTES");
    MenuButton Stone(XMiddle, 500.f, 1, "BLACK");
    MenuButton Back(XMiddle, 400.f, 0, "BACK");

    Type.vers = {"1 VS 1", "VS AI"};
    Board.vers = {"19 x 19", "9 x 9", "13 x 13"};
    Diff.vers = {"EASY", "MEDIUM", "HARD"};
    Timer.vers = {"10 MINUTES", "20 MINUTES", "30 MINUTES", "40 MINUTES"};
    Stone.vers = {"BLACK", "WHITE"};
    Type.cyc = 2, Board.cyc = 3, Diff.cyc = 3, Timer.cyc = 4, Stone.cyc = 2;
    Type.cnt = vsAI;
    Stone.cnt = userStone;
    
    if (gridSize == 19) {
        Board.cnt = 0;
    } else if (gridSize == 9) {
        Board.cnt = 1;
    } else {
        Board.cnt = 2;
    }

    Timer.cnt = Itime / 10 - 1;
    Diff.cnt = diff;

    while (!WindowShouldClose()) {
        PlayMusicBackGround.Run();
        // Compute scale for current window
        int winW = GetScreenWidth();
        int winH = GetScreenHeight();
        float scale = (float) (winW < winH ? winW : winH) / (float) screenSize;
        float scaled = screenSize * scale;
        float offsetX = (winW - scaled) * 0.5f;
        float offsetY = (winH - scaled) * 0.5f;

        // Map mouse to the real coords 
        SetMouseOffset((int)-offsetX, (int)-offsetY);
        SetMouseScale(1.0f/scale, 1.0f/scale);
        // Draw to virtual render target
        
        BeginTextureMode(target);

        DrawTextureEx(BackGround, {0, 0}, 0.0f, screenSize / 1024.0f, WHITE);
        DrawCenteredText("MODE", 56.0f, 30, BLACK);
        Type.DrawButton();
        Board.DrawButton();
        Timer.DrawButton();
        if (Type.cnt) {
            Diff.DrawButton();  
            Stone.DrawButton();
            Back.y = 600;
        } else {
            Back.y = 400;
        }

        Back.DrawButton();
        EndTextureMode();
        
        // Present scaled to the actual window with letterboxing
        BeginDrawing();
        ClearBackground(BLACK);
        Rectangle src = { 0, 0, (float)target.texture.width, (float)-target.texture.height };
        Rectangle dst = { offsetX, offsetY, scaled, scaled };
        DrawTexturePro(target.texture, src, dst, {0,0}, 0.0f, WHITE);
        EndDrawing();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && Back.IsHover()) {
            break;
        }
    }

    UnloadRenderTexture(target);

    if (Type.cnt == 0) {
        vsAI = 0;
    } else {
        vsAI = 1;
    }
    
    if (Board.cnt == 0) {
        InitializeGameParameters(19);
    } else if (Board.cnt == 1) {
        InitializeGameParameters(9);
    } else {
        InitializeGameParameters(13);
    }

    diff = Diff.cnt;
    Itime = (Timer.cnt + 1) * 10;
    userStone = Stone.cnt;
}
