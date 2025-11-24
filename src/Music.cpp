#pragma once;
#include "Music.h"

void MusicBackGround::setinitial() {
    bgm = LoadMusicStream("../../assets/sound/themesong.mp3");
    PlayMusicStream(bgm);
    SetMusicVolume(bgm, 0.4f); 
}

void MusicBackGround::Run() {
    UpdateMusicStream(bgm);
    SetMusicVolume(bgm, 0.2f * vol);
}