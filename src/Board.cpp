#pragma once
#include "Board.h"
#include <algorithm>
#include "SaveGame.h"
#include "SaveGame.h"
#include <algorithm>

std::array<std::vector<point>, 100000> stage;
bool StageTurn[100000], isSkip[100000];

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

void Board::RemoveCapturedStones(int x, int y, int turn, std::vector<point> &points, int &is_captured) {
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

bool Board::detectWin() {
    int cntBlack = 0, cntWhite = 0;
    int board[20][20], vis[20][20];

    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            board[i][j] = -1;
            vis[i][j] = 0;
        }
    }

    for (const auto &p : points) {
        if (p.black) {
            cntBlack++;
        } else {
            cntWhite++;
        }

        board[p.x][p.y] = p.black;
    }

    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            if (board[i][j] == -1 && vis[i][j] == 0) {
                int cnt = 0;
                std::queue<std::pair<int, int>> q;
                q.push({i, j});
                vis[i][j] = 1;
                int adjBlack = 0, adjWhite = 0, adjOut = 0;

                while (!q.empty()) {
                    int x = q.front().first;
                    int y = q.front().second;

                    q.pop();
                    cnt++;

                    for (int dir = 0; dir < 4; dir++) {
                        int nx = x + X[dir];
                        int ny = y + Y[dir];

                        if (nx >= 0 && nx < gridSize && ny >= 0 && ny < gridSize) {
                            if (board[nx][ny] == -1 && vis[nx][ny] == 0) {
                                vis[nx][ny] = 1;
                                q.push({nx, ny});
                            } else if (board[nx][ny] == 0) {
                                adjWhite = 1;
                            } else if (board[nx][ny] == 1) {
                                adjBlack = 1;
                            }
                        } else {
                            adjOut = 1;
                        }
                    }
                }

                if (adjBlack && !adjWhite && !adjOut) {
                    cntBlack += cnt;
                } else if (adjWhite && !adjBlack && !adjOut) {
                    cntWhite += cnt;
                }
            }
        }
    }

    if (cntWhite >= cntBlack) {
        return 0;
    } else {
        return 1;
    }
}

bool ComparePoints(const point &a, const point &b) {
    return std::vector<int>{a.x, a.y, a.black} < std::vector<int>{b.x, b.y, b.black};
}

bool Board::isTheSame(std::vector<point> a, std::vector<point> b) {
    if (a.size() != b.size()) return false;
    sort(a.begin(), a.end(), ComparePoints);
    sort(b.begin(), b.end(), ComparePoints);
    return a == b;
}

bool Board::ValidMove(int x, int y, int black) {
    for (auto &p : points) {
        if (p.x == x && p.y == y) {
            return false; // position already occupied
        }
    }

    std::vector<point> temp_points = points;
    temp_points.push_back({x, y, (bool)black});
    std::vector<std::pair<int, int>> captured = ListOfCapturedStone(x, y, black);

    int is_captured = 0;
    RemoveCapturedStones(x, y, black, temp_points, is_captured);
    
    if (turn >= 1) {
        // Check for Ko rule violation
        if (isTheSame(temp_points, stage[turn - 1])) {
            return false; // Ko rule violation
        }
    }

    if (!captured.empty()) {
        return true; // capturing move is valid
    }


    // Check if the new stone has any liberties
    std::vector<std::vector<int>> board(gridSize, std::vector<int>(gridSize, -1));
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
            if (nx >= 0 && nx < gridSize && ny >= 0 && ny < gridSize) {
                if (board[nx][ny] == -1) {
                    return true; // has liberty
                } else if (board[nx][ny] == black) {
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
        int cx = margin + s.x * cellSize - img.Black.width * 0.06f / 2;
        int cy = margin + s.y * cellSize - img.Black.height * 0.06f / 2;
        if (s.black) {
            DrawTextureEx(img.Black, {(float)cx, (float)cy }, 0.0f, 0.06f, WHITE);
        } else {
            DrawTextureEx(img.White, {(float)cx, (float)cy }, 0.0f, 0.06f, WHITE);
        }
    }
}

void Board::StonesHoverEffect(int black) {
    Vector2 mouse = GetMousePosition();
    Color blurBlack = {255, 255, 255, 150};
    Color blurWhite = {255, 255, 255, 220};
    
    int i = (int)((mouse.x - margin + cellSize/2) / cellSize);
    int j = (int)((mouse.y - margin + cellSize/2) / cellSize);

    if (i >= 0 && i < gridSize && j >= 0 && j < gridSize && ValidMove(i, j, black)) {
        if (black) {
            DrawTextureEx(img.Black, {(float)(margin + i * cellSize - img.Black.width * 0.06f / 2), (float)(margin + j * cellSize - img.Black.height * 0.06f / 2) }, 0.0f, 0.06f, blurBlack);
        } else {
            DrawTextureEx(img.White, {(float)(margin + i * cellSize - img.White.width * 0.06f / 2), (float)(margin + j * cellSize - img.White.height * 0.06f / 2) }, 0.0f, 0.06f, blurWhite);
        }
    }
}

void Board::DrawBoard() {
    Color wood = {222, 184, 135, 255};
    ClearBackground(wood);

    for (int k = 0; k < gridSize; ++k) {
        DrawLine(margin, margin + k * cellSize, margin + (gridSize - 1) * cellSize, margin + k * cellSize, BLACK);
        DrawLine(margin + k * cellSize, margin, margin + k * cellSize, margin + (gridSize - 1) * cellSize, BLACK);
    }

    int star[3] = {3, 9, 15};
    for (int si : star)
        for (int sj : star)
            DrawCircle(margin + si * cellSize, margin + sj * cellSize, 4, BLACK);
}

void Board::PlaceStone(int i, int j) {
    if (ValidMove(i, j, 1 ^ black) == true) {
        turn++;
        maxturn = turn;
        
        black ^= 1;
        int is_captured = 0;
        PlaySound(sounds.place_stone);
        RemoveCapturedStones(i, j, black, points, is_captured);
        points.push_back({i, j, (bool)(black)});
        if (is_captured) {
            PlaySound(sounds.win_some_stones);
        }

        stage[turn] = points;
        StageTurn[turn] = black;
    }
}

void Board::PressRedoButton() {
    turn = maxturn;
    points = stage[turn];
    black = StageTurn[turn];
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
    isSkip[turn] = 1;
    if (isSkip[turn - 1]) {
        matchEnd = 1;
    }
}

void Board::PressExitButton() {
    PlaySound(sounds.press);
    ExitToMenu = 1;
}

void Board::PressGGButton() {
    PlaySound(sounds.press);
    matchEndGG = 1;
}

std::string Board::TypeOfButton() {
    Vector2 mouse = GetMousePosition();
    int i = (int)((mouse.x - margin + cellSize/2) / cellSize);
    int j = (int)((mouse.y - margin + cellSize/2) / cellSize);

    if (i >= 0 && i < gridSize && j >= 0 && j < gridSize) {
        return "InBoard";
    } 

    Rectangle undoRect = { (float)(screenSize / 2 - 200), (float)(screenSize - 75),
                            img.UndoTex.width * 0.3f, img.UndoTex.height * 0.3f };
    Rectangle skipRect = { (float)(screenSize / 2 - img.SkipButton.width * 0.06f), (float)(screenSize - 75),
                            img.SkipButton.width * 0.06f, img.SkipButton.height * 0.06f };
    Rectangle redoRect = { (float)(screenSize / 2 + 80), (float)(screenSize - 75),
                            img.RedoButton.width * 0.06f, img.RedoButton.height * 0.06f };
    Rectangle exitRect = { (float)(screenSize - 100), (float)(screenSize - 75),
                           img.ExitButton.width * 0.3f, img.ExitButton.height * 0.3f };

    Rectangle GGRect = { (float)(screenSize / 2 - 320), (float)(screenSize - 75),
                        img.GG.width * 0.06f, img.GG.height * 0.06f };

    auto inRect = [](Vector2 p, const Rectangle& r) {
        return p.x >= r.x && p.x <= (r.x + r.width) && p.y >= r.y && p.y <= (r.y + r.height);
    };

    if (inRect(mouse, undoRect)) return "Undo";
    if (inRect(mouse, redoRect)) return "Redo";
    if (inRect(mouse, skipRect)) return "Skip";
    if (inRect(mouse, exitRect)) return "Exit";
    if (inRect(mouse, GGRect)) return "GG";

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
        } else if (Type == "GG") {
            PressGGButton();
        }
    }
}

void Board::DrawButton(int HoverUndo, int HoverRedo, int HoverSkip, int HoverExit, int HoverGG) {
    float size = 1.1f;
    if (HoverUndo) {
        DrawTextureEx(img.UndoTex, {(float)(screenSize / 2 - 200), (float)(screenSize - 75)}, 0.0f, 0.3f * size, WHITE);
    } else {
        DrawTextureEx(img.UndoTex, {(float)(screenSize / 2 - 200), (float)(screenSize - 75)}, 0.0f, 0.3f, WHITE);
    }

    if (HoverSkip) {
        DrawTextureEx(img.SkipButton, {(float)(screenSize / 2 - img.SkipButton.width * 0.06f), (float)(screenSize - 75)}, 0.0f, 0.06f * size, WHITE);
    } else {
        DrawTextureEx(img.SkipButton, {(float)(screenSize / 2 - img.SkipButton.width * 0.06f), (float)(screenSize - 75)}, 0.0f, 0.06f, WHITE);
    }

    if (HoverRedo) {
        DrawTextureEx(img.RedoButton, {(float)(screenSize / 2 + 80), (float)(screenSize - 75)}, 0.0f, 0.06f * size, WHITE);
    } else {
        DrawTextureEx(img.RedoButton, {(float)(screenSize / 2 + 80), (float)(screenSize - 75)}, 0.0f, 0.06f, WHITE);
    }

    if (HoverExit) {
        DrawTextureEx(img.ExitButton, {(float)(screenSize - 100), (float)(screenSize - 75)}, 0.0f, 0.3f * size, WHITE);
    } else {
        DrawTextureEx(img.ExitButton, {(float)(screenSize - 100), (float)(screenSize - 75)}, 0.0f, 0.3f, WHITE);
    }

    if (HoverGG) {
        DrawTextureEx(img.GG, {(float)(screenSize / 2 - 320), (float)(screenSize - 75)}, 0.0f, 0.06f * size, WHITE);
    } else {
        DrawTextureEx(img.GG, {(float)(screenSize / 2 - 320), (float)(screenSize - 75)}, 0.0f, 0.06f, WHITE);
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
    winningTable.run();
    SetTargetFPS(60);

    // Render to a fixed virtual square and scale to window
    RenderTexture2D target = LoadRenderTexture(screenSize, screenSize);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    int inputmouse = 1;
    while (!WindowShouldClose() && !ExitToMenu && !winningTable.exit) {
        PlayMusicBackGround.Run();
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

        int HoverUndo = 0, HoverRedo = 0, HoverSkip = 0, HoverExit = 0, HoverGG = 0;
        std::string Type = TypeOfButton();
        if (Type == "Undo") HoverUndo = 1;
        else if (Type == "Redo") HoverRedo = 1;
        else if (Type == "Skip") HoverSkip = 1;
        else if (Type == "Exit") HoverExit = 1;
        else if (Type == "GG") HoverGG = 1;

        // Draw to virtual render target
        BeginTextureMode(target);
        DrawBoard();
        DrawWhiteRectangle();
        DrawBlackRectangle();
        DrawStone();

        if (matchEnd || matchEndGG) {
            if (matchEnd) {
                winningTable.drawTable(detectWin());
            } else {
                winningTable.drawTable(black);
            }
        } else {
            StonesHoverEffect(black ^ 1);
            if (inputmouse == 0)
                Action();

            DrawButton(HoverUndo, HoverRedo, HoverSkip, HoverExit, HoverGG);
        }

        EndTextureMode();
        inputmouse = 0;

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
}
