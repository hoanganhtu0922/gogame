#include "font.h"

Font uiFont;
bool uiFontLoaded = 0;

// Ensure the font
void EnsureUIFont() {
    uiFont = LoadFontEx("../../fonts/Roboto-Bold.ttf", 48, nullptr, 250);
    SetTextureFilter(uiFont.texture, TEXTURE_FILTER_BILINEAR); // smooth render
}