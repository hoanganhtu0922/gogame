//Library 
#include "IMG.h"

IMG::IMG() {
    Image img = LoadImage("../../image/black.png");
    ImageFormat(&img, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    remove_background(img);
    Black = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImage("../../image/white.png");
    ImageFormat(&img, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    remove_background(img);
    White = LoadTextureFromImage(img);
    UnloadImage(img);

    Image undo_button = LoadImage("../../image/undo.jpg");
    ImageFormat(&undo_button, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    remove_background(undo_button);
    UndoTex = LoadTextureFromImage(undo_button);
    UnloadImage(undo_button);

    img = LoadImage("../../image/redo.png");
    ImageFormat(&img, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    RemoveBlack(img);
    RedoButton = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImage("../../image/skip.png");
    ImageFormat(&img, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    RemoveBlack(img);
    SkipButton = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImage("../../image/exit.jpg");
    ImageFormat(&img, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    RemoveBlack(img);
    ExitButton = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImage("../../image/gg.png");
    ImageFormat(&img, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    RemoveBlack(img);
    GG = LoadTextureFromImage(img);
    UnloadImage(img);
}

void IMG::remove_background(Image &to_remove) {
    int H = to_remove.height;
    int W = to_remove.width;

    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            Color c = GetImageColor(to_remove, x, y);
            if (c.r >= 237 && c.g >= 237 && c.b >= 237) {
                ImageDrawPixel(&to_remove, x, y, {0, 0, 0, 0});
            }
        }
    }
}

void IMG::RemoveBlack(Image &to_remove) {
    int H = to_remove.height;
    int W = to_remove.width;

    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            Color c = GetImageColor(to_remove, x, y);
            if (c.r <= 20 && c.g <= 20 && c.b <= 20) {
                ImageDrawPixel(&to_remove, x, y, {0, 0, 0, 0});
            }
        }
    }
}
