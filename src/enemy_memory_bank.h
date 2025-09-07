#ifndef ENEMY_MEMORY_BANK_H
#define ENEMY_MEMORY_BANK_H

#ifndef CHANGE_NAME
#define CHANGE_NAME(x) void
#endif

int new_enemy_data(int size);
CHANGE_NAME(struct) * get_enemy_data(int id);
void enemy_data_update();

#endif