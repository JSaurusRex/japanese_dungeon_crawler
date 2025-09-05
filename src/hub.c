#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <raylib.h>
#include "rendering.h"
#include <math.h>

#include "main.h"
#include "battle.h"
#include "particles.h"
#include "shadows.h"
#include "next_level_screen.h"
#include "sprite_manager.h"

void hub_frame()
{
    drawTexture(_hub_screen, 0, 0, WHITE);

    //enter button
    {
        Vector2 pos = (Vector2){ 640, 100};
        drawTextEx(_fontJapanese, "enter", pos, 30, 2, BLACK);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (pos.x < getMouseX() && pos.x + 80 > getMouseX())
            {
                if(pos.y < getMouseY() && pos.y + 40 > getMouseY())
                {
                    printf("entered!\n");
                    Battle_Reset();
                    next_level_generate();
                }
            }
        }
    }

    //draw highscoer
    {
        Vector2 pos = (Vector2){ 280, 100};
        char str[STRING_LENGTH];
        snprintf(str, STRING_LENGTH, "highscore %i", _highscore);
        drawTextEx(_fontJapanese, str, pos, 40, 2, BLACK);
    }
}