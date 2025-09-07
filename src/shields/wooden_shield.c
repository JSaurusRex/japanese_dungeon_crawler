#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <raylib.h>
#include "../rendering.h"
#include <math.h>

#include "../battle.h"
#include "../particles.h"
#include "../sprite_manager.h"

#define CHANGE_NAME(x) wooden_shield ## _ ## x

void CHANGE_NAME(Render)(sShield * self, Vector2 position);
void CHANGE_NAME(TakeDamage)(sShield * self, sEnemy * pEnemy, Element element, float damage);

char CHANGE_NAME(description) [] = "wooden shield\n20 health";

sShield _prefab_wooden_shield = {
    .active = true,
    .health = 20,
    .max_health = 20,
    .render = &CHANGE_NAME(Render),
    .take_damage = &CHANGE_NAME(TakeDamage),
    .pack = "hiragana1",
    .level = 1,
    .description = CHANGE_NAME(description)
};

void CHANGE_NAME(Render)(sShield * self, Vector2 position)
{
    if (self->shake_timer == 1)
    {
        emit_particles_explosion(PARTICLE_DUST, 1.5, (Vector2){position.x + 20, position.y + 20}, (Vector2){50, 50}, 90, 60, 8);
    }
    self->shake_timer -= GetFrameTime() * 1.5;

    if (self->shake_timer > 0)
        position.x += sin(self->shake_timer * 40) * 10 * self->shake_timer;
    drawTextureEx(_wooden_shield_sprite, position, 0, 0.1, WHITE);
    self->lastPosition = position;
}

void CHANGE_NAME(TakeDamage)(sShield * self, sEnemy * pEnemy, Element element, float damage)
{
    if (element == ELEMENT_FIRE)
        damage *= 1.5;

    add_damage_number_particle(self->lastPosition, element, -damage, element == ELEMENT_FIRE);
    self->health -= damage;
    self->shake_timer = 1;
}
