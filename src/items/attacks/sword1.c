#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <raylib.h>
#include <math.h>

#include "../../battle.h"
#include "../../questions.h"
#include "../../sprite_manager.h"

#include "generic.h"

void sword1_render(sItem * self, Vector2 position);
void sword1_effect_enemy(sItem * self, sEnemy * pEnemy);

char sword1_description [] = "Testing Item\nConsumes 35 energy\ndeals 5";

sItem _prefab_sword1 = {
    .active = true,
    .energy = 35,
    .effect_enemy = &sword1_effect_enemy,
    .effect_shield = 0,
    .effect_item = 0,
    .render = &sword1_render,
    .pack = "hiragana",
    .level = 1,
    .description = sword1_description
};

void sword1_render(sItem * self, Vector2 position)
{
    DrawTexture(_sword1_sprite, position.x, position.y, WHITE);
}

void sword1_effect_enemy_finish()
{
    if (quiz_succeeded())
        _pEnemy->takeDamage(_pEnemy, 5 * damage_factor_calc(40, 30, 1.2), ELEMENT_NONE);
    _screen = &Battle_Frame;
    try_return_item();
}

void sword1_effect_enemy(sItem * self, sEnemy * pEnemy)
{
    if (!enemy_sanity_checks(pEnemy))
        return;

    _pEnemy = pEnemy;
    _pItem = self;
    self->rounds_disabled = 2;
    consume_energy(self->energy);

    Start_Questions(5, 3, "hiragana", 1, &sword1_effect_enemy_finish);
}

