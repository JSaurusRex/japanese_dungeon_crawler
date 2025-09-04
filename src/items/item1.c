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
    .effect_shield = 0,
    .effect_item = 0,
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
    {
        float damage_factor = 1;
        if (_answers_incorrect == 0)
            damage_factor *= 1 + _pItem->enhanced;
        
        damage_factor *= 0.8 + (rand() % 40) * 0.01;

        if (_answers_incorrect == 0 && rand() % 5 == 0)
            damage_factor *= 1.2;

        (*_pEnemy->takeDamage)(_pEnemy, 5 * damage_factor, ELEMENT_NONE);
    }
    _screen = &Battle_Frame;
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
    _pItem = self;
    self->rounds_disabled = 2;
    consume_energy(self->energy);

    Start_Questions(5, 3, "hiragana", 1, &Item_effect_enemy_finish);
}

