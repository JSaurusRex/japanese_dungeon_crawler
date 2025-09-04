#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <raylib.h>
#include <raymath.h>
#include <math.h>

#include "main.h"
#include "questions.h"
#include "battle.h"
#include "particles.h"
#include "shadows.h"
#include "next_level_screen.h"
#include "gameover.h"
#include "sprite_manager.h"

#include "enemies/enemy1.h"
#include "enemies/goblin1.h"
#include "items/attacks/sword1.h"
#include "items/shield_repair_item.h"
#include "items/item_enhancer.h"
#include "items/attacks/fire_wand1.h"
#include "shields/shield1.h"

#include "enemies/generic.h"

sEnemy _enemies [MAX_ENEMIES] = {0};
sItem _inventory [MAX_ITEMS] = {0};
sItem _item_hand = {0};

sShield _shield_lanes [MAX_LANES] = {0};
sShield _shield_inventory [MAX_SHIELDS] = {0};
sShield _shield_hand = {0};

Texture2D _battle_screen_texture;

int _disabled_slot = -1;

float _health = 10;
float _shake_timer = 0;
float _max_health = 100;
int _level = 1;
Vector2 _lastPosition;

void try_return_item();
void next_turn();
void Battle_End();

int _max_energy = 50;
int _energy = 50;
int _turn = -1;
float _turn_breather = 0;

char * _description;

float _battle_timer = 0;

Vector2 CalculateEnemyPosition(int lane, int position)
{
    float walk_in_animation = Remap(powf(fmin(_battle_timer / 2.0 - lane - position * 3, 1), 0.3), 0, 1, 500, 0);
    return (Vector2){500 + walk_in_animation + position * 80 + lane * 10, 240 + lane * 50};
}

void consume_energy(int energy)
{
    _energy -= energy;
    if (_energy <= 0)
    {
        _energy = 0;
        _turn = -1; //set turn to first enemies
        next_turn();
    }

    if (_energy > _max_energy)
        _energy = _max_energy;
}

void next_turn()
{
    //de-enhance items
    if(_turn == -1)
    {        
        for(int i = 0; i < MAX_ITEMS; i++)
        {
            _inventory[i].enhanced = 0;
            _inventory[i].rounds_disabled--;
        }
    }

    _turn_breather = 1;
    _turn++;
}

void do_next_turn()
{   
    while(!_enemies[_turn].active && _turn < MAX_ENEMIES)
        _turn++;
    
    if (_turn >= MAX_ENEMIES)
    {
        _disabled_slot = rand() % MAX_ITEMS;
        _turn = -1;
        _energy += 25;

        if (_energy > _max_energy)
            _energy = _max_energy;

        //check if any enemies are left
        bool enemy_left = false;
        for(int enemy = 0; enemy < MAX_ENEMIES; enemy++)
        {
            if (!_enemies[enemy].active)
                continue;
            enemy_left = true;
            break;
        }

        //switch to loot screen
        if (!enemy_left)
        {
            Battle_End();
            next_level_generate();
        }
    }
    else
        _enemies[_turn].turn(&_enemies[_turn]);
}

void take_damage(int lane, Element element, float damage)
{
    if (lane < 0 || lane >= MAX_LANES)
    {
        printf("error: take_damage: lane is non existant %i\n", lane);
        return;
    }

    if (_shield_lanes[lane].active)
    {
        _shield_lanes[lane].take_damage(&_shield_lanes[lane], 0, element, damage);

        if (_shield_lanes[lane].health <= 0)
        {
            //put shield back in inventory
            for(int i = 0; i < MAX_SHIELDS; i++)
            {
                if (_shield_inventory[i].active)
                    continue;
                
                _shield_inventory[i] = _shield_lanes[lane];
                _shield_lanes[lane].active = false;
            }
        }
    }
    else
    {
        _shake_timer = 1;
        emit_particles_explosion(PARTICLE_BLOOD, 2, _lastPosition, (Vector2){60, 40}, 80, 60, 6);
        add_damage_number_particle(_lastPosition, element, -damage);
        _health -= damage;
    }
    
    if (_health <= 0)
    {
        Battle_End();
        Start_GameOver();
    }
}

void Battle_Init()
{
    _battle_screen_texture = LoadTexture("data/screen/battle_screen.png");
    // _inventory[0] = _prefab_sword1;
    // _inventory[1] = _prefab_shield_repair_item;
    // _inventory[2] = _prefab_firewand1;
    // _shield_inventory[0] = _prefab_shield1;
}

void Battle_Reset()
{
    _health = _max_health;
    _level = 0;
}

typedef struct
{
    sEnemy * pEnemy;
    float chance;
} sEnemySpawn;

void spawn_enemies(sEnemySpawn * enemySpawn_table, int size, int amount)
{
    printf("amount %i\n", amount);
    printf("size %i\n", size);
    int total_chance = 0;
    for(int i = 0; i < size; i++)
    {
        total_chance += enemySpawn_table[i].chance * 100;
    }

    printf("total_chance %i\n", total_chance);

    //choose amount random enemies
    for(int i = 0; i < amount; i++)
    {
        int number = rand() % total_chance;

        printf("i %i\n", i);

        int chance = 0;
        for(int enemy = 0; enemy < size; enemy++)
        {
            chance += enemySpawn_table[enemy].chance * 100;
            if (number > chance)
                continue;
            
            //find empty spot to put enemy in
            for(int spot = 0; spot < MAX_ENEMIES; spot++)
            {
                if (_enemies[spot].active)
                    continue;
                
                printf("spot %i\n", spot);
                _enemies[spot] = *enemySpawn_table[enemy].pEnemy;
                _enemies[spot].lane = rand() % MAX_LANES;

                printf("active %i\n", _enemies[spot].active);
                break;
            }
            break;
        }
    }
}

void spawn_enemies_manager()
{
    //empty existing loot
    for(int i = 0; i < MAX_ENEMIES; i++)
        _enemies[i].active = false;
    
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
                {&_prefab_enemy1, 0.2},
                {&_prefab_goblin1, 0.2}
            };

            spawn_enemies(spawn_table, sizeof(spawn_table)/sizeof(sEnemySpawn), 2 + rand() % 3);
            break;
        }
    }
}

void Battle_Start()
{
    _battle_timer = 0;
    _screen = &Battle_Frame;
    // _enemies[0] = _prefab_enemy1;
    // _enemies[1] = _prefab_goblin1;
    // _enemies[1].lane = 1;
    spawn_enemies_manager();

    _disabled_slot = rand() % MAX_ITEMS;

    _energy = 50;
    _turn = -1;
}

void Battle_End()
{
    _turn = -1;
    _disabled_slot = -1;
    _energy = _max_energy;

    
    try_return_item();

    //disable disabling
    for(int i = 0; i < MAX_ITEMS; i++)
    {
        _inventory[i].rounds_disabled = 0;
    }

    //return shields
    for(int i = 0; i < MAX_LANES; i++)
    {
        if (!_shield_lanes[i].active)
            continue;
        
        for(int shield = 0; shield < MAX_SHIELDS; shield++)
        {
            if (_shield_inventory[shield].active)
                continue;
            
            //copy
            _shield_inventory[shield] = _shield_lanes[i];
            break;
        }
        _shield_lanes[i].active = false;
    }
}

void try_return_item()
{
    if (_item_hand.active)
    {
        for(int item = 0; item < MAX_ITEMS; item++)
        {
            if (_inventory[item].active)
                continue;
            
            //copy
            _inventory[item] = _item_hand;
            _item_hand.active = false;
            break;
        }
    }

    if (_shield_hand.active)
    {
        for(int shield = 0; shield < MAX_SHIELDS; shield++)
        {
            if (_shield_inventory[shield].active)
                continue;
            
            //copy
            _shield_inventory[shield] = _shield_hand;
            _shield_hand.active = false;
            break;
        }
    }
}

void draw_item(Vector2 pos, sItem * pItem)
{
    if (pItem->enhanced > 0)
        DrawCircle(pos.x+25, pos.y+25, 30, GOLD);

    if (pItem->render)
        pItem->render(pItem, pos);
    
    if (pItem->rounds_disabled > 0 || pItem->energy > _energy)
    {
        DrawCircle(pos.x+25, pos.y+25, 35, ColorAlpha(BLACK, 0.4));

        if (pItem->rounds_disabled > 0)
        {
            char str[STRING_LENGTH];
            snprintf(str, STRING_LENGTH, "%i", pItem->rounds_disabled);
            DrawTextEx(_fontJapanese, str, (Vector2){ pos.x - 10, pos.y - 10 }, 20, 2, WHITE);
        }
    }
    
    //draw level numbers
    {
        char str[STRING_LENGTH];
        snprintf(str, STRING_LENGTH, "lvl: %i", pItem->level);
        DrawTextEx(_fontJapanese, str, (Vector2){ pos.x, pos.y + 40 }, 20, 2, WHITE);
    }

    //draw energy numbers
    {
        char str[STRING_LENGTH];
        snprintf(str, STRING_LENGTH, "e: %i", pItem->energy);
        DrawTextEx(_fontJapanese, str, (Vector2){ pos.x + 10, pos.y + 50 }, 20, 2, WHITE);
    }
}

void draw_inventory()
{
    for(int item = 0; item < MAX_ITEMS; item++)
    {   
        int x = item % 7;
        int y = floor(item / 7.0);

        Vector2 pos = (Vector2){30 + x * 80, 430 + y * 100};

        DrawRectangle(pos.x - 10, pos.y - 10, 75, 75, ColorAlpha(BLACK, 0.2));
    }

    for(int item = 0; item < MAX_ITEMS; item++)
    {
        int x = item % 7;
        int y = floor(item / 7.0);

        Vector2 pos = (Vector2){30 + x * 80, 430 + y * 100};

        bool mouse_inside = false;
        if (pos.x < GetMouseX() && GetMouseX() < pos.x + 50)
        {
            if (pos.y < GetMouseY() && GetMouseY() < pos.y + 50)
            {
                mouse_inside = true;
            }
        }

        if (!_inventory[item].active)
        {
            bool interactable = IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && _turn == -1;
            if (mouse_inside && _item_hand.active && interactable && _disabled_slot != item)
            {
                _inventory[item] = _item_hand;
                _item_hand.active = false;
            }

            if (_disabled_slot == item)
            {
                DrawTexture(_disabled_slot_sprite, pos.x-10, pos.y-10, WHITE);
            }

            continue;
        }


        draw_item(pos, &_inventory[item]);
        
        if (_disabled_slot == item)
        {
            DrawTexture(_disabled_slot_sprite, pos.x-10, pos.y-10, WHITE);
        }

        //item input / mouse behaviour
        if (mouse_inside && _disabled_slot != item)
        {
            _description = _inventory[item].description;

            bool interactable = _inventory[item].rounds_disabled <= 0 && _turn == -1 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

            if (!_item_hand.active && !_shield_hand.active && interactable && _inventory[item].energy <= _energy)
            {
                printf("clicked item!\n");
                //pick up item
                _item_hand = _inventory[item];
                _inventory[item].active = false;
                _item_hand.active = true;
            }

            if (_item_hand.active && interactable && _item_hand.effect_item)
            {
                _item_hand.effect_item(&_item_hand, &_inventory[item]);
            }
        }
    }

    //draw shields
    for(int shield = 0; shield < MAX_SHIELDS; shield++)
    {
        Vector2 pos = (Vector2){20, 10 + shield * 80};

        bool mouse_inside = false;
        if (pos.x < GetMouseX() && GetMouseX() < pos.x + 50)
        {
            if (pos.y < GetMouseY() && GetMouseY() < pos.y + 50)
            {
                mouse_inside = true;
            }
        }

        bool interactable = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);


        if (!_shield_inventory[shield].active)
        {
            DrawCircle(pos.x+25, pos.y+25, 15, YELLOW);

            if (interactable && mouse_inside && _shield_hand.active)
            {
                _shield_inventory[shield] = _shield_hand;
                _shield_hand.active = false;
            }
            continue;
        }
                


        if (_shield_inventory[shield].render)
            _shield_inventory[shield].render(&_shield_inventory[shield], pos);
        
        //draw health numbers
        {
            char str[STRING_LENGTH];
            snprintf(str, STRING_LENGTH, "hp: %.0f", _shield_inventory[shield].health);
            DrawTextEx(_fontJapanese, str, (Vector2){ pos.x, pos.y + 50 }, 20, 2, WHITE);
        }
        
        //shield input / mouse behaviour
        if (mouse_inside)
        {
            _description = _shield_inventory[shield].description;

            if (!_item_hand.active && !_shield_hand.active && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && _turn == -1 &&
                _shield_inventory[shield].health > 0)
            {
                printf("clicked shield!\n");
                //pick up shield
                _shield_hand = _shield_inventory[shield];
                _shield_inventory[shield].active = false;
                _shield_hand.active = true;
            }

            if (_item_hand.active && _item_hand.effect_shield && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && _turn == -1)
            {
                printf("clicked shield!\n");
                //apply item on shield
                _item_hand.effect_shield(&_item_hand, &_shield_inventory[shield]);
            }
        }
    }
}

void draw_items_UI()
{
    //draw description
    if (_description)
    {
        DrawTextEx(_fontJapanese, _description, (Vector2){638, 385}, 19, 2, WHITE);
    }

    //draw dragged items
    if (_item_hand.active && _item_hand.render)
    {
        _item_hand.render(&_item_hand, GetMousePosition());
    }

    //draw dragged shield
    if (_shield_hand.active && _shield_hand.render)
    {
        _shield_hand.render(&_shield_hand, GetMousePosition());
    }

    //drop item with right mouse click
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && (_item_hand.active || _shield_hand.active))
    {
        try_return_item();
    }
}

void heal_player(float amount)
{
    _health += amount;

    add_damage_number_particle(_lastPosition, ELEMENT_NONE, amount);

    if (_health >= _max_health)
        _health = _max_health;
}

void Battle_Frame()
{
    _battle_timer += GetFrameTime();

    if (_turn_breather > 0)
    {
        _turn_breather -= GetFrameTime();
        
        if (_turn_breather <= 0)
            do_next_turn();
    }

    DrawTexture(_battle_screen_texture, 0, 0, WHITE);
    render_shadows();

    // render player
    {

        Vector2 pos = CalculateEnemyPosition(1, -4);

        pos.x += shake_manager(&_shake_timer);

        _lastPosition = pos;

        DrawCircle(pos.x, pos.y, 30, WHITE);


        if (pos.x - 15 < GetMouseX() && GetMouseX() < pos.x + 15)
        {
            if (pos.y - 15 < GetMouseY() && GetMouseY() < pos.y + 15)
            {
                _description = "player";

                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && _turn == -1)
                {
                    printf("clicked player!\n");
                    if (_item_hand.active && _item_hand.effect_player)
                    {
                        (*_item_hand.effect_player)(&_item_hand);
                    }
                }
            }
        }
    }

    //draw enemies
    int postition[MAX_LANES] = {0};
    for(int enemy = 0; enemy < MAX_ENEMIES; enemy++)
    {
        if (!_enemies[enemy].active)
            continue;
        
        
        _enemies[enemy].render(&_enemies[enemy], postition[_enemies[enemy].lane]);
        
        postition[_enemies[enemy].lane]++;

        Vector2 pos = CalculateEnemyPosition(_enemies[enemy].lane, 0);

        {
            if (pos.x - 50 < GetMouseX() && GetMouseX() < pos.x + 50)
            {
                if (pos.y - 50 < GetMouseY() && GetMouseY() < pos.y + 50)
                {
                    _description = _enemies[enemy].description;

                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && _turn == -1)
                    {
                        printf("clicked enemy!\n");
                        if (_item_hand.active && _item_hand.effect_enemy)
                        {
                            (*_item_hand.effect_enemy)(&_item_hand, &_enemies[enemy]);
                        }
                    }
                }
            }
        }
    }

    //draw enemy numbers
    for(int enemy = 0; enemy < MAX_ENEMIES; enemy++)
    {
        if (!_enemies[enemy].active)
            continue;
        
        Vector2 pos = _enemies[enemy].lastPosition;
        //draw health numbers
        char str[STRING_LENGTH];
        snprintf(str, STRING_LENGTH, "hp: %.0f", _enemies[enemy].health);
        DrawTextEx(_fontJapanese, str, (Vector2){ pos.x, pos.y + 50 }, 20, 2, WHITE);
    }

    //draw inventory
    draw_inventory();    

    //draw lane shield spots
    for(int lane = 0; lane < MAX_LANES; lane++)
    {
        Vector2 pos = CalculateEnemyPosition(lane, -2);

        pos.y += 20;

        if(_shield_lanes[lane].active)
        {
            add_shadow((Vector2){pos.x, pos.y + 20}, 0.7);
            _shield_lanes[lane].render(&_shield_lanes[lane], (Vector2){pos.x - 25, pos.y - 25});

            //draw health numbers
            {
                char str[STRING_LENGTH];
                snprintf(str, STRING_LENGTH, "hp:%.0f", _shield_lanes[lane].health);
                DrawTextEx(_fontJapanese, str, (Vector2){ pos.x - 50, pos.y }, 20, 2, WHITE);
            }
            
        }
        else
            DrawCircle(pos.x, pos.y, 15, YELLOW);

        {
            if (pos.x - 15 < GetMouseX() && pos.x + 15 > GetMouseX())
            {
                if (pos.y - 15 < GetMouseY() && pos.y + 15 > GetMouseY())
                {
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && _shield_hand.active && !_shield_lanes[lane].active && _turn == -1)
                    {
                        _shield_lanes[lane] = _shield_hand;
                        _shield_hand.active = false;
                    }
                    else if (_shield_lanes[lane].active)
                    {
                        _description = _shield_lanes[lane].description;

                        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !_shield_hand.active && !_item_hand.active && _turn == -1)
                        {
                            _shield_hand = _shield_lanes[lane];
                            _shield_lanes[lane].active = false;
                        }

                        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && _item_hand.active && _item_hand.effect_shield && _turn == -1)
                        {
                            _item_hand.effect_shield(&_item_hand, &_shield_lanes[lane]);
                        }
                    }
                }
            }
        }
    }

    //end turn button
    if (_turn == -1)
    {
        Vector2 pos = (Vector2){ 300, 10 };
        DrawTextEx(_fontJapanese, "end turn", pos, 30, 2, WHITE);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (pos.x < GetMouseX() && pos.x + 80 > GetMouseX())
            {
                if(pos.y < GetMouseY() && pos.y + 40 > GetMouseY())
                {
                    printf("pressed end turn!\n");
                    next_turn();
                }
            }
        }
    }

    //draw health / energy numbers
    {
        char str[STRING_LENGTH];
        snprintf(str, STRING_LENGTH, "Health: %i\nEnergy: %i/%i", (int)_health, _energy, _max_energy);
        DrawTextEx(_fontJapanese, str, (Vector2){ 120, 10 }, 30, 2, WHITE);
    }

    //draw level number
    {
        char str[STRING_LENGTH];
        snprintf(str, STRING_LENGTH, "Level: %i", _level);
        DrawTextEx(_fontJapanese, str, (Vector2){ 590, 10 }, 60, 2, WHITE);
    }

    draw_items_UI();

    draw_particles();
}