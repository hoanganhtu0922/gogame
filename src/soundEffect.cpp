#include "soundEffect.h"

soundEffect::soundEffect() {
    place_stone = LoadSound("../../assets/sound/place_stone.wav");
    win_some_stones = LoadSound("../../assets/sound/win_some_stones.wav");
    error = LoadSound("../../assets/sound/error.wav");
    press = LoadSound("../../assets/sound/press.wav");
}
