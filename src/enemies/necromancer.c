#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <raylib.h>
#include "../rendering.h"
#include <raymath.h>
#include <math.h>

#include "../battle.h"
#include "../particles.h"
#include "../shadows.h"
#include "../sprite_manager.h"

#include "generic.h"

#include "zombie.h"

#define CHANGE_NAME(y) Necromancer ## _ ## y

#define MAX_ZOMBIES 3

typedef struct
{
    int rounds_since_zombie_revive;
    sEnemy * zombies[MAX_ZOMBIES];
} CHANGE_NAME(struct);

#include "../enemy_memory_bank.h"

void CHANGE_NAME (Render)(sEnemy * self, int position)
{
    Vector2 pos = CalculateEnemyPosition(self->lane, position);
    add_shadow((Vector2){pos.x+23, pos.y+50}, 0.8, true);

    pos.x += shake_manager(&self->shake_timer);

    if (do_at_timer(&self->attack_animation_timer, 0.5))
    {
        if (self->data_id != 0)
        {
            //get and cast data
            CHANGE_NAME(struct) * data = get_enemy_data(self->data_id);

            if (data)
            {
                data->rounds_since_zombie_revive++;

                if (data->rounds_since_zombie_revive >= 3)
                {
                    data->rounds_since_zombie_revive = 0;
                    for(int i = 0; i < MAX_ZOMBIES; i++)
                    {
                        if (data->zombies[i] == 0)
                            continue;

                        if (!data->zombies[i]->active)
                            continue;
                        
                        if (_printDebug)
                            printf("debug: necromancer: reviving zombie!\n");
                        //zombie logic
                        data->zombies[i]->health = 5;
                        self->shake_timer = 1;
                    }
                }
            }
        }

        next_turn();
    }

    drawTextureEx(_goblin1_sprite, pos, 0, 0.1, GRAY);
    self->lastPosition = pos;
}

void CHANGE_NAME (Turn)(sEnemy * self)
{
    printf("doing enemy turn!\n");

    //spawn logic
    if (self->data_id == 0)
    {
        self->data_id = new_enemy_data(sizeof(CHANGE_NAME(struct)));

        if (self->data_id != 0)
        {

            //get and cast data
            CHANGE_NAME(struct) * data = (CHANGE_NAME(struct) * ) get_enemy_data(self->data_id);

            //spawn zombies
            for(int i = 0; i < MAX_ZOMBIES; i++)
            {
                data->zombies[i] = spawn_enemy(&_prefab_zombie);
            }

            self->shake_timer = 1;
        }
    }

    self->attack_animation_timer = 1;
}

void CHANGE_NAME (TakeDamage)(sEnemy * self, float damage, Element element)
{
    if (element == ELEMENT_HOLY)
        damage *= 1.4;
    
    if (_printDebug)
        printf("debug: EnemyTakeDamage: enemy took %.2f damage\n", damage);

    apply_damage_enemy(self, element, damage, element == ELEMENT_HOLY);
    
    if (self->health <= 0)
        self->active = false;
}

char CHANGE_NAME(description)[] = "necromancer\n25 health\nDeals 20 dmg\nWeak to Holy magic";

sEnemy _prefab_necromancer = {
    .active = true,
    .health = 15,
    .lane = 0,
    .render = &CHANGE_NAME (Render),
    .takeDamage = &CHANGE_NAME (TakeDamage),
    .turn = &CHANGE_NAME (Turn), 
    .description = CHANGE_NAME(description)
};