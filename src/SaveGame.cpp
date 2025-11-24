#include "SaveGame.h"
#include "Board.h"

void SaveGame(const Board& board, std::string filename) {
    std::ofstream out(filename);
    if (!out.is_open()) return;

    out << board.turn << " " << board.maxturn << "\n";
    for (int i = 1; i <= board.maxturn; i++) {
        out << stage[i].size() << "\n";
        out << isSkip[i] << " " << StageTurn[i] << "\n";
        for (auto x : stage[i]) {
            out << x.x << " " << x.y << " " << x.black << "\n";
        }
    }

    out.close();
}
