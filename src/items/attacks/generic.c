#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "../../battle.h"
#include "../../questions.h"


sEnemy * _pEnemy = 0;
sItem * _pItem = 0;

bool quiz_succeeded()
{
    return (_answers_incorrect < _quiz_hearts);
}

float damage_factor_calc(unsigned int random_variance_percent, int crit_percent, float crit_factor)
{
    float damage_factor = 1;
    printf("damage factor %.2f\n", damage_factor);

    if (_answers_incorrect == 0)
        damage_factor *= 1 + _pItem->enhanced;
    printf("damage factor %.2f\n", damage_factor);
    
    damage_factor *= (100 - random_variance_percent + rand() % (1 + random_variance_percent * 2)) / 100.0;
    printf("damage factor %.2f\n", damage_factor);

    if (_answers_incorrect == 0 && rand() % 100 < crit_percent)
        damage_factor *= crit_factor;
    
    printf("crit_factor %.2f\n", crit_factor);


    printf("damage factor %.2f\n", damage_factor);
    return damage_factor;
}

bool enemy_sanity_checks(sEnemy * pEnemy)
{
    if (!pEnemy)
    {
        printf("error: Item_effect_enemy: pEnemy is null\n");
        return false;
    }

    if (!pEnemy->active)
    {
        if (_printDebug)
            printf("debug: Item_effect_enemy: clicked on inactive enemy\n");
        return false;
    }

    return true;
}