#pragma once
#include "LoadGame.h"

Board LoadGame(std::string filename) {
    std::ifstream in(filename);
    if (!in.is_open()) return Board();

    Board board;

    in >> gridSize >> vsAI >> diff >> userStone >> Itime >> board.counterWhite.paused_time >> board.counterBlack.paused_time >> board.cntpass;
    in >> board.turn >> board.maxturn;
    
    for (int i = 1; i <= board.maxturn; i++) {
        int sz;
        in >> sz;
        in >> isSkip[i] >> StageTurn[i];
        stage[i].clear();
        for (int j = 0; j < sz; j++) {
            point p;
            in >> p.x >> p.y >> p.black;
            stage[i].push_back(p);
        }
    }

    board.points = stage[board.turn];
    board.black = StageTurn[board.turn];
    
    in.close();
    return board;
}