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

#define CHANGE_NAME(y) shield_repair ## _ ## y

void CHANGE_NAME(renderer)(sItem * self, Vector2 position);
void CHANGE_NAME(effect_shield)(sItem * self, sShield * pShield);

char CHANGE_NAME(description) [] = "Shield repair item\nConsumes 15 energy\nrepairs up to 50";

sItem _prefab_shield_repair_item = {
    .active = true,
    .energy = 15,
    .effect_enemy = 0,
    .effect_enemy = 0,
    .effect_shield = &CHANGE_NAME(effect_shield), 
    .render = &CHANGE_NAME(renderer),
    .pack = "hiragana1",
    .level = 1,
    .description = CHANGE_NAME(description)
};

void CHANGE_NAME(renderer)(sItem * self, Vector2 position)
{
    drawTextureEx(_shield_repair_sprite, position, 0, 0.1, WHITE);
}

void CHANGE_NAME(effect_shield)(sItem * self, sShield * pShield)
{
    if (!pShield)
    {
        printf("error: effect_shield: pShield is null\n");
        return;
    }

    if (!pShield->active)
    {
        if (_printDebug)
            printf("debug: effect_shield: clicked on inactive shield\n");
        return;
    }

    consume_energy(self->energy);

    pShield->health += 50;
    if (pShield->health > pShield->max_health)
        pShield->health = pShield->max_health;
    
    self->active = false;
}

