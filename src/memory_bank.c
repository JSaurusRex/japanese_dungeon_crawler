#include <stdlib.h>
#include <stdio.h>

#include "battle.h"

typedef struct{
    int users;
    int id;
    void * data;
} sDataBank;

#define MAX_DATA_BANK 40

sDataBank _data_bank[MAX_DATA_BANK] = {0};
int _data_bank_id = 0;

int new_data_bank(int size)
{
    for(int i = 0; i < MAX_DATA_BANK; i++)
    {
        if (_data_bank[i].id > 0)
            continue;
        
        _data_bank_id++;
        _data_bank[i].id = _data_bank_id;

        _data_bank[i].data = calloc(1, size);

        return _data_bank[i].id;
    }

    return 0;
}

void * get_data_bank(int id)
{
    if (id == 0)
        return 0;
    
    for(int i = 0; i < MAX_DATA_BANK; i++)
    {
        if (_data_bank[i].id == id)
        {
            return _data_bank[i].data;
        }
    }

    return 0;
}

void free_data_bank(int id)
{
    if (_printDebug)
        printf("debug: free_data_bank: freeing id %i\n", id);
    
    free(_data_bank[id].data);
    _data_bank[id].id = 0;
    _data_bank[id].users = 0;
}

void data_bank_update()
{
    for(int i = 0; i < MAX_DATA_BANK; i++)
    {
        if (_data_bank[i].id == 0)
            continue;
        
        // if (_printDebug)
            // printf("debug: data_bank_update: %i has %i users\n", _data_bank[i].id, _data_bank[i].users);

        if (_data_bank[i].users == 0)
        {
            free_data_bank(i);
        }

        _data_bank[i].users = 0;
    }
}

void data_bank_enemy(sEnemy *enemies, int amount)
{
    for(int i = 0; i < MAX_DATA_BANK; i++)
    {
        if (_data_bank[i].id == 0)
            continue;
        
        int id = _data_bank[i].id;
        
        bool found = false;
        for(int enemy = 0; enemy < amount; enemy++)
        {
            if (!enemies[enemy].active)
                continue;

            if (enemies[enemy].data_id != id)
                continue;
            
            _data_bank[i].users++;
        }
    }
}

void data_bank_shield(sShield *shields, int amount)
{
    for(int i = 0; i < MAX_DATA_BANK; i++)
    {
        if (_data_bank[i].id == 0)
            continue;
        
        int id = _data_bank[i].id;
        
        bool found = false;
        for(int shield = 0; shield < amount; shield++)
        {
            if (!shields[shield].active)
                continue;

            if (shields[shield].data_id != id)
                continue;
            
            _data_bank[i].users++;
        }
    }
}

void data_bank_items(sItem *items, int amount)
{
    for(int i = 0; i < MAX_DATA_BANK; i++)
    {
        if (_data_bank[i].id == 0)
            continue;
        
        int id = _data_bank[i].id;
        
        bool found = false;
        for(int item = 0; item < amount; item++)
        {
            if (!items[item].active)
                continue;

            if (items[item].data_id != id)
                continue;
            
            _data_bank[i].users++;
        }
    }
}