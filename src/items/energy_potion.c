#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <raylib.h>
#include <math.h>

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
    .pack = "hiragana",
    .level = 1,
    .description = Energy_potion_description
};

void Energy_potion_renderer(sItem * self, Vector2 position)
{
    DrawRectangle(position.x, position.y, 50, 50, GREEN);
}

void Energy_potion_effect_player(sItem * self)
{
    consume_energy(self->energy);
    self->active = false;
}

