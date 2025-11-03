#include "Mode.h"
#include "Menu.h"
#include <string>

static void DrawCenteredText(const char* text, int fontSize, int y, Color color) {
    int width = MeasureText(text, fontSize);
    DrawText(text, (screenSize - width) / 2, y, fontSize, color);
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
    int textWidth = MeasureText(label, fontSize);
    DrawText(label, (int)(r.x + (r.width - textWidth) / 2), (int)(r.y + (r.height - fontSize) / 2), fontSize, RAYWHITE);
}

void Mode::Run() {
    std::string VS[2] = {"1 VS 1", "VS AI"};
    std::string Board[3] = {"11 x 11", "13 x 13", "19 x 19"};
    std::string Med[3] = {"EASY", "MEDIUM", "HARD"};

    SetTargetFPS(60);

    // Render to a fixed virtual square and scale to window
    RenderTexture2D target = LoadRenderTexture(screenSize, screenSize);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);
    int CountVS = 0, CountBoard = 0, CountMed = 0;

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
        Rectangle VSButton = { (float)(screenSize / 2 - 140), (float)(screenSize / 2 - 30), 280.0f, 60.0f };
        Rectangle BoardButton  = { (float)(screenSize / 2 - 140), (float)(screenSize / 2 + 50), 280.0f, 60.0f };
        Rectangle MedButton =  { (float)(screenSize / 2 - 140), (float)(screenSize / 2 + 130), 280.0f, 60.0f };
        Rectangle BackButton = { (float)(screenSize / 2 - 140), (float)(screenSize / 2 + 210), 280.0f, 60.0f };

        bool HoverVS = IsMouseOverRect(VSButton, mouse);
        bool HoverBoard = IsMouseOverRect(BoardButton, mouse);
        bool HoverMed = IsMouseOverRect(MedButton, mouse);
        bool HoverBack = IsMouseOverRect(BackButton, mouse);

        // Draw to virtual render target
        BeginTextureMode(target);
        ClearBackground({30, 30, 30, 255});
        DrawCenteredText("CHOOSE YOUR MODE", 48, 120, RAYWHITE);
        DrawButton(VSButton, VS[CountVS % 2].c_str(), HoverVS);
        DrawButton(BoardButton, Board[CountBoard % 3].c_str(), HoverBoard);
        if (CountVS % 2) DrawButton(MedButton, Med[CountMed % 3].c_str(), HoverMed);
        DrawButton(BackButton, "BACK", HoverBack);
        EndTextureMode();

        // Present scaled to the actual window with letterboxing
        BeginDrawing();
        ClearBackground(BLACK);
        Rectangle src = { 0, 0, (float)target.texture.width, (float)-target.texture.height };
        Rectangle dst = { offsetX, offsetY, scaled, scaled };
        DrawTexturePro(target.texture, src, dst, {0,0}, 0.0f, WHITE);
        EndDrawing();
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (HoverVS) { CountVS++; }
            if (HoverBoard) { CountBoard++; }
            if (HoverMed) { CountMed++; }
            if (HoverBack) { return;}
        }
    }
    UnloadRenderTexture(target);
}
