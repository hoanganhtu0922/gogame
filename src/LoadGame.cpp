#pragma once
#include <fstream>
#include "Board.h"

Board LoadGame(std::string filename) {
    std::ifstream in(filename);
    if (!in.is_open()) return Board();

    Board board;
    in >> board.turn;
    in >> board.black;

    size_t pointsCount;
    in >> pointsCount;
    board.points.resize(pointsCount);

    for (auto& p : board.points) {
        in >> p.x >> p.y >> p.black;
    }

    in.close();
    return board;
}

