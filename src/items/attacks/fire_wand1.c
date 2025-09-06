#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <raylib.h>
#include "../../rendering.h"
#include <math.h>

#include "../../battle.h"
#include "../../questions.h"
#include "../../sprite_manager.h"

#include "generic.h"

#define CHANGE_NAME(y) firewand1 ## _ ## y

void CHANGE_NAME(render)(sItem * self, Vector2 position);
void CHANGE_NAME(effect_enemy)(sItem * self, sEnemy * pEnemy);

char CHANGE_NAME(description) [] = "FireWand\nConsumes 35 energy\nDeals 5";

sItem _prefab_firewand1 = {
    .active = true,
    .energy = 35,
    .effect_enemy = &CHANGE_NAME(effect_enemy),
    .effect_shield = 0,
    .effect_item = 0,
    .render = &CHANGE_NAME(render),
    .pack = "animals",
    .level = 1,
    .description = CHANGE_NAME(description)
};

void CHANGE_NAME(render)(sItem * self, Vector2 position)
{
    drawTextureEx(_firewand1_sprite, position, 0, 0.1, WHITE);
}

void CHANGE_NAME(effect_enemy_finish)()
{
    if (quiz_succeeded())
        _pEnemy->takeDamage(_pEnemy, 5 * _pItem->level * damage_factor_calc(40, 30, 1.2), ELEMENT_FIRE);

    change_screen(&Battle_Frame);
    try_return_item();
}

void CHANGE_NAME(effect_enemy)(sItem * self, sEnemy * pEnemy)
{
    if (!enemy_sanity_checks(pEnemy))
        return;

    _pEnemy = pEnemy;
    _pItem = self;
    self->rounds_disabled = 2;
    consume_energy(self->energy);

    Start_Questions(5, 3, self->pack, self->level, &CHANGE_NAME(effect_enemy_finish));
}

