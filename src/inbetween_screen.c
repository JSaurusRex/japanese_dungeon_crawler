#include <stdlib.h>
#include <stdio.h>

#include <raylib.h>
#include "rendering.h"

#include "sprite_manager.h"
#include "battle.h"

float _inbetween_timer = 0;

void inbetween_screen_frame()
{
    drawTexture(_inbetween_screen, 0, 0, WHITE);


    //draw level numbers
    {
        char str[STRING_LENGTH];
        snprintf(str, STRING_LENGTH, "Level %i", _level);
        drawTextEx(_fontJapanese, str, (Vector2){ 300, 200 }, 70, 2, WHITE);
    }

    if (_inbetween_timer <= 2)
    {
        _inbetween_timer += GetFrameTime();

        if (_inbetween_timer > 2)
            Battle_Start();
    }
}