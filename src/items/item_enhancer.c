#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <raylib.h>
#include "../rendering.h"
#include <math.h>

#include "../battle.h"
#include "../questions.h"

void Item_enhancer_renderer(sItem * self, Vector2 position);
void Item_enhancer_effect(sItem * self, sItem * pEnemy);

char item_enhancer_description [] = "Enhances item\nConsumes 10 energy\nEnhances 1.5 times\nOnly works if you\nanswered all\nquestions correctly!";

sItem _prefab_item_enhancer = {
    .active = true,
    .energy = 10,
    .effect_enemy = 0,
    .effect_shield = 0,
    .effect_item = &Item_enhancer_effect,
    .render = &Item_enhancer_renderer,
    .pack = "hiragana1",
    .level = 1,
    .description = item_enhancer_description
};

void Item_enhancer_renderer(sItem * self, Vector2 position)
{
    drawRectangle(position.x, position.y, 50, 50, GRAY);
}

void Item_enhancer_effect(sItem * self, sItem * pItem)
{
    if (!pItem)
    {
        printf("error: Item_enhancer_effect: pItem is null\n");
        return;
    }

    if (!pItem->active)
    {
        if (_printDebug)
            printf("debug: Item_enhancer_effect: clicked on inactive enemy\n");
        return;
    }

    pItem->enhanced = 0.5;
    self->active = false;
}

