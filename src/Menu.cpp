#pragma once
#include "Menu.h"


static void DrawCenteredText(const char* text, float fontSize, int y, Color color) {
    Vector2 sz = MeasureTextEx(uiFont, text, fontSize, 1.0f);
    DrawTextEx(uiFont, text, {(float)(screenSize - sz.x)/2.0f, (float)y}, fontSize, 1.0f, color);
}

MenuChoice Menu::Run() {
    SetTargetFPS(60);
    EnsureUIFont();
    
    // Render to a fixed virtual square and scale to window
    RenderTexture2D target = LoadRenderTexture(screenSize, screenSize);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);
    // Load UI textures once

    Image img = LoadImage("../../image/background.png");
    Texture2D BackGround = LoadTextureFromImage(img);
    UnloadImage(img);

    //Button on main menu 
    float XMiddle = (float)(screenSize  - 519.0f * 0.35f) / 2;
    MenuButton Continue(XMiddle, 200.f, 0, "CONTINUE");
    MenuButton Play(XMiddle, 300.f, 0, "PLAY");
    MenuButton Mod(XMiddle, 400.f, 0, "MODE");
    MenuButton Sett(XMiddle, 500.f, 0, "SETTING");
    MenuButton Exit(XMiddle, 600.f, 0, "EXIT");

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
        DrawCenteredText("GO BOARD", 56.0f, 120, BLACK);
        Continue.DrawButton();
        Play.DrawButton();
        Mod.DrawButton();
        Sett.DrawButton();
        Exit.DrawButton();
        EndTextureMode();
        
        // Present scaled to the actual window with letterboxing
        BeginDrawing();
        ClearBackground(BLACK);
        Rectangle src = { 0, 0, (float)target.texture.width, (float)-target.texture.height };
        Rectangle dst = { offsetX, offsetY, scaled, scaled };
        DrawTexturePro(target.texture, src, dst, {0,0}, 0.0f, WHITE);
        EndDrawing();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (Continue.IsHover()) {
                return MenuChoice::Continue;
            }

            if (Play.IsHover()) {
                return MenuChoice::Play;
            }

            if (Mod.IsHover()) {
                Mode T;
                T.Run();
                continue;
            }

            if (Sett.IsHover()) {
                Setting St;
                St.Run();
                continue;
            }

            if (Exit.IsHover()) {
                return MenuChoice::Exit;
            }
        }
    }

    UnloadRenderTexture(target);
    return MenuChoice::Exit;
}
