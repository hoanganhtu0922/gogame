#pragma once
//Library 

#include <array>
#include <queue>
#include <cmath>
#include <raylib.h>
#include <string>
#include <vector>
#include "soundEffect.h"
#include "IMG.h"
#include "initial.h"
#include "Music.h"
#include "table.h"
#include "button.h"
#include "SaveGame.h"
#include <algorithm>
#include "katago.h"
#include <cstdlib>  
#include <ctime>    
#include "timer.h"


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
	std::vector < std::vector <int>> conv(std::vector<point> points);
	
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

	void PressResetButton();

	void DrawWhiteRectangle(int endgame);

	void DrawBlackRectangle(int endgame);

	void DrawStone();

	void DrawButton(int HoverUndo, int HoverRedo, int HoverSkip, int HoverExit, int HoverGG, int HoverReset);
	
	void Action();

	void LoopGame();

	void DrawTimer();

	bool detectWin();
	void DrawBackground();

	std::pair <int, int> randomMove();
	std::pair <int, int> mediumMove();
	int turn = 0, maxturn = 0, cntpass = 0;
	int black = 0, cyc = 0, backtoMenu = 0, matchEndGG = 0, matchEnd = 0;
	int cntBlack = 0, cntWhite = 6, placeStoneAlready = 0, timeBlack = Itime * 60, timeWhite = Itime * 60;
	bool ExitToMenu = false;
	Timer counterBlack, counterWhite;
	std::vector<point> points;
	soundEffect sounds; 
	IMG img;
	Texture2D BackGround;
	table winningTable;
};


extern std::array<std::vector<point>, 100000> stage;
extern bool StageTurn[100000], isSkip[100000];