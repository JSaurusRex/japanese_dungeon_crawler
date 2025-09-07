#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <raylib.h>
#include "../rendering.h"
#include <raymath.h>
#include <math.h>

#include "../battle.h"
#include "../particles.h"
#include "../shadows.h"
#include "../sprite_manager.h"

#include "generic.h"

#define CHANGE_NAME(y) Goblin1 ## _ ## y

void CHANGE_NAME (Render)(sEnemy * self, int position)
{
    Vector2 pos = CalculateEnemyPosition(self->lane, position);
    add_shadow((Vector2){pos.x+23, pos.y+50}, 0.8, true);

    pos.x += shake_manager(&self->shake_timer);

    if (do_at_timer(&self->attack_animation_timer, 0.5))
    {
        take_damage(self->lane, ELEMENT_NONE, 20);
        next_turn();
    }

    pos.x -= move_forward_animation(self->attack_animation_timer, 80);

    drawTextureEx(_goblin1_sprite, pos, 0, 0.1, WHITE);
    self->lastPosition = pos;
}

void CHANGE_NAME (Turn)(sEnemy * self)
{
    printf("doing enemy turn!\n");

    self->attack_animation_timer = 1;
}

void CHANGE_NAME (TakeDamage)(sEnemy * self, float damage, Element element)
{
    if (element == ELEMENT_FIRE)
        damage *= 1.4;
    
    if (_printDebug)
        printf("debug: EnemyTakeDamage: enemy took %.2f damage\n", damage);

    apply_damage_enemy(self, element, damage, element == ELEMENT_FIRE);
    
    if (self->health <= 0)
        self->active = false;
}

char CHANGE_NAME(description)[] = "goblin1\n10 health\nDeals 20 dmg\nWeak to Fire";

sEnemy _prefab_goblin1 = {
    .active = true,
    .health = 10,
    .lane = 0,
    .render = &CHANGE_NAME (Render),
    .takeDamage = &CHANGE_NAME (TakeDamage),
    .turn = &CHANGE_NAME (Turn), 
    .description = CHANGE_NAME(description)
};