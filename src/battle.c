#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <raylib.h>
#include <math.h>

#include "main.h"
#include "questions.h"
#include "battle.h"

#include "enemies/enemy1.h"
#include "items/item1.h"

#define MAX_ENEMIES 8
#define MAX_ITEMS 50

sEnemy _enemies [MAX_ENEMIES] = {0};
sItem _inventory [MAX_ITEMS] = {0};
sItem _hand = {0};

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
    return (Vector2){500 + position * 90, 200 + lane * 40};
}

void consume_energy(int energy)
{
    _energy -= energy;
    if (_energy <= 0)
    {
        _energy = _max_energy;
        _turn = -1; //set turn to first enemies
        next_turn();
    }
}

void next_turn()
{
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
    }
    else
        _enemies[_turn].turn(&_enemies[_turn]);
}

void take_damage(int lane, Element element, float damage)
{
    _health -= damage;
}

void Battle_Init()
{
    _battle_screen_texture = LoadTexture("data/screen/battle_screen.png");

    _enemies[0] = _prefab_enemy1;
    _inventory[0] = _prefab_item1;

    _energy = 50;
    _turn = -1;
}

void try_return_item()
{
    if (!_hand.active)
        return;
    
    for(int item = 0; item < MAX_ITEMS; item++)
    {
        if (_inventory[item].active)
            continue;
        
        //copy
        _inventory[item] = _hand;
        _hand.active = false;
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
            snprintf(str, STRING_LENGTH, "%.0f", _enemies[enemy].health);
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
                        if (_hand.active && _hand.effect_enemy)
                        {
                            (*_hand.effect_enemy)(&_hand, &_enemies[enemy]);
                        }
                    }
                }
            }
        }

    }

    //draw items
    int item_counter = 0;
    for(int item = 0; item < MAX_ITEMS; item++)
    {
        if (!_inventory[item].active)
            continue;
        
        int x = item_counter % 5;
        int y = floor(item_counter / 5.0);
        
        item_counter++;

        Vector2 pos = (Vector2){50 + x * 100, 430 + y * 100};

        if (_inventory[item].render)
            _inventory[item].render(&_inventory[item], pos);
        
        //draw energy numbers
        {
            char str[STRING_LENGTH];
            snprintf(str, STRING_LENGTH, "%i", _inventory[item].energy);
            DrawTextEx(_fontJapanese, str, (Vector2){ pos.x, pos.y + 50 }, 20, 2, WHITE);
        }
        
        //item input / mouse behaviour
        {
            if (pos.x < GetMouseX() && GetMouseX() < pos.x + 50)
            {
                if (pos.y < GetMouseY() && GetMouseY() < pos.y + 50)
                {
                    _description = _inventory[item].description;

                    if (!_hand.active && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && _turn == -1 && _inventory[item].energy < _energy)
                    {
                        printf("clicked item!\n");
                        //pick up item
                        _hand = _inventory[item];
                        _inventory[item].active = false;
                        _hand.active = true;
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

    //draw description
    if (_description)
    {
        DrawTextEx(_fontJapanese, _description, (Vector2){638, 385}, 19, 2, WHITE);
    }

    //draw dragged items
    if (_hand.active && _hand.render)
    {
        _hand.render(&_hand, GetMousePosition());
    }

    //drop item with right mouse click
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && _hand.active)
    {
        try_return_item();
    }
}