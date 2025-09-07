#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <raylib.h>
#include "rendering.h"
#include <raymath.h>
#include <math.h>

#include "main.h"
#include "sprite_manager.h"
#include "battle.h"
#include "inbetween_screen.h"
#include "shadows.h"

#include "items/attacks/sword1.h"
#include "items/shield_repair_item.h"
#include "items/item_enhancer.h"
#include "items/attacks/fire_wand1.h"
#include "items/healing_potion.h"
#include "items/energy_potion.h"
#include "shields/shield1.h"
#include "items/upgrade_item.h"

#include "next_level_screen.h"

sItem _loot[LOOT_MAX] = {0};
sShield _loot_shields[LOOT_SHIELDS_MAX] = {0};

void (*_dungeon_generate_loot)();

void next_level_frame()
{
    _turn = -1;
    _energy = _max_energy;
    drawTexture(_next_level_screen, 0, 0, WHITE);

    render_shadows();

    draw_inventory();

    for(int item = 0; item < LOOT_MAX; item++)
    {
        int x = item % 7;
        int y = floor(item / 7.0);

        Vector2 pos = (Vector2){150 + x * 80, 200 + y * 100};

        drawCircle(pos.x + 25, pos.y + 25, 40, ColorAlpha(BLACK, 0.3));

        bool mouse_inside = false;
        if (pos.x < getMouseX() && getMouseX() < pos.x + 50)
        {
            if (pos.y < getMouseY() && getMouseY() < pos.y + 50)
            {
                mouse_inside = true;
            }
        }

        if (!_loot[item].active)
        {
            bool interactable = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
            if (mouse_inside && _item_hand.active && interactable)
            {
                _loot[item] = _item_hand;
                _item_hand.active = false;
            }

            continue;
        }


        draw_item(pos, &_loot[item]);
        
        //item input / mouse behaviour
        if (mouse_inside)
        {
            _description = _loot[item].description;
            _hovered_item_pack = _loot[item].pack;

            bool interactable = _loot[item].rounds_disabled <= 0&& IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

            if (!_item_hand.active && !_shield_hand.active && interactable)
            {
                printf("clicked item!\n");
                //pick up item
                _item_hand = _loot[item];
                _loot[item].active = false;
                _item_hand.active = true;
            }

            if (_item_hand.active && interactable && _item_hand.effect_item)
            {
                _item_hand.effect_item(&_item_hand, &_loot[item]);
            }
        }
    }

    for(int shield = 0; shield < LOOT_SHIELDS_MAX; shield++)
    {
        Vector2 pos = (Vector2){150 + shield * 80, 100};

        drawCircle(pos.x+25, pos.y+25, 10, ColorAlpha(WHITE, 0.3));

        bool mouse_inside = false;
        if (pos.x < getMouseX() && getMouseX() < pos.x + 50)
        {
            if (pos.y < getMouseY() && getMouseY() < pos.y + 50)
            {
                mouse_inside = true;
            }
        }

        bool interactable = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        if (!_loot_shields[shield].active)
        {
            if (mouse_inside && _shield_hand.active && interactable)
            {
                _loot_shields[shield] = _shield_hand;
                _shield_hand.active = false;
            }
            continue;
        }

        if (_loot_shields[shield].render)
            _loot_shields[shield].render(&_loot_shields[shield], pos);
        
        //draw health numbers
        {
            char str[STRING_LENGTH];
            snprintf(str, STRING_LENGTH, "hp:%.0f", _loot_shields[shield].health);
            drawTextEx(_fontJapanese, str, (Vector2){ pos.x, pos.y + 40}, 20, 2, WHITE, true);
        }
        
        if (interactable && mouse_inside && !_shield_hand.active && !_item_hand.active)
        {
            _shield_hand = _loot_shields[shield];
            _loot_shields[shield].active = false;
        }
    }

    //next level button
    {
        Vector2 pos = (Vector2){ 120, 10 };
        drawTextEx(_fontJapanese, "next level", pos, 30, 2, WHITE, true);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (pos.x < getMouseX() && pos.x + 80 > getMouseX())
            {
                if(pos.y < getMouseY() && pos.y + 40 > getMouseY())
                {
                    printf("pressed next level!\n");
                    _level++;
                    _inbetween_timer = 0;
                    change_screen(&inbetween_screen_frame, true);
                }
            }
        }
    }

    //skip 7 levels button
    if((_level - 3) % 10 == 0)
    {
        Vector2 pos = (Vector2){ 240, 10 };
        drawTextEx(_fontJapanese, "skip 7 levels", pos, 30, 2, WHITE, true);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (pos.x < getMouseX() && pos.x + 80 > getMouseX())
            {
                if(pos.y < getMouseY() && pos.y + 40 > getMouseY())
                {
                    printf("pressed skip 7 levels!\n");
                    _level += 7;
                    _inbetween_timer = 0;
                    change_screen(&inbetween_screen_frame, true);
                }
            }
        }
    }

    //you won text
    {
        Vector2 pos = (Vector2){ 350, 10 };
        char str[STRING_LENGTH];
        snprintf(str, STRING_LENGTH, "completed level %i\npick your loot", _level);
        drawTextEx(_fontJapanese, str, pos, 30, 2, WHITE, true);
    }

    draw_items_UI();
}

void generate_loot(sItemSpawn * itemspawn_table, int size, int amount)
{
    int total_chance = 0;
    for(int i = 0; i < size; i++)
    {
        total_chance += itemspawn_table[i].chance * 100;
    }

    //choose amount random items
    for(int i = 0; i < amount; i++)
    {
        int number = rand() % total_chance;

        int chance = 0;
        for(int item = 0; item < size; item++)
        {
            chance += itemspawn_table[item].chance * 100;
            if (number > chance)
                continue;
            
            //find empty spot to put item in
            if (itemspawn_table[item].pItem)
            {
                for(int spot = 0; spot < LOOT_MAX; spot++)
                {
                    if (_loot[spot].active)
                        continue;
                    
                    _loot[spot] = *itemspawn_table[item].pItem;
                    break;
                }
            }

            if (itemspawn_table[item].pShield)
            {
                for(int spot = 0; spot < LOOT_SHIELDS_MAX; spot++)
                {
                    if (_loot_shields[spot].active)
                        continue;
                    
                    _loot_shields[spot] = *itemspawn_table[item].pShield;
                    break;
                }
            }

            break;
        }
    }
}

void next_level_generate()
{
    change_screen(&next_level_frame, true);

    //empty existing loot
    for(int i = 0; i < LOOT_MAX; i++)
        _loot[i].active = false;
    
    for(int i = 0; i < LOOT_SHIELDS_MAX; i++)
        _loot_shields[i].active = false;
    
    _dungeon_generate_loot();
}