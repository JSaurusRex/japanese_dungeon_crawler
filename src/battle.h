#ifndef BATTLE_H
#define BATTLE_H

#include <raylib.h>
#include <stdbool.h>

void Battle_Init();
void Battle_Frame();
void try_return_item();
void next_turn();
void consume_energy(int energy);
Vector2 CalculateEnemyPosition(int lane, int position);

#define MAX_ENEMIES 8
#define MAX_ITEMS 50
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

    void (*render)(sEnemy *, int);
    void (*turn)(sEnemy *);
    void (*takeDamage)(sEnemy *, float, Element);

    char * description;
};

typedef struct sItem sItem;

struct sItem
{
    bool active;
    int energy;
    void (*render)(sItem *, Vector2);
    void (*effect_enemy)(sItem *, sEnemy*);

    char * description;
};

typedef struct sShield sShield;

struct sShield
{
    bool active;
    float health;
    void (*render)(sShield *, Vector2);
    void (*take_damage)(sShield *, sEnemy*, Element, float);

    char * description;
};

extern float _health;
void take_damage(int lane, Element element, float damage);

#endif