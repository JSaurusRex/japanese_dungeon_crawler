#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <raylib.h>
#include <math.h>

#include "main.h"
#include "battle.h"
#include "particles.h"
#include "shadows.h"
#include "next_level_screen.h"
#include "sprite_manager.h"

void hub_frame()
{
    DrawTexture(_hub_screen, 0, 0, WHITE);

    //enter button
    {
        Vector2 pos = (Vector2){ 640, 100};
        DrawTextEx(_fontJapanese, "enter", pos, 30, 2, BLACK);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (pos.x < GetMouseX() && pos.x + 80 > GetMouseX())
            {
                if(pos.y < GetMouseY() && pos.y + 40 > GetMouseY())
                {
                    printf("entered!\n");
                    Battle_Reset();
                    next_level_generate();
                }
            }
        }
    }
}