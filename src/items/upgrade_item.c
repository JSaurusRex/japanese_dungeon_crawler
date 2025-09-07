#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <raylib.h>
#include "../rendering.h"
#include <math.h>

#include "../sprite_manager.h"
#include "../battle.h"
#include "../questions.h"

#define CHANGE_NAME(y) upgrade_item ## _ ## y

void CHANGE_NAME(renderer)(sItem * self, Vector2 position);
void CHANGE_NAME(effect)(sItem * self, sItem * pEnemy);

char CHANGE_NAME(description) [] = "upgrade item\nupgrades 1 level\n";

sItem _prefab_upgrade_item = {
    .active = true,
    .energy = 10,
    .effect_enemy = 0,
    .effect_shield = 0,
    .effect_item = &CHANGE_NAME(effect),
    .render = &CHANGE_NAME(renderer),
    .pack = "hiragana1",
    .level = 1,
    .level_cap = 1,
    .description = CHANGE_NAME(description)
};

void CHANGE_NAME(renderer)(sItem * self, Vector2 position)
{
    drawTextureEx(_item_upgrade_sprite, position, 0, 0.1, WHITE);
}

void CHANGE_NAME(effect_shield)(sItem * self, sShield * pShield)
{
    if (!pShield)
    {
        printf("error: effect shield: pShield is null\n");
        return;
    }

    if (!pShield->active)
    {
        if (_printDebug)
            printf("debug: effect shield: clicked on inactive shield\n");
        return;
    }

    if (pShield->level >= pShield->level_cap)
        return;
        
    pShield->level++;

    self->active = false;
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
            printf("debug: effect: clicked on inactive item\n");
        return;
    }

    int max_level = GetMaxLevel(pItem->pack);
    if (pItem->level >= max_level)
        return;

    if (pItem->level >= pItem->level_cap && pItem->level_cap != 0)
        return;

    pItem->level++;

    self->active = false;
}

