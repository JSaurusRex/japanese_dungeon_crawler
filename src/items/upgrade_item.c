#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <raylib.h>
#include "../rendering.h"
#include <math.h>

#include "../battle.h"
#include "../questions.h"

void upgrade_item_renderer(sItem * self, Vector2 position);
void upgrade_item_effect(sItem * self, sItem * pEnemy);

char upgrade_item_description [] = "upgrade item\nupgrades 1 level\n";

sItem _prefab_upgrade_item = {
    .active = true,
    .energy = 10,
    .effect_enemy = 0,
    .effect_shield = 0,
    .effect_item = &upgrade_item_effect,
    .render = &upgrade_item_renderer,
    .pack = "hiragana",
    .level = 1,
    .description = upgrade_item_description
};

void upgrade_item_renderer(sItem * self, Vector2 position)
{
    drawRectangle(position.x, position.y, 50, 50, GRAY);
}

void upgrade_item_effect_shield(sItem * self, sShield * pShield)
{
    if (!pShield)
    {
        printf("error: upgrade_item_effect: pShield is null\n");
        return;
    }

    if (!pShield->active)
    {
        if (_printDebug)
            printf("debug: upgrade_item_effect: clicked on inactive shield\n");
        return;
    }

    pShield->level++;
    self->active = false;
}

void upgrade_item_effect(sItem * self, sItem * pItem)
{
    if (!pItem)
    {
        printf("error: upgrade_item_effect: pItem is null\n");
        return;
    }

    if (!pItem->active)
    {
        if (_printDebug)
            printf("debug: upgrade_item_effect: clicked on inactive item\n");
        return;
    }

    pItem->level++;
    self->active = false;
}

