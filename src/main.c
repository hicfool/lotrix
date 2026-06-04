#include <stdlib.h>
#include <raylib.h>

#define LO(a, b) (a < b) ? a : b;
#define HI(a, b) (a > b) ? a : b;

int random_range(int min, int max) { return min + rand() % (max - min + 1); }

int main(int argc, char *argv[]) {
    const int WIDTH = (argc > 1) ? atoi(argv[1]) : 128, HEIGHT = (argc > 2) ? atoi(argv[2]) : WIDTH;
    bool paused = false;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(256, 256, "lotrix");
    SetTargetFPS(60);
    RenderTexture2D surface = LoadRenderTexture(WIDTH, HEIGHT);
    SetTextureFilter(surface.texture, TEXTURE_FILTER_POINT);
    Font font = LoadFont("../src/textures/font.png");

    const int AMOUNT = (argc > 3) ? atoi(argv[3]) : HI(WIDTH / 2, HEIGHT / 2);
    const int TAIL   = (argc > 4) ? atoi(argv[4]) : 6;

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
        float surf_scale = LO((float)GetScreenWidth() / WIDTH, (float)GetScreenHeight() / HEIGHT);
        if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_P)) { paused = (!paused) ? true : false; }
        if (IsKeyPressed(KEY_F)) { ToggleBorderlessWindowed(); }

        BeginTextureMode(surface);
            if (!paused) {
                ClearBackground(BLACK);

                for (int i = 0; i < AMOUNT; i++) {
                    DrawTextEx(font, txt[i].type, (Vector2) { txt[i].x, txt[i].y }, 8, 0, WHITE);

                    for (int j = 1; j < TAIL; j++) {
                        DrawTextEx(font, txt[i].type, (Vector2) { txt[i].x, txt[i].y - 8 * j }, 8, 0, (Color) { 0, 255, 0, 255 / j });
                    }

                    if (txt[i].delay > 0) { txt[i].delay -= GetFrameTime() * 240; }
                    else {
                        txt[i].y += 8;
                        txt[i].type[0] = random_range(32, 127);
                        txt[i].delay = txt[i].delay_def;
                    }

                    if (txt[i].y - 8 * TAIL > HEIGHT) {
                        txt[i].x = column[random_range(0, WIDTH / 8)];
                        txt[i].y = -line[random_range(0, HEIGHT / 8)];
                    }
                }
            }
        EndTextureMode();

        BeginDrawing();
            DrawTexturePro(surface.texture, (Rectangle) { 0, 0, (float)surface.texture.width, (float)-surface.texture.height }, (Rectangle) { (GetScreenWidth() - (float)WIDTH * surf_scale) / 2, (GetScreenHeight() - ((float)HEIGHT * surf_scale)) / 2, (float)WIDTH * surf_scale, (float)HEIGHT * surf_scale }, (Vector2) { 0, 0 }, 0, WHITE);
        EndDrawing();
    }

    UnloadRenderTexture(surface);
    UnloadFont(font);
    CloseWindow();
    return 0;
}
