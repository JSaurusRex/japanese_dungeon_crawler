#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <raylib.h>

#include "main.h"
#include "questions.h"
#include "battle.h"
#include "sprite_manager.h"
#include "next_level_screen.h"

bool _printDebug = true;
Font _fontJapanese;

void (*_screen)() = &next_level_frame;

void EmptyScreen();

void main() {
    SetWindowState(FLAG_MSAA_4X_HINT);
    InitWindow(800, 600, "Japanese text test");

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

    srand(GetTime());

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

        EndDrawing();
    }

    UnloadFont(_fontJapanese);
    CloseWindow();
}

void EmptyScreen()
{
    ClearBackground(YELLOW);
}