#pragma once
#include <fstream>
#include <string>

class Board; // forward declaration

void SaveGame(const Board& board, std::string filename);
