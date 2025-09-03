#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <raylib.h>
#include <math.h>

#include "main.h"
#include "questions.h"
#include "battle.h"

#define MAX_ENEMIES 8
#define MAX_ITEMS 50

typedef enum {TYPE_ENEMY, TYPE_ITEM, TYPE_AMOUNT} Type;

typedef struct sEnemy sEnemy;

struct sEnemy
{
    bool active;
    float health;
    int lane;

    void (*render)(sEnemy *, int);
    void (*turn)(sEnemy *);
    void (*takeDamage)(sEnemy *, float);
};

sEnemy _enemies [MAX_ENEMIES] = {0};

typedef struct sItem sItem;

struct sItem
{
    bool active;
    void (*render)(sItem *, Vector2);
    void (*effect_enemy)(sItem *, sEnemy*);
};

sItem _inventory [MAX_ITEMS] = {0};
sItem _hand = {0};

Texture2D _battle_screen_texture;

float _health = 100;

void try_return_item();

Vector2 CalculateEnemyPosition(int lane, int position)
{
    return (Vector2){500 + position * 90, 200 + lane * 40};
}

void EnemyRender(sEnemy * self, int position)
{
    Vector2 pos = CalculateEnemyPosition(self->lane, position);
    DrawRectangle(pos.x, pos.y, 50, 50, BROWN);
}

void EnemyTurn(sEnemy * self)
{
    printf("doing enemy turn!\n");
}

void EnemyTakeDamage(sEnemy * self, float damage)
{
    printf("enemy took %.2f damage\n", damage);
    self->health -= damage;
    if (self->health <= 0)
        self->active = false;
}

void ItemRender(sItem * self, Vector2 position)
{
    DrawRectangle(position.x, position.y, 50, 50, GRAY);
}

sEnemy * _pEnemy = 0;
sItem * _pItem = 0;

void Item_effect_enemy_finish()
{
    if (_answers_incorrect < _quiz_hearts)
        (*_pEnemy->takeDamage)(_pEnemy, 5);
    _screen = &Battle_Frame;
    try_return_item();
}

void Item_effect_enemy(sItem * self, sEnemy * pEnemy)
{
    if (!pEnemy)
    {
        printf("error: Item_effect_enemy: pEnemy is null\n");
        return;
    }

    if (!pEnemy->active)
    {
        if (_printDebug)
            printf("debug: Item_effect_enemy: clicked on inactive enemy\n");
        return;
    }

    _pEnemy = pEnemy;
    Start_Questions(5, 3, "hiragana", &Item_effect_enemy_finish);
}

void Battle_Init()
{
    _battle_screen_texture = LoadTexture("data/screen/battle_screen.png");

    _enemies[0].active = true;
    _enemies[0].health = 10;
    _enemies[0].render = &EnemyRender;
    _enemies[0].turn = &EnemyTurn;
    _enemies[0].takeDamage = &EnemyTakeDamage;

    _inventory[0].active = true;
    _inventory[0].render = &ItemRender;
    _inventory[0].effect_enemy = &Item_effect_enemy;
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
    DrawTexture(_battle_screen_texture, 0, 0, WHITE);

    //draw enemies
    for(int enemy = 0; enemy < MAX_ENEMIES; enemy++)
    {
        if (!_enemies[enemy].active)
            continue;
        
        
        _enemies[enemy].render(&_enemies[enemy], 0);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 pos = CalculateEnemyPosition(_enemies[enemy].lane, 0);
            if (pos.x - 50 < GetMouseX() && GetMouseX() < pos.x + 50)
            {
                if (pos.y - 50 < GetMouseY() && GetMouseY() < pos.y + 50)
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
        
        if (!_hand.active && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (pos.x < GetMouseX() && GetMouseX() < pos.x + 50)
            {
                if (pos.y < GetMouseY() && GetMouseY() < pos.y + 50)
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

    //draw dragged items
    if (_hand.active && _hand.render)
    {
        _hand.render(&_hand, GetMousePosition());
    }
}