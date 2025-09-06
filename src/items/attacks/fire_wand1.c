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

void firewand1_render(sItem * self, Vector2 position);
void firewand1_effect_enemy(sItem * self, sEnemy * pEnemy);

char firewand1_description [] = "FireWand\nConsumes 35 energy\nDeals 5";

sItem _prefab_firewand1 = {
    .active = true,
    .energy = 35,
    .effect_enemy = &firewand1_effect_enemy,
    .effect_shield = 0,
    .effect_item = 0,
    .render = &firewand1_render,
    .pack = "words1",
    .level = 1,
    .description = firewand1_description
};

void firewand1_render(sItem * self, Vector2 position)
{
    drawTexture(_firewand1_sprite, position.x, position.y, WHITE);
}

void firewand1_effect_enemy_finish()
{
    if (quiz_succeeded())
        _pEnemy->takeDamage(_pEnemy, 5 * _pItem->level * damage_factor_calc(40, 30, 1.2), ELEMENT_FIRE);

    change_screen(&Battle_Frame);
    try_return_item();
}

void firewand1_effect_enemy(sItem * self, sEnemy * pEnemy)
{
    if (!enemy_sanity_checks(pEnemy))
        return;

    _pEnemy = pEnemy;
    _pItem = self;
    self->rounds_disabled = 2;
    consume_energy(self->energy);

    Start_Questions(5, 3, self->pack, self->level, &firewand1_effect_enemy_finish);
}

