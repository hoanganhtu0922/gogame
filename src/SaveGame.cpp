#include "SaveGame.h"
#include "Board.h"

void SaveGame(const Board& board, std::string filename) {
    std::ofstream out(filename);
    if (!out.is_open()) return;

    out << board.turn << std::endl;
    out << board.black << std::endl;
    out << board.points.size() << std::endl;

    for (auto p : board.points) {
        out << p.x << " " << p.y << " " << p.black << std::endl;
    }

    out.close();
}
