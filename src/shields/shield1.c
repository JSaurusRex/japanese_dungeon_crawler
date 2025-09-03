#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <raylib.h>
#include <math.h>

#include "../battle.h"

void ShieldRender(sShield * self, Vector2 position);
void ShieldTakeDamage(sShield * self, sEnemy * pEnemy, Element element, float damage);

char shield1_description [] = "Testing Shield\n40 health";

sShield _prefab_shield1 = {
    .active = true,
    .health = 40,
    .render = &ShieldRender,
    .take_damage = &ShieldTakeDamage,
    .description = shield1_description
};

void ShieldRender(sShield * self, Vector2 position)
{
    DrawRectangle(position.x, position.y, 50, 50, DARKGRAY);
}

void ShieldTakeDamage(sShield * self, sEnemy * pEnemy, Element element, float damage)
{
    self->health -= damage;
}
