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

#define CHANGE_NAME(y) sword1 ## _ ## y

void CHANGE_NAME(render)(sItem * self, Vector2 position);
void CHANGE_NAME(effect_enemy)(sItem * self, sEnemy * pEnemy);

char CHANGE_NAME(description) [] = "Sword\nConsumes 35 energy\ndeals 5";

sItem _prefab_sword1 = {
    .active = true,
    .energy = 35,
    .effect_enemy = &CHANGE_NAME(effect_enemy),
    .effect_shield = 0,
    .effect_item = 0,
    .render = &CHANGE_NAME(render),
    .pack = "hiragana1",
    .level = 1,
    .description = CHANGE_NAME(description)
};

void CHANGE_NAME(render)(sItem * self, Vector2 position)
{
    drawTextureEx(_sword1_sprite, position, 0, 0.1, WHITE);
}

void CHANGE_NAME(apply_effect_enemy)()
{
    if (quiz_succeeded())
        _pEnemy->takeDamage(_pEnemy, 5 * damage_factor_calc(40, 30, 1.2), ELEMENT_NONE);
}

void CHANGE_NAME(effect_enemy_finish)()
{
    change_screen(&Battle_Frame, true);
    try_return_item();

    //slightly delay showing damage being applied
    _delayed_callback = &CHANGE_NAME(apply_effect_enemy);
    _delayed_callback_timer = 1;
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

