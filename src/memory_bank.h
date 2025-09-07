#ifndef MEMORY_BANK_H
#define MEMORY_BANK_H

#include "battle.h"

#ifndef CHANGE_NAME
    #define CHANGE_NAME(x) void

    #define UNDEF_CHANGE_NAME_MEMORY_BANK
#endif


int new_data_bank(int size);
CHANGE_NAME(struct) * get_data_bank(int id);
void data_bank_update();
void data_bank_enemy(sEnemy *enemies, int amount);
void data_bank_shield(sShield *shields, int amount);
void data_bank_items(sItem *items, int amount);

#ifdef UNDEF_CHANGE_NAME_MEMORY_BANK
    #undef CHANGE_NAME
    #undef UNDEF_CHANGE_NAME_MEMORY_BANK
#endif

#endif