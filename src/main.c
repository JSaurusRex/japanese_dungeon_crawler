#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <raylib.h>
#include <math.h>
#include <time.h>

#include "rendering.h"
#include "main.h"
#include "questions.h"
#include "battle.h"
#include "sprite_manager.h"
#include "next_level_screen.h"
#include "hub.h"


bool _printDebug = true;
Font _fontJapanese;

float _transition_timer = 1;

void (*_screen)() = &hub_frame;

void EmptyScreen();

void (*_next_screen)() = &hub_frame;

void change_screen(void (*next_screen)())
{
    _transition_timer = 1;
    _next_screen = next_screen;
}

void main() {
    SetWindowState(FLAG_MSAA_4X_HINT);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Japanese text test");
    srand(time(NULL));

    int total = (0x9FFF - 0x3040) + LATIN_UNICODE;

    // Allocate and fill codepoint list
    int *codepoints = malloc(total * sizeof(int));
    for(int i = 0; i < total; i++)
    {
        codepoints[i] = 0x3040 + (i - LATIN_UNICODE);
        if (i < LATIN_UNICODE)
            codepoints[i] = i;
    }

    // Load font with all Japanese glyphs
    _fontJapanese = LoadFontEx("data/fonts/NotoSansJP-Regular.ttf", 50, codepoints, total);

    free(codepoints);

    Questions_init();
    Battle_Init();
    sprite_manager_init();

    SetTargetFPS(60);
    while (!WindowShouldClose()) {

        if (IsKeyPressed(KEY_F1))
        {
            printf("%i, %i\n", GetMouseX(), GetMouseY());
        }

        if (IsKeyPressed(KEY_F10))
        {
            _printDebug = !_printDebug;
            if (_printDebug)
                printf("debugging printing enabled\n");
            else
                printf("debugging printing disabled\n");
        }

        BeginDrawing();
        
        (*_screen)();

        if (_transition_timer > 0.5)
        {
            _transition_timer -= GetFrameTime();
            if (_transition_timer <= 0.5)
            {
                _screen = _next_screen;
            }
        }else
        {
            _transition_timer -= GetFrameTime();

            if (_transition_timer < 0)
                _transition_timer = 0;
        }
        
        drawRectangle(0, 0, 800, 600, ColorAlpha(BLACK, sin(_transition_timer*PI)));

        EndDrawing();
    }

    UnloadFont(_fontJapanese);
    CloseWindow();
}

void EmptyScreen()
{
    ClearBackground(YELLOW);
}