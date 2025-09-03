#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <raylib.h>
#include <math.h>

#include "../battle.h"
#include "../particles.h"
#include "../shadows.h"

void EnemyRender(sEnemy * self, int position)
{
    Vector2 pos = CalculateEnemyPosition(self->lane, position);
    add_shadow((Vector2){pos.x+23, pos.y+50}, 0.8);

    if (self->shake_timer == 1)
    {
        emit_particles_explosion(PARTICLE_BLOOD, 1.5, (Vector2){pos.x + 20, pos.y + 20}, (Vector2){50, 50}, 90, 60, 8);
    }

    self->shake_timer -= GetFrameTime();
    if (self->shake_timer < 0)
        self->shake_timer = 0;

    pos.x += sin(self->shake_timer * 40) * 10 * self->shake_timer;

    if (self->attack_animation_timer > 0.5)
    {
        self->attack_animation_timer -= GetFrameTime();

        if (self->attack_animation_timer <= 0.5)
            take_damage(self->lane, ELEMENT_NONE, 20);
    }else
        self->attack_animation_timer -= GetFrameTime();

    if (self->attack_animation_timer < 0)
        self->attack_animation_timer = 0;

    pos.x -= pow(sin(self->attack_animation_timer * PI), 3) * 80;

    DrawRectangle(pos.x, pos.y, 50, 50, BROWN);
}

void EnemyTurn(sEnemy * self)
{
    printf("doing enemy turn!\n");

    self->attack_animation_timer = 1;

    next_turn();
}

void EnemyTakeDamage(sEnemy * self, float damage, Element element)
{
    printf("enemy took %.2f damage\n", damage);
    self->health -= damage;

    self->shake_timer = 1;

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