#pragma once
//Library 

#include <array>
#include <queue>
#include <cmath>
#include <raylib.h>
#include <string>
#include "soundEffect.h"
#include "IMG.h"
#include "initial.h"
#include "Music.h"
#include "table.h"
#include "button.h"

//Forward-declaration
class point{
public:
	int x, y;
	bool black;
	bool operator==(const point& other) const {
        return x == other.x && y == other.y && black == other.black;
    }
};

class Board
{
public:
	int X[4] = {1, -1, 0, 0};
	int Y[4] = {0, 0, 1, -1};


	std::vector<std::pair<int, int>> ListOfCapturedStone(int x, int y, int turn);

	void PressGGButton();
	
	void DrawBoard();

	void RemoveCapturedStones(int x, int y, int turn, std::vector<point> &points, int &is_captured);

	bool ValidMove(int x, int y, int turn);
	
	void StonesHoverEffect(int turn);

	bool comparePoints(const point &a, const point &b);

	bool isTheSame(const std::vector<point> a, const std::vector<point> b);

	void PlaceStone(int i, int j);

	std::string TypeOfButton();

	void PressRedoButton();

	void PressSkipButton();

	void PressUndoButton();

	void PressExitButton();

	void DrawWhiteRectangle();

	void DrawBlackRectangle();

	void DrawStone();

	void DrawButton(int HoverUndo, int HoverRedo, int HoverSkip, int HoverExit, int HoverGG);
	
	void Action();

	void LoopGame();

	bool detectWin();
	
	int turn = 0, maxturn = 0;
	int black = 0, cyc = 0, backtoMenu = 0, matchEndGG = 0, matchEnd = 0;
	bool ExitToMenu = false;
	std::vector<point> points;
	soundEffect sounds; 
	IMG img;
	table winningTable;
};


