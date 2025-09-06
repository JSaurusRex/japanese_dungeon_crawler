#ifndef NEXT_LEVEL_SCREEN_H
#define NEXT_LEVEL_SCREEN_H

void next_level_frame();
void next_level_generate();

typedef struct
{
    sItem * pItem;
    sShield * pShield;
    float chance;
} sItemSpawn;

#define LOOT_MAX 14
#define LOOT_SHIELDS_MAX 7

extern void (*_dungeon_generate_loot)();
extern sItem _loot[LOOT_MAX];
extern sShield _loot_shields[LOOT_SHIELDS_MAX];

void generate_loot(sItemSpawn * itemspawn_table, int size, int amount);

#endif