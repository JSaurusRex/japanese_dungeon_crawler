#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <raylib.h>
#include "../rendering.h"
#include <math.h>

#include "../battle.h"
#include "../questions.h"

#define CHANGE_NAME(y) item_enhancer ## _ ## y

void CHANGE_NAME(renderer)(sItem * self, Vector2 position);
void CHANGE_NAME(effect)(sItem * self, sItem * pEnemy);

char CHANGE_NAME(description) [] = "Enhances item\nConsumes 10 energy\nEnhances 1.5 times\nOnly works if you\nanswered all\nquestions correctly!";

sItem _prefab_item_enhancer = {
    .active = true,
    .energy = 10,
    .effect_enemy = 0,
    .effect_shield = 0,
    .effect_item = &CHANGE_NAME(effect),
    .render = &CHANGE_NAME(renderer),
    .pack = "hiragana1",
    .level = 1,
    .description = CHANGE_NAME(description)
};

void CHANGE_NAME(renderer)(sItem * self, Vector2 position)
{
    drawRectangle(position.x, position.y, 50, 50, GRAY);
}

void CHANGE_NAME(effect)(sItem * self, sItem * pItem)
{
    if (!pItem)
    {
        printf("error: effect: pItem is null\n");
        return;
    }

    if (!pItem->active)
    {
        if (_printDebug)
            printf("debug: effect: clicked on inactive enemy\n");
        return;
    }

    pItem->enhanced = 0.5;
    self->active = false;
}

