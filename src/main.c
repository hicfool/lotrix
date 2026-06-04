#include <raylib.h>
#include <stdlib.h>
#include <string.h>

#define LO(a, b) (a < b) ? a : b
#define HI(a, b) (a > b) ? a : b
#define WWIDTH 600
#define WHEIGHT 600

int genRand(int min, int max) { return min + rand() % (max - min + 1); }

int main(int argc, char* argv[]) {
    const int RWIDTH = (argc > 1) ? atoi(argv[1]) : 128, RHEIGHT = (argc > 2) ? atoi(argv[2]) : RWIDTH;
    bool paused = false;
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(WWIDTH, WHEIGHT, "Lotrix");
	SetTargetFPS(60);
	RenderTexture2D surface = LoadRenderTexture(RWIDTH, RHEIGHT);
	SetTextureFilter(surface.texture, TEXTURE_FILTER_POINT);
    Font font = LoadFont("font.png");

    struct Grid { int x[RWIDTH / 8], y[RHEIGHT / 8]; }; struct Grid grid;
    for (int i = 0; i < RWIDTH / 8; i++) { grid.x[i] = i * 8; }
    for (int i = 0; i < RHEIGHT / 8; i++) { grid.y[i] = i * 8; }

    const int MAX = (argc > 3) ? atoi(argv[3]) : HI(RWIDTH / 4, RHEIGHT / 4);

    struct C {
        float x, y, speed;
        char type[2];
        int r, g, b, SPD;
    }; struct C c[MAX];

    for (int i = 0; i < MAX; i++) {
        c[i].x = grid.x[genRand(0, RWIDTH / 8)];
        c[i].y = grid.y[genRand(0, RHEIGHT / 8)] - RHEIGHT;
        c[i].SPD = genRand(1, 12);
        c[i].type[0] = genRand(32, 127);
        c[i].type[1] = '\0';

        if (argc > 4 && strcmp(argv[4], "true") == 0) {
            c[i].r = genRand(50, 255);
            c[i].g = genRand(50, 255);
            c[i].b = genRand(50, 255);
        } else {
            c[i].r = 0;
            c[i].g = 255;
            c[i].b = 0;
        }
    }

	while (!WindowShouldClose()) {
        float surfaceScale = LO((float)GetScreenWidth() / RWIDTH, (float)GetScreenHeight() / RHEIGHT);
        if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_P)) { paused = (!paused) ? true : false; }
        if (IsKeyPressed(KEY_F)) { ToggleBorderlessWindowed(); }
        HideCursor();

        BeginTextureMode(surface);
            if (!paused) {
                ClearBackground((Color) { 0, 0, 0, 24 });

                for (int i = 0; i < MAX; i++) {
                    DrawRectangle(c[i].x, c[i].y, 7, 7, (Color) { 0, 0, 0, 255 });
                    DrawRectangle(c[i].x, c[i].y - 8, 7, 7, (Color) { 0, 0, 0, 255 });
                    DrawRectangle(c[i].x, c[i].y, 7, 7, (Color) { c[i].r, c[i].g, c[i].b, 75 });
                    DrawTextEx(font, c[i].type, (Vector2) { c[i].x, c[i].y - 8 }, 8, 0, (Color) { c[i].r, c[i].g, c[i].b, 255 });
                    DrawTextEx(font, c[i].type, (Vector2) { c[i].x, c[i].y }, 8, 0, WHITE);

                    if (c[i].speed > 0) { c[i].speed -= GetFrameTime() * 240; }
                    else {
                        c[i].y += 8;
                        c[i].type[0] = genRand(32, 127);
                        c[i].speed = c[i].SPD;

                        if (c[i].y - 8 > RHEIGHT) {
                            c[i].x = grid.x[genRand(0, RWIDTH / 8)];
                            c[i].y = grid.y[genRand(0, RHEIGHT / 8)] - RHEIGHT;
                        }
                    }
                }

                if (argc > 5 && strlen(argv[5]) > 0) {
                    DrawRectangle(RWIDTH / 2 - ((strlen(argv[5]) / 2) * 8) - 8, RHEIGHT / 2 - 16, strlen(argv[5]) * 8 + 16, 24, BLACK);
                    DrawTextEx(font, argv[5], (Vector2) { RWIDTH / 2 - ((strlen(argv[5]) / 2) * 8), RHEIGHT / 2 - 8 }, 8, 0, WHITE);
                }
            } else { ClearBackground((Color) { 0, 0, 0, 0 }); }
        EndTextureMode();

        BeginDrawing();
            DrawTexturePro(surface.texture, (Rectangle) { 0, 0, (float)surface.texture.width, (float)-surface.texture.height }, (Rectangle) { (GetScreenWidth() - (float)RWIDTH * surfaceScale) / 2, (GetScreenHeight() - ((float)RHEIGHT * surfaceScale)) / 2, (float)RWIDTH * surfaceScale, (float)RHEIGHT * surfaceScale }, (Vector2) { 0, 0 }, 0, WHITE);
            DrawRectangle(0, 0, GetScreenWidth(), (GetScreenHeight() - RHEIGHT * surfaceScale) / 2, BLACK);
            DrawRectangle(0, GetScreenHeight() - (GetScreenHeight() - RHEIGHT * surfaceScale) / 2, GetScreenWidth(), GetScreenHeight(), BLACK);
            DrawRectangle(0, 0, (GetScreenWidth() - RWIDTH * surfaceScale) / 2, GetScreenHeight(), BLACK);
            DrawRectangle(GetScreenWidth() - (GetScreenWidth() - RWIDTH * surfaceScale) / 2, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
        EndDrawing();
	}

	UnloadRenderTexture(surface);
    UnloadFont(font);
	CloseWindow();
	return 0;
}
