#pragma once

#include <raylib.h>
#include "initial.h"

enum class MenuChoice { Start1v1, Exit, Setting, Continue };

class Menu {
public:
    MenuChoice Run();
};

