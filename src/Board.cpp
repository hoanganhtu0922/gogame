#pragma once
#include "Board.h"

std::array<std::vector<point>, 100000> stage;
bool StageTurn[100000], isSkip[100000];

std::vector<std::pair<int, int>> Board::ListOfCapturedStone(int x, int y, int turn) {
    std::vector<std::vector<int>> board(19, std::vector<int>(19, -1));
    std::vector<std::vector<int>> degree(19, std::vector<int>(19, 0));

    for (const auto &p : points) {
        board[p.x][p.y] = p.black;
    }

    board[x][y] = turn;
    for (int x = 0; x < gridSize; x++) {
        for (int y = 0; y < gridSize; y++) {
            if (board[x][y] != -1) {
                for (int dir = 0; dir < 4; dir++) {
                    int nx = x + X[dir];
                    int ny = y + Y[dir];
                    if (nx >= 0 && nx < gridSize && ny >= 0 && ny < gridSize) {
                        if (board[nx][ny] == -1) {
                            degree[x][y]++;
                        }
                    }
                }
            }
        }
    }

    std::vector<std::pair<int, int>> Remove;

    for (int x = 0; x < gridSize; x++) {
        for (int y = 0; y < gridSize; y++) {
            if (board[x][y] == (turn ^ 1) && degree[x][y] == 0) {
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
                        if (nx >= 0 && nx < gridSize && ny >= 0 && ny < gridSize) {
                            if (board[nx][ny] == (1^turn)) {
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
    cntBlack = 0;
    cntWhite = 6;
    int board[20][20], vis[20][20];

    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
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
    
    /*if (turn >= 1) {
        // Check for Ko rule violation
        if (isTheSame(temp_points, stage[turn - 1])) {
            return false; // Ko rule violation
        }
    }*/

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
void Board::DrawBackground() {
    ClearBackground(BLACK);
    std::cout << GetScreenHeight() << " " << GetScreenWidth() << "\n";
    float scale = (float)screenSize / (float)BackGround.width;
    Vector2 origin = { 0, 0 };
    DrawTextureEx(BackGround, origin, 0, scale, WHITE);
}

void Board::DrawBoard() {
    Color wood = {222, 184, 135, 255};
    
    for (int k = 0; k < gridSize; ++k) {
        DrawLine(margin, margin + k * cellSize, margin + (gridSize - 1) * cellSize, margin + k * cellSize, BLACK);
        DrawLine(margin + k * cellSize, margin, margin + k * cellSize, margin + (gridSize - 1) * cellSize, BLACK);
    }

    std::vector <int> star;
    if (gridSize == 19) {
        star = {3, 9, 15};
    } else if (gridSize == 13) {
        star = {3, 6, 9};
    } else {
        star = {2, 4, 6};
    }

    for (int si : star)
        for (int sj : star)
            DrawCircle(margin + si * cellSize, margin + sj * cellSize, 4, BLACK);
}

void Board::PlaceStone(int i, int j) {
    if (ValidMove(i, j, 1 ^ black) == true) {
        //std::cout << i << " " << j << " hehe" << std::endl; 
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
        if (vsAI == 0) {
            turn--;
            points = stage[turn];
            black = StageTurn[turn];
        } else {
            if (turn >= 2) {
                turn -= 2;
                points = stage[turn];
                black = StageTurn[turn];
            }
        }
    }
}

void Board::PressSkipButton() {
    PlaySound(sounds.press);
    turn++;
    black ^= 1;
    stage[turn] = points;
}

void Board::PressExitButton() {
    PlaySound(sounds.press);
    ExitToMenu = 1;
}

void Board::PressGGButton() {
    PlaySound(sounds.press);
    matchEndGG = 1;
}

void Board::PressResetButton() {
    PlaySound(sounds.press);
    turn = 0;
    maxturn = 0;
    points = stage[turn];
    black = StageTurn[turn];
    counterWhite.paused_time = counterBlack.paused_time = 0;
}

std::string Board::TypeOfButton() {
    Vector2 mouse = GetMousePosition();
    int i = (int)((mouse.x - margin + cellSize/2) / cellSize);
    int j = (int)((mouse.y - margin + cellSize/2) / cellSize);

    if (i >= 0 && i < gridSize && j >= 0 && j < gridSize) {
        return "InBoard";
    } 

    Rectangle undoRect = { (float)screenSize / 7 * 2 - img.UndoTex.width * 0.3f / 2, (float)(screenSize - 75),
                            img.UndoTex.width * 0.3f, img.UndoTex.height * 0.3f };
    Rectangle skipRect = { (float)screenSize / 7 * 3 - img.SkipButton.width * 0.06f / 2, (float)(screenSize - 75),
                            img.SkipButton.width * 0.06f, img.SkipButton.height * 0.06f };
    Rectangle redoRect = { (float)screenSize / 7 * 4 - img.RedoButton.width * 0.06f / 2, (float)(screenSize - 75),
                            img.RedoButton.width * 0.06f, img.RedoButton.height * 0.06f };
    Rectangle exitRect = { (float)screenSize / 7 * 6 - img.ExitButton.width * 0.3f / 2, (float)(screenSize - 75),
                           img.ExitButton.width * 0.3f, img.ExitButton.height * 0.3f };
    Rectangle GGRect = { (float)screenSize / 7 - img.GG.width * 0.06f / 2 , (float)(screenSize - 75),
                        img.GG.width * 0.06f, img.GG.height * 0.06f };
    Rectangle ResetRect = { (float)screenSize / 7 * 5 - img.Reset.width * 0.06f / 2, (float)(screenSize - 75),
                        img.Reset.width * 0.06f, img.Reset.height * 0.06f};

    auto inRect = [](Vector2 p, const Rectangle& r) {
        return p.x >= r.x && p.x <= (r.x + r.width) && p.y >= r.y && p.y <= (r.y + r.height);
    };

    if (inRect(mouse, undoRect)) return "Undo";
    if (inRect(mouse, redoRect)) return "Redo";
    if (inRect(mouse, skipRect)) return "Skip";
    if (inRect(mouse, exitRect)) return "Exit";
    if (inRect(mouse, GGRect)) return "GG";
    if (inRect(mouse, ResetRect)) return "Reset";

    return "None";
}

std::vector < std::vector <int>> Board::conv(std::vector<point> points) {
    std::vector<std::vector<int>> lst;
    for (auto v : points) {
        lst.push_back({v.x, v.y, v.black});
    }  

    return lst;
}

void Board::Action() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        std::string Type = TypeOfButton();
        if (Type == "InBoard") {
            Vector2 mouse = GetMousePosition();
            int i = (int)((mouse.x - margin + cellSize/2) / cellSize);
            int j = (int)((mouse.y - margin + cellSize/2) / cellSize);
            PlaceStone(i, j);
            cntpass = 0;
        } else if (Type == "Undo") {
            PressUndoButton();
            cntpass = 0;
        } else if (Type == "Redo") {
            PressRedoButton();
            cntpass = 0;
        } else if (Type == "Skip") {
            if (cntpass == 1) {
                matchEnd = 1;
            } else {
                PressSkipButton();
                cntpass++;
            }
        } else if (Type == "Exit") {
            cntpass = 0;
            PressExitButton();
        } else if (Type == "GG") {
            cntpass = 0;
            PressGGButton();
        } else if (Type == "Reset") {
            cntpass = 0;
            PressResetButton();
        }
    }
}

void Board::DrawButton(int HoverUndo, int HoverRedo, int HoverSkip, int HoverExit, int HoverGG, int HoverReset) {
    float size = 1.1f;
    if (HoverUndo) {
        DrawTextureEx(img.UndoTex, {(float)screenSize / 7 * 2 - img.UndoTex.width * 0.3f / 2, (float)(screenSize - 75)}, 0.0f, 0.3f * size, WHITE);
    } else {
        DrawTextureEx(img.UndoTex, {(float)screenSize / 7 * 2 - img.UndoTex.width * 0.3f / 2, (float)(screenSize - 75)}, 0.0f, 0.3f, WHITE);
    }

    if (HoverSkip) {
        DrawTextureEx(img.SkipButton, {(float)screenSize / 7 * 3 - img.SkipButton.width * 0.06f / 2, (float)(screenSize - 75)}, 0.0f, 0.06f * size, WHITE);
    } else {
        DrawTextureEx(img.SkipButton, {(float)screenSize / 7 * 3 - img.SkipButton.width * 0.06f / 2, (float)(screenSize - 75)}, 0.0f, 0.06f, WHITE);
    }

    if (HoverRedo) {
        DrawTextureEx(img.RedoButton, {(float)screenSize / 7 * 4 - img.RedoButton.width * 0.06f / 2, (float)(screenSize - 75)}, 0.0f, 0.06f * size, WHITE);
    } else {
        DrawTextureEx(img.RedoButton, {(float)screenSize / 7 * 4 - img.RedoButton.width * 0.06f / 2, (float)(screenSize - 75)}, 0.0f, 0.06f, WHITE);
    }

    if (HoverExit) {
        DrawTextureEx(img.ExitButton, {(float)screenSize / 7 * 6 - img.ExitButton.width * 0.3f / 2, (float)(screenSize - 75)}, 0.0f, 0.3f * size, WHITE);
    } else {
        DrawTextureEx(img.ExitButton, {(float)screenSize / 7 * 6 - img.ExitButton.width * 0.3f / 2, (float)(screenSize - 75)}, 0.0f, 0.3f, WHITE);
    }

    if (HoverGG) {
        DrawTextureEx(img.GG, {(float)screenSize / 7 * 1 - img.GG.width * 0.06f / 2, (float)(screenSize - 75)}, 0.0f, 0.06f * size, WHITE);
    } else {
        DrawTextureEx(img.GG, {(float)screenSize / 7 * 1 - img.GG.width * 0.06f / 2, (float)(screenSize - 75)}, 0.0f, 0.06f, WHITE);
    }

    //assert(IsTextureReady(img.Reset));
    if (HoverReset) {
        DrawTextureEx(img.Reset, {(float)screenSize / 7 * 5 - img.Reset.width * 0.06f / 2, (float)(screenSize - 75)}, 0.0f, 0.06f * size, WHITE);
    } else {
        DrawTextureEx(img.Reset, {(float)screenSize / 7 * 5 - img.Reset.width * 0.06f / 2, (float)(screenSize - 75)}, 0.0f, 0.06f, WHITE);
    }
}

void Board::DrawWhiteRectangle(int endgame) {
    Rectangle rect = {10, 10, screenSize / 2 - 20, margin - 20};
    Color border = {220, 220, 220, 180};
    Color shadowColor = {0, 0, 0, 50};         
    Color buttonColor = {255, 255, 255, 230}; 

    if (black == 1) {
        DrawRectangleRounded({rect.x + 6, rect.y + 6, rect.width, rect.height}, 0.3f, 16, shadowColor);
        DrawRectangleRounded(rect, 0.3f, 16, buttonColor);
        DrawRectangleRoundedLines(rect, 0.3f, 16, border);
    } else {
        DrawRectangleRounded(rect, 0.3f, 16, buttonColor);
    }

    std::string number = TextFormat("%d", cntWhite) + std::string(".5"); 
    Vector2 sz = MeasureTextEx(uiFont, number.c_str(), 21, 1.0f);
    DrawTextEx(uiFont, number.c_str(), {(float)(rect.x + (rect.width - sz.x)/2.0f), (float)(rect.y + (rect.height - sz.y)/2.0f)}, 21, 1.0f, BLACK);
}

void Board::DrawBlackRectangle(int endgame) {
    Rectangle rect = {(float)screenSize / 2 + 10, 10, (float)screenSize / 2 - 20, margin - 20};

    Color darkMain = {20, 20, 20, 220};   
    Color darkShadow = {0, 0, 0, 70};    

    if (black == 0) {
        DrawRectangleRounded({rect.x + 6, rect.y + 6, rect.width, rect.height}, 0.3f, 16, darkShadow);
        DrawRectangleRounded(rect, 0.3f, 16, darkMain);
        DrawRectangleRoundedLines(rect, 0.3f, 16, {70, 70, 70, 200});
    } else {
        DrawRectangleRounded(rect, 0.3f, 16, darkMain);
    }

    std::string number = TextFormat("%d", cntBlack); 
    Vector2 sz = MeasureTextEx(uiFont, number.c_str(), 21, 1.0f);
    DrawTextEx(uiFont, number.c_str(), {(float)(rect.x + (rect.width - sz.x)/2.0f), (float)(rect.y + (rect.height - sz.y)/2.0f)}, 21, 1.0f, WHITE);
}

void Board::DrawTimer() {
    Rectangle rectWhite = {(float)15, margin + 10, margin - 30, margin - 30};
    Rectangle rectBlack = {(float)(screenSize - margin) + 15, margin + 10, margin - 30, margin - 30};
    Color cWhite= {255, 255, 255, 230}; 
    Color cBlack = {20, 20, 20, 220};  
    DrawRectangleRounded(rectBlack, 0.3f, 16, cBlack);
    DrawRectangleRounded(rectWhite, 0.3f, 16, cWhite);

    int curtimeWhite = timeWhite - counterWhite.elapsedSeconds(), curtimeBlack = timeBlack - counterBlack.elapsedSeconds();
    std::string timerBlack = std::to_string(curtimeBlack / 60) + ":" + (curtimeBlack % 60 < 10 ? "0" : "") + std::to_string(curtimeBlack % 60); 
    Vector2 sz = MeasureTextEx(uiFont, timerBlack.c_str(), 21, 1.0f);
    DrawTextEx(uiFont, timerBlack.c_str(), {(float)(rectBlack.x + (rectBlack.width - sz.x)/2.0f), (float)(rectBlack.y + (rectBlack.height - sz.y)/2.0f)}, 21, 1.0f, WHITE);

    std::string timerWhite = std::to_string(curtimeWhite / 60) + ":" + (curtimeWhite % 60 < 10 ? "0" : "") + std::to_string(curtimeWhite % 60); 
    sz = MeasureTextEx(uiFont, timerWhite.c_str(), 21, 1.0f);
    DrawTextEx(uiFont, timerWhite.c_str(), {(float)(rectWhite.x + (rectWhite.width - sz.x)/2.0f), (float)(rectWhite.y + (rectWhite.height - sz.y)/2.0f)}, 21, 1.0f, BLACK);
}


std::pair <int, int> Board::randomMove() {
    std::vector <std::pair <int, int>> lst;
    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            if (ValidMove(i, j, 1 ^ black)) {
                lst.push_back({i, j});
            }
        }
    }

    lst.push_back({-2, -2});
    int sz = (int)lst.size();
    return lst[rand() % sz];
}

std::pair <int, int> Board::mediumMove() {
    std::vector <std::vector <int>> color(19, std::vector<int>(19, -1));
    std::vector <std::pair <int, int>> lst;

    for (auto b : points) {
        color[b.x][b.y] = b.black; 
    }

    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            if (ValidMove(i, j, 1 ^ black) == 0) {
                continue;
            }

            int sz = ListOfCapturedStone(i, j, 1 ^ black).size();
            for (int n = 0; n < sz * 20; n++) {
                lst.push_back({i, j});
            }

            if (ValidMove(i, j, black)) {
                sz = ListOfCapturedStone(i, j, black).size();
                for (int n = 0; n < sz * 20; n++) {
                    lst.push_back({i, j});
                }
            }

            for (int f = 0; f < 4; f++) {
                int x = i + X[f], y = j + Y[f];
                if (x >= 0 && x < gridSize && y >= 0 && y < gridSize) {
                    if (color[x][y] == (1 ^ black)) {
                        for (int n = 0; n < 4; n++) {
                            lst.push_back({i, j});
                        }
                    }

                    if (color[x][y] == black) {
                        lst.push_back({i, j});
                    }
                }
            }
        }
    }

    if (lst.empty()) {
        return randomMove();
    }

    lst.push_back({-2, -2});
    int sz = (int)lst.size();
    return lst[rand() % sz];
}


void Board::LoopGame() {
    if (typeboard == 2) {
        BackGround = LoadTexture("../../assets/image/japan.png");
    } else if (typeboard == 1) {
        BackGround = LoadTexture("../../assets/image/china.png");
    } else {
        BackGround = LoadTexture("../../assets/image/classic.png");
    }

    winningTable.run();
    SetTargetFPS(60);
    KataGoController AI;
    counterBlack.start();
    counterWhite.start();
    if (diff == 2) {
        AI.start();
        AI.Send("boardsize " + std::to_string(gridSize));
        AI.ReadResponse();
    }

    // Render to a fixed virtual square and scale to window
    RenderTexture2D target = LoadRenderTexture(screenSize, screenSize);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    int inputmouse = 1;
    while (!WindowShouldClose() && !ExitToMenu && !winningTable.exit) {
        if (black == 0) {
            counterBlack.resume();
            counterWhite.pause();
        } else {
            counterBlack.pause();
            counterWhite.resume();
        }
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

        int HoverUndo = 0, HoverRedo = 0, HoverSkip = 0, HoverExit = 0, HoverGG = 0, HoverReset = 0;
        std::string Type = TypeOfButton();
        if (Type == "Undo") HoverUndo = 1;
        else if (Type == "Redo") HoverRedo = 1;
        else if (Type == "Skip") HoverSkip = 1;
        else if (Type == "Exit") HoverExit = 1;
        else if (Type == "GG") HoverGG = 1; 
        else if (Type == "Reset") HoverReset = 1;

        if (vsAI && matchEnd == 0 && black != userStone) {
            std::pair <int, int> move;

            if (diff == 2) {
                move = AI.getAImove(conv(points), black);
            } else if (diff == 0) {
                move = randomMove(); 
            } else {
                move = mediumMove();
            }

            if (move.first >= 0 && move.second >= 0) {
                PlaceStone(move.first, move.second);
                cntpass = 0;
            } else if (move.first == -1) {
                PressGGButton();
            } else if (move.first == -2) {
                if (cntpass) {
                    matchEnd = 1;
                } else {
                    PressSkipButton();
                }
                cntpass++;
            }
        }

        // Draw to virtual render target
        BeginTextureMode(target);
        DrawBackground();
        DrawBoard();
        
        if (matchEnd == 0 && matchEndGG == 0){
            StonesHoverEffect(black ^ 1);
            if (inputmouse == 0)
                Action();

            DrawButton(HoverUndo, HoverRedo, HoverSkip, HoverExit, HoverGG, HoverReset);
        }

        
        DrawWhiteRectangle(matchEnd || matchEndGG);
        DrawBlackRectangle(matchEnd || matchEndGG);
        DrawStone();
        DrawTimer();
        detectWin();

        if (matchEnd || matchEndGG) {
            counterBlack.pause();
            counterWhite.pause();
            if (matchEnd) {
                winningTable.drawTable(detectWin());
            } else {
                winningTable.drawTable(black);
            }
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

    counterBlack.pause();
    counterWhite.pause();
    SaveGame(*this, "SaveGame.txt");
    UnloadRenderTexture(target);
}
