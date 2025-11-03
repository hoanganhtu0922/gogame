#pragma once
//Library 

#include <array>
#include <queue>
#include <cmath>
#include <raylib.h>
#include <vector>
#include "soundEffect.h"
#include "initial.h"

class IMG{
public:
	IMG();
	void remove_background(Image &to_remove);
	Texture2D Black, White, UndoTex, SkipButton, RedoButton, ExitButton;
};