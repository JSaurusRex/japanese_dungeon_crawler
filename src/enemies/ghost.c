#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <raylib.h>
#include "../rendering.h"
#include <math.h>

#include "../battle.h"
#include "../particles.h"
#include "../shadows.h"
#include "../sprite_manager.h"

#include "generic.h"

void GhostRender(sEnemy * self, int position)
{
    Vector2 pos = CalculateEnemyPosition(self->lane, position);
    // add_shadow((Vector2){pos.x+23, pos.y+50}, 0.8, true);
    //do ghosts have shadows?

    pos.x += shake_manager(&self->shake_timer);

    if (do_at_timer(&self->attack_animation_timer, 0.5))
    {
        take_damage(self->lane, ELEMENT_NONE, 5);
        next_turn();
    }

    pos.x -= move_forward_animation(self->attack_animation_timer, 80);

    pos.y += sin(_battle_timer * 0.6 + position + self->lane) * 10;

    drawTextureEx(_ghost_sprite, pos, 0, 0.1, WHITE);
    self->lastPosition = pos;
}

void GhostTurn(sEnemy * self)
{
    printf("doing ghost turn!\n");

    self->attack_animation_timer = 1;
}

void GhostTakeDamage(sEnemy * self, float damage, Element element)
{
    if (_printDebug)
        printf("debug: GhostTakeDamage: ghost took %.2f damage\n", damage);

    apply_damage_enemy(self, element, damage, false);
}

char ghost_description[] = "Spooky ghost\n25 health\nDeals 5 dmg\nScares easily";

sEnemy _prefab_ghost = {
    .active = true,
    .health = 25,
    .lane = 0,
    .render = &GhostRender,
    .takeDamage = &GhostTakeDamage,
    .turn = &GhostTurn, 
    .description = ghost_description
};