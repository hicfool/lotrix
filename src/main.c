#include <stdlib.h>
#include <time.h>

#include "../lib/raylib.h"

#define LO(a, b) (a < b) ? a : b;
#define HI(a, b) (a > b) ? a : b;

int random_range(int min, int max) { return min + rand() % (max - min + 1); }

int main(int argc, char *argv[]) {
    const int WIDTH  = (argc > 1) ? atoi(argv[1]) : 128, HEIGHT = (argc > 2) ? atoi(argv[2]) : WIDTH;
    const int AMOUNT = (argc > 3) ? atoi(argv[3]) : HI(WIDTH * 2, HEIGHT * 2);
    const int TAIL   = (argc > 4) ? atoi(argv[4]) : 6;
    int paused       = 0;
    Color bg         = { 0, 0, 0, 255 };

    srand(time(NULL));

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WIDTH, HEIGHT, "lotrix");
    SetTargetFPS(60);
    RenderTexture2D surface = LoadRenderTexture(WIDTH, HEIGHT);
    SetTextureFilter(surface.texture, TEXTURE_FILTER_POINT);
    Font font = LoadFont("../src/textures/font.png");

    int column[WIDTH / 8];
    for (int i = 0; i < WIDTH / 8; i++) { column[i] = i * 8; }

    int line[HEIGHT / 8];
    for (int i = 0; i < HEIGHT / 8; i++) { line[i] = i * 8; }

    struct TXT {
        float x, y, delay;
        int delay_def;
        char type[2];
    }; struct TXT txt[AMOUNT];

    for (int i = 0; i < AMOUNT; i++) {
        txt[i].x         = column[random_range(0, WIDTH / 8)];
        txt[i].y         = -line[random_range(0, HEIGHT / 8)];
        txt[i].delay_def = random_range(1, 12);
        txt[i].delay     = 0;
        txt[i].type[0]   = random_range(32, 127);
        txt[i].type[1]   = '\0';
    }

    while (!WindowShouldClose()) {
        float scale = LO((float)GetScreenWidth() / WIDTH, (float)GetScreenHeight() / HEIGHT);
        float tw    = surface.texture.width;
        float th    = surface.texture.height;
        float sw    = GetScreenWidth();
        float sh    = GetScreenHeight();
        float w     = WIDTH  * scale;
        float h     = HEIGHT * scale;

        if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_P)) { paused = (!paused) ? 1 : 0; }

        BeginTextureMode(surface);

        if (!paused) {
            ClearBackground(bg);

            for (int i = 0; i < AMOUNT; i++) {
                for (int j = 1; j < TAIL + 1; j++) {
                    Vector2 pos = { txt[i].x, txt[i].y - 8 * j };
                    int c       = (j == 1) ? 200 : 0;
                    Color fg    = { c, 255, c, 255 / j };

                    DrawTextEx(font, txt[i].type, pos, 8, 0, fg);
                }

                if (txt[i].delay > 0) { txt[i].delay -= GetFrameTime() * 240; }
                else {
                    txt[i].y      += 8;
                    txt[i].type[0] = random_range(32, 127);
                    txt[i].delay   = txt[i].delay_def;
                }

                if (txt[i].y - 8 * TAIL > HEIGHT) {
                    txt[i].x = column[random_range(0, WIDTH / 8)];
                    txt[i].y = -line[random_range(0, HEIGHT / 8)];
                }
            }
        }

        EndTextureMode();

        BeginDrawing();

        DrawTexturePro(surface.texture,
            (Rectangle){ 0, 0, tw, -th },
            (Rectangle){ (sw - w) / 2, (sh - h) / 2, w, h },
            (Vector2){ 0 }, 0, WHITE);

        EndDrawing();
    }

    UnloadRenderTexture(surface);
    UnloadFont(font);
    CloseWindow();
    return 0;
}
