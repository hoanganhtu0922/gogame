#pragma once
#include "Button.h"
#include <assert.h>    
#include <iostream>

MenuButton::MenuButton(float x_, float y_, int type_, std::string str) {
    img = LoadImage("../../image/button.jpg");
    button = LoadTextureFromImage(img);
    UnloadImage(img);
    
    img = LoadImage("../../image/moveleft.png");
    MoveLeft = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImage("../../image/moveright.png");
    MoveRight = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImage("../../image/banner.png");
    Banner = LoadTextureFromImage(img);
    assert(Banner.id);
    UnloadImage(img);

    img = LoadImage("../../image/volume0.png");
    Volume.push_back(LoadTextureFromImage(img));
    UnloadImage(img);

    img = LoadImage("../../image/volume1.jpg");
    Volume.push_back(LoadTextureFromImage(img));
    UnloadImage(img);

    img = LoadImage("../../image/volume2.jpg");
    Volume.push_back(LoadTextureFromImage(img));
    UnloadImage(img);

    img = LoadImage("../../image/volume3.jpg");
    Volume.push_back(LoadTextureFromImage(img));
    UnloadImage(img);

    img = LoadImage("../../image/volume4.jpg");
    Volume.push_back(LoadTextureFromImage(img));
    UnloadImage(img);

    img = LoadImage("../../image/volume5.jpg");
    Volume.push_back(LoadTextureFromImage(img));
    UnloadImage(img);

    x = x_, y = y_, type = type_;
    label = str;
}

bool MenuButton::IsHover() {
    Vector2 mouse = GetMousePosition();
    float _x = mouse.x;
    float _y = mouse.y;

    return _x >= x && _x <= button.width * 0.35f + x && _y >= y && _y <= y + button.height * 0.35f;
}

bool MenuButton::IsHoverRight() {
    Vector2 mouse = GetMousePosition();
    float _x = mouse.x;
    float _y = mouse.y;
    float x = this -> x;
    float y = this -> y;

    if (type == 2)
        x = 300;

    float PosX = x + button.width * 0.35f + 20;

    if (type == 2) {
        PosX = PosX - button.width * 0.35f + Volume[0].width * 0.35f;
    }

    float PosY = y;
    return _x >= PosX && _x <= MoveRight.width * 0.35f + PosX && _y >= PosY && _y <= PosY + MoveRight.height * 0.35f;
}

bool MenuButton::IsHoverLeft() {
    Vector2 mouse = GetMousePosition();
    float _x = mouse.x;
    float _y = mouse.y;
    float x = this -> x;
    float y = this -> y;
    if (type == 2)
        x = 300;
    float PosX = x - MoveLeft.width * 0.35f - 20;
    float PosY = y;
    return _x >= PosX && _x <= MoveLeft.width * 0.35f + PosX && _y >= PosY && _y <= PosY + MoveLeft.height * 0.35f;
}

void MenuButton::DrawCenteredText(float x, float y, std::string str, Texture2D button, float texScale) {
    float bw = button.width * texScale;
    float bh = button.height * texScale;

    const char* text = str.c_str();   
    float fontSize = 30.0f;
    float spacing  = 1.0f;
    Vector2 sz = MeasureTextEx(uiFont, text, 30.0f, 1.0f);
    Vector2 pos = {x + (bw - sz.x) * 0.5f, y + (bh - sz.y) * 0.5f};
    DrawTextEx(uiFont, text, pos, fontSize, spacing, WHITE);
}

void MenuButton::DrawButton() {
    SetSoundVolume(sound.press, 0.2f * SoundVol);
    float scale = (IsHover() & (!type)) ? 1.2f : 1.0f;
    const float texScale = 0.35f * scale;
    Texture2D button = this -> button;
    if (type == 2) {
        button = Volume[cnt];
    }
    
    float MiddleX = (screenSize - texScale * button.width) / 2;

    float bw = button.width * texScale;
    float bh = button.height * texScale;

    float PosX = MiddleX, PosY = y;

    if (cyc)
        label = vers[cnt];

    if (type == 2) {
        PosX = 300;
        PosY = y + 15;
    }

    DrawTextureEx(button, {PosX, PosY}, 0.0f, texScale, WHITE);

    if (type < 2) {
        DrawCenteredText(PosX, PosY, label, button, texScale);
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && type == 0 && IsHover()) {
        PlaySound(sound.press);
    }
    if (type == 1 || type == 2) {
        if (IsHoverRight()) {
            DrawTextureEx(MoveRight, {PosX + button.width * texScale + 20, y}, 0.0f, texScale * 1.2f, WHITE);
        } else {
            DrawTextureEx(MoveRight, {PosX + button.width * texScale + 20, y}, 0.0f, texScale * 1.0f, WHITE);
        }
        
        if (IsHoverLeft()) {
            DrawTextureEx(MoveLeft, {PosX - MoveLeft.width * texScale - 20, y}, 0.0f, texScale * 1.2f, WHITE);
        } else {
            DrawTextureEx(MoveLeft, {PosX - MoveLeft.width * texScale - 20, y}, 0.0f, texScale * 1.0f, WHITE);
        }

        if (type == 2) {
            DrawTextureEx(Banner, {PosX - MoveLeft.width * texScale - 20 - Banner.width * 0.3f - 20, y}, 0.0f, 0.3f, WHITE);
            DrawCenteredText(PosX - MoveLeft.width * texScale - 20 - Banner.width * 0.3f - 60, y + 15, label, button, 0.3f);
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && type == 1) {
            PlaySound(sound.press);
            if (IsHoverLeft()) {
                cnt = ((cnt - 1) + cyc) % cyc;
            } else if (IsHoverRight()){
                cnt = (cnt + 1) % cyc;
            }
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && type == 2) {
            PlaySound(sound.press);
            if (IsHoverLeft()) {
                cnt = (cnt - 1);
                if (cnt < 0)
                    cnt = 0;
            } else if (IsHoverRight()){
                cnt = (cnt + 1);
                if (cnt > 5)
                    cnt = 5;
            }

            if (label == "MUSIC") {
                PlayMusicBackGround.vol = cnt;
            }

            if (label == "SOUND") {
                SoundVol = cnt;
                SetSoundVolume(sound.press, 0.2f * cnt);
            }
        }
    }
}