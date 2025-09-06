#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <raylib.h>
#include "../rendering.h"
#include <math.h>

#include "../sprite_manager.h"
#include "../battle.h"
#include "../main.h"

#define CHANGE_NAME(y) healing_potion ## _ ## y

void CHANGE_NAME(renderer)(sItem * self, Vector2 position);
void CHANGE_NAME(effect_player)(sItem * self);

char CHANGE_NAME(description) [] = "healing potion\nConsumes 15 energy\nHeals up to 40";

sItem _prefab_healing_potion = {
    .active = true,
    .energy = 15,
    .effect_enemy = 0,
    .effect_player = &CHANGE_NAME(effect_player),
    .render = &CHANGE_NAME(renderer),
    .pack = "hiragana1",
    .level = 1,
    .description = CHANGE_NAME(description)
};

void CHANGE_NAME(renderer)(sItem * self, Vector2 position)
{
    drawTextureEx(_potion_liquid_sprite, position, 0, 0.1, RED);
    drawTextureEx(_potion_sprite, position, 0, 0.1, WHITE);
}

void CHANGE_NAME(effect_player)(sItem * self)
{
    consume_energy(self->energy);

    heal_player(40);

    self->active = false;
}

