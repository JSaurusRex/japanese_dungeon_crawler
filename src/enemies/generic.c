#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <math.h>
#include <raylib.h>
#include "../rendering.h"
#include <raymath.h>

#include "../battle.h"
#include "../particles.h"

bool do_at_timer(float *pTimer, float time)
{
    if (*pTimer > time)
    {
        *pTimer -= GetFrameTime();

        if (*pTimer <= time)
        {
            return true;
        }
    }else
        *pTimer -= GetFrameTime();

    if (*pTimer < 0)
        *pTimer = 0;
    
    return false;
}

float shake_manager(float * pTimer)
{
    *pTimer -= GetFrameTime();
    if (*pTimer < 0)
        *pTimer = 0;

    return sin(*pTimer * 40) * 10 * *pTimer;
}

void apply_damage_enemy(sEnemy * pEnemy, Element element, float damage, bool effective)
{
    add_damage_number_particle(pEnemy->lastPosition, element, -damage, effective);
    emit_particles_explosion(PARTICLE_BLOOD, 1.5, Vector2Add((Vector2){25, 25}, pEnemy->lastPosition), (Vector2){50, 50}, 90, 60, 8);

    pEnemy->health -= damage;

    if (damage > 0)
        pEnemy->shake_timer = 1;

    if (pEnemy->health <= 0)
        pEnemy->active = false;
}

float move_forward_animation(float timer, float amount)
{
    return pow(sin(timer * PI), 3) * amount;
}