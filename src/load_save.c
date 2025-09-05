#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <raylib.h>

#include "battle.h"

#include "items/attacks/fire_wand1.h"
#include "items/attacks/sword1.h"

#include "items/energy_potion.h"
#include "items/healing_potion.h"
#include "items/item_enhancer.h"
#include "items/shield_repair_item.h"
#include "items/upgrade_item.h"

#include "shields/shield1.h"

sItem * _load_array [] = {
    &_prefab_firewand1,
    &_prefab_sword1,
    &_prefab_energy_potion,
    &_prefab_healing_potion,
    &_prefab_item_enhancer,
    &_prefab_shield_repair_item,
    &_prefab_upgrade_item
};

sShield * _load_array_shield [] = {
    &_prefab_shield1
};

void load_save()
{
    //empty inventory
    for(int i = 0; i < MAX_ITEMS; i++)
        _inventory[i].active = false;
    
    for(int i = 0; i < MAX_SHIELDS; i++)
        _shield_inventory[i].active = false;
    

    //give items their ids
    int load_array_size = sizeof(_load_array) / sizeof(sItem);
    for(int i = 0; i < load_array_size; i++)
    {
        _load_array[i]->load_id = i;
    }

    //give shields their ids
    load_array_size = sizeof(_load_array_shield) / sizeof(sItem);
    for(int i = 0; i < load_array_size; i++)
    {
        _load_array_shield[i]->load_id = i;
    }

    char * text = LoadFileText("save.s");
    if (!text)
    {
        printf("error: load_save: failed to load save\n");
        return;
    }

    //process file
    int index = 0;
    int length = strlen(text);

    int item_index = 0;
    int shield_index = 0;
    for(int line = 0; line < 5; line++)
    {
        if(index >= length)
            break;
        
        char c = text[index];
        index += 2; //skip :

        if (index >= length)
            break;
        
        // if (_printDebug)
        //     printf("debug: ProcessQuestionsFile: c = %c\n", c);
        
        switch(c)
        {
            //item
            case 'i':
            {
                if (item_index >= MAX_ITEMS)
                    break;
                
                int item_id = atoi(&text[index]);

                index += 3; // - 
                if (index >= length)
                    break;
                int level = atoi(&text[index]);

                //spawn item
                _inventory[item_index] = *_load_array[item_id];
                _inventory[item_index].level = level;
                
                if(_printDebug)
                    printf("load_save: found item %i: %i lvl %i\n", item_index, item_id, level);
                item_index++;
                break;
            }
            
            //shield
            case 's':
            {
                if (shield_index >= MAX_SHIELDS)
                    break;
                
                int shield_id = atoi(&text[index]);

                index += 3; // - 
                if (index >= length)
                    break;
                int level = atoi(&text[index]);

                //spawn shield
                _shield_inventory[shield_index] = *_load_array_shield[shield_id];
                _shield_inventory[shield_index].level = level;
                shield_index++;

                if(_printDebug)
                    printf("load_save: found shield %i: %i lvl %i\n", shield_index, shield_id, level);
                break;
            }

            //highscore
            case 'h':
            {
                _highscore = atoi(&text[index]);
                if(_printDebug)
                    printf("load_save: found highscore %i\n", _highscore);
                break;
            }
        }

        //goto next line
        while(index < length && text[index] != '\n')
        {
            index++;
        }
        index++; //atm its at '\n' so we have to move 1 further
    }

    UnloadFileText(text);    
}

void save_save()
{
    char str[1000] = {0};
    char str_copy[1000] = {0};

    int index = 0;

    for(int i = 0; i < MAX_ITEMS; i++)
    {
        if (!_inventory[i].active)
            continue;
        
        snprintf(str_copy, 1000, "%s\ni:%i - %i", str, _inventory[i].load_id, _inventory[i].level);
        strncpy(str, str_copy, 1000);       
    }

    for(int i = 0; i < MAX_SHIELDS; i++)
    {
        if (!_shield_inventory[i].active)
            continue;
        
        snprintf(str_copy, 1000, "%s\ns:%i - %i", str, _shield_inventory[i].load_id, _shield_inventory[i].level);
        strncpy(str, str_copy, 1000);
    }

    snprintf(str_copy, 1000, "%s\nh:%i", str, _highscore);

    strncpy(str, str_copy, 1000);

    SaveFileText("save.s", str);
}