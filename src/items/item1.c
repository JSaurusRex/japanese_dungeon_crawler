#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <raylib.h>
#include <math.h>

#include "../battle.h"
#include "../questions.h"

void ItemRender(sItem * self, Vector2 position);
void Item_effect_enemy(sItem * self, sEnemy * pEnemy);

char item1_description [] = "Testing Item\nConsumes 35 energy\ndeals 5";

sItem _prefab_item1 = {
    .active = true,
    .energy = 35,
    .effect_enemy = &Item_effect_enemy,
    .render = &ItemRender,
    .description = item1_description
};

void ItemRender(sItem * self, Vector2 position)
{
    DrawRectangle(position.x, position.y, 50, 50, GRAY);
}

sEnemy * _pEnemy = 0;
sItem * _pItem = 0;

void Item_effect_enemy_finish()
{
    if (_answers_incorrect < _quiz_hearts)
        (*_pEnemy->takeDamage)(_pEnemy, 5, ELEMENT_NONE);
    _screen = &Battle_Frame;
    consume_energy(_prefab_item1.energy);
    try_return_item();
}

void Item_effect_enemy(sItem * self, sEnemy * pEnemy)
{
    if (!pEnemy)
    {
        printf("error: Item_effect_enemy: pEnemy is null\n");
        return;
    }

    if (!pEnemy->active)
    {
        if (_printDebug)
            printf("debug: Item_effect_enemy: clicked on inactive enemy\n");
        return;
    }

    _pEnemy = pEnemy;
    Start_Questions(5, 3, "hiragana", &Item_effect_enemy_finish);
}

