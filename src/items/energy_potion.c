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

#define CHANGE_NAME(y) energy_potion ## _ ## y

void CHANGE_NAME(renderer)(sItem * self, Vector2 position);
void CHANGE_NAME(effect_player)(sItem * self);

char CHANGE_NAME(description) [] = "Energy potion\nGives 30 energy\n";

sItem _prefab_energy_potion = {
    .active = true,
    .energy = -30,
    .effect_enemy = 0,
    .effect_player = &CHANGE_NAME(effect_player),
    .render = &CHANGE_NAME(renderer),
    .pack = "hiragana1",
    .level = 1,
    .level_cap = 1,
    .description = CHANGE_NAME(description)
};

void CHANGE_NAME(renderer)(sItem * self, Vector2 position)
{
    drawTextureEx(_potion_liquid_sprite, position, 0, 0.1, GREEN);
    drawTextureEx(_potion_sprite, position, 0, 0.1, WHITE);
}

void CHANGE_NAME(effect_player)(sItem * self)
{
    consume_energy(self->energy);
    self->active = false;
}

