#pragma once
#include "Board.h"
#include <algorithm>
#include "SaveGame.h"
#include "SaveGame.h"
#include <algorithm>

std::array<std::vector<point>, 100000> stage;
bool StageTurn[100000];

std::vector<std::pair<int, int>> Board::ListOfCapturedStone(int x, int y, int turn) {
    std::vector<std::vector<int>> board(19, std::vector<int>(19, -1));
    std::vector<std::vector<int>> degree(19, std::vector<int>(19, 0));

    for (const auto &p : points) {
        board[p.x][p.y] = p.black;
    }

    board[x][y] = turn;
    for (int x = 0; x < 19; x++) {
        for (int y = 0; y < 19; y++) {
            if (board[x][y] != -1) {
                for (int dir = 0; dir < 4; dir++) {
                    int nx = x + X[dir];
                    int ny = y + Y[dir];
                    if (nx >= 0 && nx < 19 && ny >= 0 && ny < 19) {
                        if (board[nx][ny] == -1) {
                            degree[x][y]++;
                        }
                    }
                }
            }
        }
    }

    std::vector<std::pair<int, int>> Remove;

    for (int x = 0; x < 19; x++) {
        for (int y = 0; y < 19; y++) {
            if (board[x][y] == (1 - turn) && degree[x][y] == 0) {
                std::queue <std::pair<int, int>> q;
                q.push({x, y});
                std::vector<std::pair<int, int>> toRemove;
                bool liberty = 1;
                board[x][y] = -2; // mark as visited
                
                while (!q.empty()) {
                    int cx = q.front().first;
                    int cy = q.front().second;
                    q.pop();
                    toRemove.push_back({cx, cy});

                    for (int dir = 0; dir < 4; dir++) {
                        int nx = cx + X[dir];
                        int ny = cy + Y[dir];
                        if (nx >= 0 && nx < 19 && ny >= 0 && ny < 19) {
                            if (board[nx][ny] == (1 - turn)) {
                                if (degree[nx][ny] > 0)
                                    liberty = 0;

                                board[nx][ny] = -2; // mark as visited
                                q.push({nx, ny});
                            }
                        }
                    }
                }
                
                if (liberty) {
                    for (auto stone : toRemove) {
                        Remove.push_back(stone);
                    }
                }
            }
        }
    }

    return Remove;
}

void Board::RemoveCapturedStones(int x, int y, int turn, int &is_captured) {
    std::vector<std::pair<int, int>> toRemove = ListOfCapturedStone(x, y, turn);
    for (const auto &pos : toRemove) {
        points.erase(std::remove_if(points.begin(), points.end(),
                    [&](const point &p) { return p.x == pos.first && p.y == pos.second; }),
                    points.end());
    }

    if (!toRemove.empty()) {
        is_captured = 1;
    }
}

bool Board::ValidMove(int x, int y, int turn) {
    for (const auto &p : points) {
        if (p.x == x && p.y == y) {
            return false; // position already occupied
        }
    }

    std::vector<point> temp_points = points;
    temp_points.push_back({x, y, (bool)turn});
    std::vector<std::pair<int, int>> captured = ListOfCapturedStone(x, y, turn);

    if (!captured.empty()) {
        return true; // capturing move is valid
    }

    // Check if the new stone has any liberties
    std::vector<std::vector<int>> board(19, std::vector<int>(19, -1));
    for (const auto &p : temp_points) {
        board[p.x][p.y] = p.black;
    }

    std::queue <std::pair<int, int>> q;
    q.push({x, y});
    board[x][y] = -2; // mark as visited

    while (!q.empty()) {
        int x = q.front().first;
        int y = q.front().second;
        q.pop();
        for (int dir = 0; dir < 4; dir++) {
            int nx = x + X[dir];
            int ny = y + Y[dir];
            if (nx >= 0 && nx < 19 && ny >= 0 && ny < 19) {
                if (board[nx][ny] == -1) {
                    return true; // has liberty
                } else if (board[nx][ny] == turn) {
                    q.push({nx, ny});
                    board[nx][ny] = -2; // mark as visited
                }
            }
        }
    }

    return false; // no liberties found
}

void Board::DrawStone() {
    for (auto &s : points) {
        int cx = margin + s.x * cellSize - img.Black.width * 0.03f / 2;
        int cy = margin + s.y * cellSize - img.Black.height * 0.03f / 2;
        if (s.black) {
            DrawTextureEx(img.Black, {(float)cx, (float)cy }, 0.0f, 0.03f, WHITE);
        } else {
            DrawTextureEx(img.White, {(float)cx, (float)cy }, 0.0f, 0.03f, WHITE);
        }
    }
}

void Board::StonesHoverEffect(int turn) {
    Vector2 mouse = GetMousePosition();
    Color blur = {255, 255, 255, 100};
    
    int i = (int)((mouse.x - margin + cellSize/2) / cellSize);
    int j = (int)((mouse.y - margin + cellSize/2) / cellSize);

    if (i >= 0 && i < gridSize && j >= 0 && j < gridSize && ValidMove(i, j, turn)) {
        if (turn) {
            DrawTextureEx(img.Black, {(float)(margin + i * cellSize - img.Black.width * 0.03f / 2), (float)(margin + j * cellSize - img.Black.height * 0.03f / 2) }, 0.0f, 0.03f, blur);
        } else {
            DrawTextureEx(img.White, {(float)(margin + i * cellSize - img.White.width * 0.03f / 2), (float)(margin + j * cellSize - img.White.height * 0.03f / 2) }, 0.0f, 0.03f, blur);
        }
    }
}

void Board::DrawBoard() {
    Color wood = {222, 184, 135, 255};
    ClearBackground(wood);

    for (int k = 0; k < gridSize; ++k) {
        DrawLine(margin, margin + k * cellSize,
                    margin + (gridSize - 1) * cellSize, margin + k * cellSize, BLACK);
        DrawLine(margin + k * cellSize, margin,
                    margin + k * cellSize, margin + (gridSize - 1) * cellSize, BLACK);
    }

    int star[3] = {3, 9, 15};
    for (int si : star)
        for (int sj : star)
            DrawCircle(margin + si * cellSize, margin + sj * cellSize, 4, BLACK);
}

void Board::PlaceStone(int i, int j) {
    if (ValidMove(i, j, 1 ^ black) == true) {
        turn++;
        black ^= 1;
        int is_captured = 0;
        PlaySound(sounds.place_stone);
        RemoveCapturedStones(i, j, black, is_captured);
        points.push_back({i, j, (bool)(black)});
        if (is_captured) {
            PlaySound(sounds.win_some_stones);
        }

        stage[turn] = points;
        StageTurn[turn] = black;
    } else {
        PlaySound(sounds.error);
    }
}

void Board::PressRedoButton() {
    PlaySound(sounds.press);
}

void Board::PressUndoButton() {
    PlaySound(sounds.press);
    if (turn > 0) {
        turn--;
        points = stage[turn];
        black = StageTurn[turn];
    }
}

void Board::PressSkipButton() {
    PlaySound(sounds.press);
    turn++;
    black ^= 1;
    stage[turn] = points;
    StageTurn[turn] = black;
}

void Board::PressExitButton() {
    PlaySound(sounds.press);
}

std::string Board::TypeOfButton() {
    Vector2 mouse = GetMousePosition();
    int i = (int)((mouse.x - margin + cellSize/2) / cellSize);
    int j = (int)((mouse.y - margin + cellSize/2) / cellSize);

    if (i >= 0 && i < gridSize && j >= 0 && j < gridSize) {
        return "InBoard";
    } 

    if (mouse.x >= screenSize / 2 - 200 && mouse.x <= (float)(screenSize / 2 - 200 + img.UndoTex.width * 0.27) &&
    mouse.y >= screenSize - 75 && mouse.y <= (float)(screenSize - 75 + img.UndoTex.height * 0.27)) {
        return "Undo";
    }

    if (mouse.x >= screenSize / 2 + 140 && mouse.x <= (float)(screenSize / 2 + 140 + img.RedoButton.width * 0.27) &&
    mouse.y >= screenSize - 75 && mouse.y <= (float)(screenSize - 75 + img.RedoButton.height * 0.27)) {
        return "Redo";
    }

    if (mouse.x >= (float)(screenSize / 2 - img.SkipButton.width * 0.08) && mouse.x <= (float)(screenSize / 2 + img.SkipButton.width * 0.08) &&
    mouse.y >= screenSize - 80 && mouse.y <= (float)(screenSize - 80 + img.SkipButton.height * 0.16)) {
        return "Skip";
    }

    if (mouse.x >= (float)(screenSize - 85) && mouse.x <= (float)(screenSize - 85 + img.ExitButton.width * 0.09) &&
    mouse.y >= screenSize - 85 && mouse.y <= (float)(screenSize - 85 + img.ExitButton.height * 0.09)) {
        return "Exit";
    }

    return "None";
}

void Board::Action() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        std::string Type = TypeOfButton();
        if (Type == "InBoard") {
            Vector2 mouse = GetMousePosition();
            int i = (int)((mouse.x - margin + cellSize/2) / cellSize);
            int j = (int)((mouse.y - margin + cellSize/2) / cellSize);
            PlaceStone(i, j);
        } else if (Type == "Undo") {
            PressUndoButton();
        } else if (Type == "Redo") {
            PressRedoButton();
        } else if (Type == "Skip") {
            PressSkipButton();
        } else if (Type == "Exit") {
            PressExitButton();
        }
    }
}

void Board::DrawButton(int HoverUndo, int HoverRedo, int HoverSkip, int HoverExit) {
    float size = 1.1f;
    if (HoverUndo) {
        DrawTextureEx(img.UndoTex, {(float)(screenSize / 2 - 200), (float)(screenSize - 75)}, 0.0f, 0.27f * size, WHITE);
    } else {
        DrawTextureEx(img.UndoTex, {(float)(screenSize / 2 - 200), (float)(screenSize - 75)}, 0.0f, 0.27f, WHITE);
    }

    if (HoverSkip) {
        DrawTextureEx(img.SkipButton, {(float)(screenSize / 2 - img.SkipButton.width * 0.08), (float)(screenSize - 80)}, 0.0f, 0.16f * size, WHITE);
    } else {
        DrawTextureEx(img.SkipButton, {(float)(screenSize / 2 - img.SkipButton.width * 0.08), (float)(screenSize - 80)}, 0.0f, 0.16f, WHITE);
    }

    if (HoverRedo) {
        DrawTextureEx(img.RedoButton, {(float)(screenSize / 2 + 140), (float)(screenSize - 75)}, 0.0f, 0.27f * size, WHITE);
    } else {
        DrawTextureEx(img.RedoButton, {(float)(screenSize / 2 + 140), (float)(screenSize - 75)}, 0.0f, 0.27f, WHITE);
    }

    if (HoverExit) {
        DrawTextureEx(img.ExitButton, {(float)(screenSize - 85), (float)(screenSize - 85)}, 0.0f, 0.09f * size, WHITE);
    } else {
        DrawTextureEx(img.ExitButton, {(float)(screenSize - 85), (float)(screenSize - 85)}, 0.0f, 0.09f, WHITE);
    }
}

void Board::DrawWhiteRectangle() {
    Rectangle rect = {10, 10, screenSize / 2 - 20, margin - 20};
    Color border = {220, 220, 220, 180};
    Color shadowColor = {0, 0, 0, 50};         
    Color buttonColor = {255, 255, 255, 230}; 

    if (black == 0) {
        DrawRectangleRounded({rect.x + 6, rect.y + 6, rect.width, rect.height}, 0.3f, 16, shadowColor);
        DrawRectangleRounded(rect, 0.3f, 16, buttonColor);
        DrawRectangleRoundedLines(rect, 0.3f, 16, border);
    } else {
        DrawRectangleRounded(rect, 0.3f, 16, buttonColor);
    }
}

void Board::DrawBlackRectangle() {
    Rectangle rect = {screenSize / 2 + 10, 10, screenSize / 2 - 20, margin - 20};

    Color darkMain = {20, 20, 20, 220};   
    Color darkShadow = {0, 0, 0, 70};    

    if (black == 1) {
        DrawRectangleRounded({rect.x + 6, rect.y + 6, rect.width, rect.height}, 0.3f, 16, darkShadow);
        DrawRectangleRounded(rect, 0.3f, 16, darkMain);
        DrawRectangleRoundedLines(rect, 0.3f, 16, {70, 70, 70, 200});
    } else {
        DrawRectangleRounded(rect, 0.3f, 16, darkMain);
    }
}

void Board::LoopGame() {
    SetTargetFPS(60);

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

        // Map mouse to virtual coords so existing logic works
        SetMouseOffset((int)-offsetX, (int)-offsetY);
        SetMouseScale(1.0f/scale, 1.0f/scale);

        int HoverUndo = 0, HoverRedo = 0, HoverSkip = 0, HoverExit = 0;
        std::string Type = TypeOfButton();
        if (Type == "Undo") HoverUndo = 1;
        else if (Type == "Redo") HoverRedo = 1;
        else if (Type == "Skip") HoverSkip = 1;
        else if (Type == "Exit") HoverExit = 1;

        // Draw to virtual render target
        BeginTextureMode(target);
        DrawBoard();
        StonesHoverEffect(black ^ 1);
        Action();
        DrawButton(HoverUndo, HoverRedo, HoverSkip, HoverExit);
        DrawWhiteRectangle();
        DrawBlackRectangle();
        DrawStone();
        EndTextureMode();

        // Present scaled to the actual window with letterboxing
        BeginDrawing();
        ClearBackground(BLACK);
        Rectangle src = { 0, 0, (float)target.texture.width, (float)-target.texture.height };
        Rectangle dst = { offsetX, offsetY, scaled, scaled };
        DrawTexturePro(target.texture, src, dst, {0,0}, 0.0f, WHITE);
        EndDrawing();
    }

    SaveGame(*this, "SaveGame.txt");
    UnloadRenderTexture(target);
    CloseAudioDevice();
    CloseWindow();
}
