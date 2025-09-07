#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <raylib.h>
#include "../rendering.h"
#include <math.h>

#include "../battle.h"
#include "../particles.h"
#include "../sprite_manager.h"

#include "../items/upgrade_item.h"


#define CHANGE_NAME(x) chest ## _ ## x

void CHANGE_NAME(Render)(sShield * self, Vector2 position);
void CHANGE_NAME(TakeDamage)(sShield * self, sEnemy * pEnemy, Element element, float damage);

char CHANGE_NAME(description) [] = "Chest\n50 health\nGives you an item\nwhen it gets hit\nenough times\nMake sure it\nsurvives until then";

sShield _prefab_chest = {
    .active = true,
    .health = 50,
    .max_health = 50,
    .render = &CHANGE_NAME(Render),
    .take_damage = &CHANGE_NAME(TakeDamage),
    .pack = "hiragana1",
    .level = 1,
    .description = CHANGE_NAME(description)
};

typedef struct
{
    int hit_counter;
} CHANGE_NAME(struct);

#include "../memory_bank.h"

void CHANGE_NAME(Render)(sShield * self, Vector2 position)
{

    if (self->shake_timer == 1)
    {
        emit_particles_explosion(PARTICLE_DUST, 1.5, (Vector2){position.x + 20, position.y + 20}, (Vector2){50, 50}, 90, 60, 8);
    }
    self->shake_timer -= GetFrameTime() * 1.5;

    if (self->shake_timer > 0)
        position.x += sin(self->shake_timer * 40) * 10 * self->shake_timer;
    // drawRectangle(position.x, position.y, 50, 50, DARKGRAY);
    drawTextureEx(_sprite_shield1, position, 0, 0.1, GRAY);
    
    if (self->data_id)
    {
        CHANGE_NAME(struct) * data = get_data_bank(self->data_id);

        if (data)
        {
            char str[STRING_LENGTH];
            snprintf(str, STRING_LENGTH, "%i/4", data->hit_counter);
            drawTextEx(_fontJapanese, str, position, 20, 2, WHITE, true);
        }
    }
    self->lastPosition = position;
}

void CHANGE_NAME(TakeDamage)(sShield * self, sEnemy * pEnemy, Element element, float damage)
{
    if (self->data_id == 0)
    {
        self->data_id = new_data_bank(sizeof(CHANGE_NAME(struct)));
    }

    if (self->data_id != 0)
    {
        CHANGE_NAME(struct) * data = get_data_bank(self->data_id);

        if (data)
        {
            data->hit_counter++;

            if (data->hit_counter >= 4)
            {
                //give random item
                self->active = false;

                //todo: not yet random
                try_return_item();

                if (!_item_hand.active && !_shield_hand.active)
                {
                    _item_hand = _prefab_upgrade_item;
                }
            }
        }
    }

    add_damage_number_particle(self->lastPosition, element, -damage, false);
    self->health -= damage;
    self->shake_timer = 1;

    if (self->health <= 0)
    {
        self->active = false;
    }
}
