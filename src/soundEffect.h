#pragma once
//Library 

#include <array>
#include <queue>
#include <cmath>
#include <raylib.h>
#include "initial.h"

class soundEffect{
public:
	soundEffect();
	Sound place_stone, error, win_some_stones, press;
};