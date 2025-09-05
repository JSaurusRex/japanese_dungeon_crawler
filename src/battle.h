#ifndef BATTLE_H
#define BATTLE_H

#include <raylib.h>
#include <stdbool.h>

#include "main.h"

void Battle_Init();
void Battle_Reset();
void Battle_Frame();
void try_return_item();
void next_turn();
void consume_energy(int energy);
Vector2 CalculateEnemyPosition(int lane, int position);
void draw_inventory();
void draw_items_UI();

#define MAX_ENEMIES 8
#define MAX_ITEMS 7
#define MAX_LANES 3
#define MAX_SHIELDS 5

typedef enum {TYPE_ENEMY, TYPE_ITEM, TYPE_AMOUNT} Type;

typedef enum {ELEMENT_NONE, ELEMENT_WIND, ELEMENT_WATER, ELEMENT_FIRE, ELEMENT_EARTH} Element;

typedef struct sEnemy sEnemy;

struct sEnemy
{
    bool active;
    float health;
    int lane;

    float shake_timer;
    float attack_animation_timer;

    void (*render)(sEnemy *, int);
    void (*turn)(sEnemy *);
    void (*takeDamage)(sEnemy *, float, Element);

    char * description;
    Vector2 lastPosition;
};

typedef struct sItem sItem;
typedef struct sShield sShield;

struct sItem
{
    bool active;
    int energy;
    float shake_timer;
    void (*render)(sItem *, Vector2);
    void (*effect_enemy)(sItem *, sEnemy*);
    void (*effect_shield)(sItem*, sShield*);
    void (*effect_item)(sItem*, sItem*);
    void (*effect_player)(sItem *);
    float enhanced;
    int rounds_disabled;
    int level;

    char * description;
    char pack[STRING_LENGTH];
};


struct sShield
{
    bool active;
    float health;
    int max_health;
    float shake_timer;
    void (*render)(sShield *, Vector2);
    void (*take_damage)(sShield *, sEnemy*, Element, float);
    int level;

    char * description;
    Vector2 lastPosition;
    char pack[STRING_LENGTH];
};

extern float _health;
extern char * _description;
extern sItem _item_hand;
extern sShield _shield_hand;
extern sItem _inventory [MAX_ITEMS];
extern sShield _shield_inventory [MAX_SHIELDS];
extern float _battle_timer;
extern int _level;
extern int _turn;


void heal_player(float amount);
void take_damage(int lane, Element element, float damage);
void draw_item(Vector2 pos, sItem * pItem);
void Battle_Start();

#endif