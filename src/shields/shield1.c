#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <raylib.h>
#include <math.h>

#include "../battle.h"
#include "../particles.h"
#include "../sprite_manager.h"

void ShieldRender(sShield * self, Vector2 position);
void ShieldTakeDamage(sShield * self, sEnemy * pEnemy, Element element, float damage);

char shield1_description [] = "Testing Shield\n40 health";

sShield _prefab_shield1 = {
    .active = true,
    .health = 40,
    .max_health = 40,
    .render = &ShieldRender,
    .take_damage = &ShieldTakeDamage,
    .description = shield1_description
};

void ShieldRender(sShield * self, Vector2 position)
{
    if (self->shake_timer == 1)
    {
        emit_particles_explosion(PARTICLE_DUST, 1.5, (Vector2){position.x + 20, position.y + 20}, (Vector2){50, 50}, 90, 60, 8);
    }
    self->shake_timer -= GetFrameTime() * 1.5;

    if (self->shake_timer > 0)
        position.x += sin(self->shake_timer * 40) * 10 * self->shake_timer;
    // DrawRectangle(position.x, position.y, 50, 50, DARKGRAY);
    DrawTextureEx(_sprite_shield1, position, 0, 0.1, WHITE);
}

void ShieldTakeDamage(sShield * self, sEnemy * pEnemy, Element element, float damage)
{
    self->health -= damage;
    self->shake_timer = 1;
}
