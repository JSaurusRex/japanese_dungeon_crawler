#ifndef GENERIC_ATTACK_H
#define GENERIC_ATTACK_H

#include "../../battle.h"

extern sEnemy * _pEnemy;
extern sItem * _pItem;

bool quiz_succeeded();
float damage_factor_calc(unsigned int random_variance_percent, int crit_percent, float crit_factor);
bool enemy_sanity_checks(sEnemy * pEnemy);

#endif