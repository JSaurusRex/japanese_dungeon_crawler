#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <raylib.h>
#include <raymath.h>
#include <math.h>

#include "../battle.h"
#include "../particles.h"
#include "../shadows.h"
#include "../sprite_manager.h"

#include "generic.h"

void Goblin1Render(sEnemy * self, int position)
{
    Vector2 pos = CalculateEnemyPosition(self->lane, position);
    add_shadow((Vector2){pos.x+23, pos.y+50}, 0.8);

    pos.x += shake_manager(&self->shake_timer);

    if (do_at_timer(&self->attack_animation_timer, 0.5))
    {
        take_damage(self->lane, ELEMENT_NONE, 20);
        next_turn();
    }

    pos.x -= move_forward_animation(self->attack_animation_timer, 80);

    DrawTexture(_goblin1_sprite, pos.x, pos.y, WHITE);
    self->lastPosition = pos;
}

void Goblin1Turn(sEnemy * self)
{
    printf("doing enemy turn!\n");

    self->attack_animation_timer = 1;
}

void Goblin1TakeDamage(sEnemy * self, float damage, Element element)
{
    if (_printDebug)
        printf("debug: EnemyTakeDamage: enemy took %.2f damage\n", damage);

    apply_damage_enemy(self, element, damage);
}

char goblin1_description[] = "goblin1\n10 health\nDeals 20 dmg";

sEnemy _prefab_goblin1 = {
    .active = true,
    .health = 10,
    .lane = 0,
    .render = &Goblin1Render,
    .takeDamage = &Goblin1TakeDamage,
    .turn = &Goblin1Turn, 
    .description = goblin1_description
};