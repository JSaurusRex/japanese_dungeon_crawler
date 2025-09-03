#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <raylib.h>
#include <math.h>

#include "main.h"
#include "questions.h"
#include "battle.h"
#include "particles.h"
#include "shadows.h"
#include "next_level_screen.h"

#include "enemies/enemy1.h"
#include "items/item1.h"
#include "items/shield_repair_item.h"
#include "items/item_enhancer.h"
#include "shields/shield1.h"

sEnemy _enemies [MAX_ENEMIES] = {0};
sItem _inventory [MAX_ITEMS] = {0};
sItem _item_hand = {0};

sShield _shield_lanes [MAX_LANES] = {0};
sShield _shield_inventory [MAX_SHIELDS] = {0};
sShield _shield_hand = {0};

Texture2D _battle_screen_texture;

float _health = 100;

void try_return_item();
void next_turn();

int _max_energy = 50;
int _energy = 50;
int _turn = -1;
float _turn_breather = 0;

char * _description;

Vector2 CalculateEnemyPosition(int lane, int position)
{
    return (Vector2){500 + position * 80 + lane * 10, 240 + lane * 50};
}

void consume_energy(int energy)
{
    _energy -= energy;
    if (_energy <= 0)
    {
        _energy += 25;

        if (_energy > _max_energy)
            _energy = _max_energy;
        _turn = -1; //set turn to first enemies
        next_turn();
    }
}

void next_turn()
{
    //de-enhance items
    if(_turn == -1)
        for(int i = 0; i < MAX_ITEMS; i++)
        {
            _inventory[i].enhanced = 1;
            _inventory[i].rounds_disabled--;
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
        _turn = -1;
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
        _health -= damage;
}

void Battle_Init()
{
    _battle_screen_texture = LoadTexture("data/screen/battle_screen.png");
}

void Battle_Start()
{
    _screen = &Battle_Frame;
    _enemies[0] = _prefab_enemy1;
    _inventory[0] = _prefab_item1;
    _inventory[1] = _prefab_shield_repair_item;
    _inventory[2] = _prefab_item_enhancer;
    _shield_inventory[0] = _prefab_shield1;

    _energy = 50;
    _turn = -1;
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
        }
    }
}

void draw_item(Vector2 pos, sItem * pItem)
{
    if (pItem->enhanced > 1)
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
            if (mouse_inside && _item_hand.active && interactable)
            {
                _inventory[item] = _item_hand;
                _item_hand.active = false;
            }

            continue;
        }


        draw_item(pos, &_inventory[item]);
        
        //item input / mouse behaviour
        if (mouse_inside)
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
        Vector2 pos = (Vector2){10, 10 + shield * 50};

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

void Battle_Frame()
{
    if (_turn_breather > 0)
    {
        _turn_breather -= GetFrameTime();
        
        if (_turn_breather <= 0)
            do_next_turn();
    }

    DrawTexture(_battle_screen_texture, 0, 0, WHITE);
    render_shadows();

    //draw enemies
    for(int enemy = 0; enemy < MAX_ENEMIES; enemy++)
    {
        if (!_enemies[enemy].active)
            continue;
        
        
        _enemies[enemy].render(&_enemies[enemy], 0);

        Vector2 pos = CalculateEnemyPosition(_enemies[enemy].lane, 0);

        //draw health numbers
        {
            char str[STRING_LENGTH];
            snprintf(str, STRING_LENGTH, "hp: %.0f", _enemies[enemy].health);
            DrawTextEx(_fontJapanese, str, (Vector2){ pos.x, pos.y + 50 }, 20, 2, WHITE);
        }

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

    draw_items_UI();

    draw_particles();
}