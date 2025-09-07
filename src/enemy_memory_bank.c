#include <stdlib.h>
#include <stdio.h>

#include "battle.h"

typedef struct{
    int id;
    void * data;
} sEnemyData;

#define MAX_ENEMY_DATA MAX_ENEMIES

sEnemyData _enemy_data[MAX_ENEMY_DATA] = {0};
int _enemy_id = 0;

int new_enemy_data(int size)
{
    for(int i = 0; i < MAX_ENEMY_DATA; i++)
    {
        if (_enemy_data[i].id > 0)
            continue;
        
        _enemy_id++;
        _enemy_data[i].id = _enemy_id;

        _enemy_data[i].data = calloc(1, size);

        return _enemy_data[i].id;
    }

    return 0;
}

void * get_enemy_data(int id)
{
    if (id == 0)
        return 0;
    
    for(int i = 0; i < MAX_ENEMY_DATA; i++)
    {
        if (_enemy_data[i].id == id)
        {
            return _enemy_data[i].data;
        }
    }

    return 0;
}

void enemy_data_update()
{
    for(int i = 0; i < MAX_ENEMY_DATA; i++)
    {
        if (_enemy_data[i].id == 0)
            continue;
        
        bool found = false;
        for(int enemy = 0; enemy < MAX_ENEMIES; enemy++)
        {
            if (!_enemies[enemy].active)
                continue;

            if (_enemies[enemy].data_id != i)
                continue;
            
            found = true;
            break;
        }

        if (!found)
        {
            free(_enemy_data[i].data);
            _enemy_data[i].id = 0;
        }
    }

}