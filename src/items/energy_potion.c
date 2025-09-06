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

void Energy_potion_renderer(sItem * self, Vector2 position);
void Energy_potion_effect_player(sItem * self);

char Energy_potion_description [] = "Energy potion\nGives 30 energy\n";

sItem _prefab_energy_potion = {
    .active = true,
    .energy = -30,
    .effect_enemy = 0,
    .effect_player = &Energy_potion_effect_player,
    .render = &Energy_potion_renderer,
    .pack = "hiragana1",
    .level = 1,
    .description = Energy_potion_description
};

void Energy_potion_renderer(sItem * self, Vector2 position)
{
    drawTextureEx(_potion_liquid_sprite, position, 0, 0.1, RED);
    drawTextureEx(_potion_sprite, position, 0, 0.1, WHITE);
}

void Energy_potion_effect_player(sItem * self)
{
    consume_energy(self->energy);
    self->active = false;
}

