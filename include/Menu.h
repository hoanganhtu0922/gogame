#pragma once

#include <raylib.h>
#include "initial.h"
#include "font.h"
#include "Mode.h"
#include "Button.h"
#include "Setting.h"
#include <string>
#include <iostream>
#include "Music.h"

enum class MenuChoice { Play, Exit, Mode, Setting, Continue };

class Menu {
public:
    MenuChoice Run();
    
};

