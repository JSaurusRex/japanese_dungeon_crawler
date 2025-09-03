#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <raylib.h>
#include <math.h>

#include "../battle.h"

void EnemyRender(sEnemy * self, int position)
{
    Vector2 pos = CalculateEnemyPosition(self->lane, position);
    DrawRectangle(pos.x, pos.y, 50, 50, BROWN);
}

void EnemyTurn(sEnemy * self)
{
    printf("doing enemy turn!\n");

    take_damage(self->lane, ELEMENT_NONE, 20);
    next_turn();
}

void EnemyTakeDamage(sEnemy * self, float damage, Element element)
{
    printf("enemy took %.2f damage\n", damage);
    self->health -= damage;
    if (self->health <= 0)
        self->active = false;
}

char enemy1_description[] = "enemy1\n10 health\nDeals 20 dmg";

sEnemy _prefab_enemy1 = {
    .active = true,
    .health = 10,
    .lane = 0,
    .render = &EnemyRender,
    .takeDamage = &EnemyTakeDamage,
    .turn = &EnemyTurn, 
    .description = enemy1_description
};