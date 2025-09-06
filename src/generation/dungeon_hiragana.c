#include <stdlib.h>
#include <stdio.h>

#include "../battle.h"
#include "../next_level_screen.h"

#include "../enemies/enemy1.h"
#include "../enemies/goblin1.h"
#include "../enemies/ghost.h"
#include "../items/attacks/sword1.h"
#include "../items/shield_repair_item.h"
#include "../items/item_enhancer.h"
#include "../items/upgrade_item.h"
#include "../items/energy_potion.h"
#include "../items/healing_potion.h"
#include "../items/attacks/fire_wand1.h"
#include "../shields/shield1.h"

#include "../enemies/generic.h"

#define SPAWN_SHIELD(prefab, chance) {0, &prefab, chance},
#define SPAWN_ITEM(prefab, chance) {&prefab, 0, chance},
#define SPAWN_ENEMY(prefab, chance) {&prefab, chance},


void dungeon_hiragana_enemy_spawning()
{
    int loottable_levels [0];
    int level = 0;

    int loottable_levels_amount = sizeof(loottable_levels)/sizeof(int);
    for(int i = 1; i < loottable_levels_amount; i++)
    {
        if ( loottable_levels[i] > _level)
        {
            level = loottable_levels[i-1];
            break;
        }
    }

    //generate loot
    switch(level)
    {
        case 0:
        {
            sEnemySpawn spawn_table[] = {
                SPAWN_ENEMY(_prefab_enemy1, 0.2)
                SPAWN_ENEMY(_prefab_goblin1, 0.2)
                SPAWN_ENEMY(_prefab_ghost, 1)
            };

            spawn_enemies(spawn_table, sizeof(spawn_table)/sizeof(sEnemySpawn), 1 + rand() % 3);
            break;
        }
    }
}

void dungeon_hiragana_loot_spawning()
{
    int loottable_levels [0];
    int level = 0;

    int loottable_levels_amount = sizeof(loottable_levels)/sizeof(int);
    for(int i = 1; i < loottable_levels_amount; i++)
    {
        if ( loottable_levels[i] > _level)
        {
            level = loottable_levels[i-1];
            break;
        }
    }

    //generate loot
    switch(level)
    {
        case 0:
        {
            _loot[0] = _prefab_sword1;
            // _loot[1] = _prefab_healing_potion;
            // _loot[2] = _prefab_energy_potion;
            _loot_shields[0] = _prefab_shield1;
            sItemSpawn spawn_table[] = {
                SPAWN_ITEM(_prefab_sword1, 0.2)
                SPAWN_ITEM(_prefab_firewand1, 0.2)
                SPAWN_SHIELD(_prefab_shield1, 0.5)
                SPAWN_ITEM(_prefab_shield_repair_item, 0.5)
                SPAWN_ITEM(_prefab_upgrade_item, 0.2)
                SPAWN_ITEM(_prefab_healing_potion, 0.2)
                SPAWN_ITEM(_prefab_energy_potion, 0.2)
            };

            generate_loot(spawn_table, sizeof(spawn_table)/sizeof(sItemSpawn), 2 + rand() % 4);
            break;
        }
    }
}