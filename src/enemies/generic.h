#ifndef GENERIC_ENEMY_H
#define GENERIC_ENEMY_H

#include <stdbool.h>

#include "../battle.h"

bool do_at_timer(float *pTimer, float time);
float shake_manager(float *pTimer);
void apply_damage_enemy(sEnemy * pEnemy, Element element, float damage);
float move_forward_animation(float timer, float amount);

#endif