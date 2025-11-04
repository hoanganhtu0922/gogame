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

//Forward-declaration
class point{
public:
	int x, y;
	bool black;
};

class Board
{
public:
	int X[4] = {1, -1, 0, 0};
	int Y[4] = {0, 0, 1, -1};

	std::vector<std::pair<int, int>> ListOfCapturedStone(int x, int y, int turn);

	void DrawBoard();

	void RemoveCapturedStones(int x, int y, int turn, int &is_captured);

	bool ValidMove(int x, int y, int turn);
	
	void StonesHoverEffect(int turn);

	void PlaceStone(int i, int j);

	std::string TypeOfButton();

	void PressRedoButton();

	void PressSkipButton();

	void PressUndoButton();

	void PressExitButton();

	void DrawWhiteRectangle();

	void DrawBlackRectangle();

	void DrawStone();

	void DrawButton(int HoverUndo, int HoverRedo, int HoverSkip, int HoverExit);
	
	void Action();

	void LoopGame();
	
	int turn = 0, maxturn = 0;
	int black = 0;
	bool ExitToMenu = false;
	std::vector<point> points;
	soundEffect sounds; 
	IMG img;
};


