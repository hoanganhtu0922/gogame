#include "soundEffect.h"

soundEffect::soundEffect() {
    place_stone = LoadSound("C:/Users/HOANG ANH/minifolder/cs160/Project/sound/place_stone.wav");
    win_some_stones = LoadSound("C:/Users/HOANG ANH/minifolder/cs160/Project/sound/win_some_stones.wav");
    error = LoadSound("C:/Users/HOANG ANH/minifolder/cs160/Project/sound/error.wav");
    press = LoadSound("C:/Users/HOANG ANH/minifolder/cs160/Project/sound/press.wav");
}
