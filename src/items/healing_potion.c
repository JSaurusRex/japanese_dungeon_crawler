#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <raylib.h>
#include "../rendering.h"
#include <math.h>

#include "../battle.h"
#include "../main.h"

void Healing_potion_renderer(sItem * self, Vector2 position);
void Healing_potion_effect_player(sItem * self);

char healing_potion_description [] = "healing potion\nConsumes 15 energy\nHeals up to 40";

sItem _prefab_healing_potion = {
    .active = true,
    .energy = 15,
    .effect_enemy = 0,
    .effect_player = &Healing_potion_effect_player,
    .render = &Healing_potion_renderer,
    .pack = "hiragana1",
    .level = 1,
    .description = healing_potion_description
};

void Healing_potion_renderer(sItem * self, Vector2 position)
{
    drawRectangle(position.x, position.y, 50, 50, GREEN);
}

void Healing_potion_effect_player(sItem * self)
{
    consume_energy(self->energy);

    heal_player(40);

    self->active = false;
}

