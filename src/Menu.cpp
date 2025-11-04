#pragma once
#include "Menu.h"
#include "Mode.h"
#include <string>
#include <iostream>

static void DrawCenteredText(const char* text, float fontSize, int y, Color color) {
    Vector2 sz = MeasureTextEx(uiFont, text, fontSize, 1.0f);
    DrawTextEx(uiFont, text, {(float)(screenSize - sz.x)/2.0f, (float)y}, fontSize, 1.0f, color);
}

static bool IsMouseOverRect(Rectangle r, Vector2 m) {
    return m.x >= r.x && m.x <= r.x + r.width && m.y >= r.y && m.y <= r.y + r.height;
}

static void DrawButton(Rectangle r,const char* label, bool hover) {
    Color bg = hover ? Color{50, 120, 200, 255} : Color{30, 30, 30, 255};
    Color border = {220, 220, 220, 255};
    DrawRectangleRounded(r, 0.2f, 8, bg);
    DrawRectangleRoundedLines(r, 0.2f, 8, border);
    int fontSize = 28;
    float fs = 28.0f, sp = 1.0f;

    Vector2 sz = MeasureTextEx(uiFont, label, fs, sp);
    DrawTextEx(uiFont, label, {(r.x + (r.width - sz.x) / 2), (r.y + (r.height - sz.y) / 2)}, fs, sp, RAYWHITE);
}

MenuChoice Menu::Run() {
    SetTargetFPS(60);
    EnsureUIFont();
    
    // Render to a fixed virtual square and scale to window
    RenderTexture2D target = LoadRenderTexture(screenSize, screenSize);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    while (!WindowShouldClose()) {
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

        Vector2 mouse = GetMousePosition();
        Rectangle ContinueButton = { (float)(screenSize / 2 - 140), (float)(screenSize / 2 - 110), 280.0f, 60.0f };
        Rectangle PlayButton = { (float)(screenSize / 2 - 140), (float)(screenSize / 2 - 30), 280.0f, 60.0f };
        Rectangle ModeButton  = { (float)(screenSize / 2 - 140), (float)(screenSize / 2 + 50), 280.0f, 60.0f };
        Rectangle SettingButton =  { (float)(screenSize / 2 - 140), (float)(screenSize / 2 + 130), 280.0f, 60.0f };
        Rectangle ExitButton = { (float)(screenSize / 2 - 140), (float)(screenSize / 2 + 210), 280.0f, 60.0f };

        bool HoverContinue = IsMouseOverRect(ContinueButton, mouse);
        bool HoverPlay = IsMouseOverRect(PlayButton, mouse);
        bool HoverExit  = IsMouseOverRect(ExitButton, mouse);
        bool HoverSetting = IsMouseOverRect(SettingButton, mouse);
        bool HoverMode  = IsMouseOverRect(ModeButton, mouse);

        // Draw to virtual render target
        BeginTextureMode(target);
        ClearBackground({30, 30, 30, 255});
        DrawCenteredText("GO BOARD", 48, 120, RAYWHITE);
        DrawButton(ContinueButton, "CONTINUE", HoverContinue);
        DrawButton(PlayButton, "PLAY", HoverPlay);
        DrawButton(ModeButton, "MODE", HoverMode);
        DrawButton(SettingButton, "SETTING", HoverSetting);
        DrawButton(ExitButton, "EXIT", HoverExit);
        EndTextureMode();

        // Present scaled to the actual window with letterboxing
        BeginDrawing();
        ClearBackground(BLACK);
        Rectangle src = { 0, 0, (float)target.texture.width, (float)-target.texture.height };
        Rectangle dst = { offsetX, offsetY, scaled, scaled };
        DrawTexturePro(target.texture, src, dst, {0,0}, 0.0f, WHITE);
        EndDrawing();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (HoverContinue) {
                UnloadRenderTexture(target);
                return MenuChoice::Continue;
            }

            if (HoverPlay) { 
                UnloadRenderTexture(target); return MenuChoice::Start1v1; 
            }

            if (HoverSetting) { 
                UnloadRenderTexture(target); 
            }

            if (HoverMode) {
                UnloadRenderTexture(target);
                Mode mode;
                mode.Run();
            }

            if (HoverExit)  { 
                UnloadRenderTexture(target); 
                return MenuChoice::Exit; 
            }
        }
    }
    UnloadRenderTexture(target);
    return MenuChoice::Exit;
}
