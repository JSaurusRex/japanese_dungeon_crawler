#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <raylib.h>
#include <raymath.h>
#include <math.h>

#include "main.h"
#include "sprite_manager.h"
#include "battle.h"

#define LOOT_MAX 14
#define LOOT_SHIELDS_MAX 7

sItem _loot[LOOT_MAX] = {0};
sShield _loot_shields[LOOT_SHIELDS_MAX] = {0};

void next_level_frame()
{
    DrawTexture(_next_level_screen, 0, 0, WHITE);

    draw_inventory();

    for(int item = 0; item < LOOT_MAX; item++)
    {
        int x = item % 7;
        int y = floor(item / 7.0);

        Vector2 pos = (Vector2){150 + x * 80, 200 + y * 100};

        DrawCircle(pos.x + 25, pos.y + 25, 40, ColorAlpha(BLACK, 0.3));

        bool mouse_inside = false;
        if (pos.x < GetMouseX() && GetMouseX() < pos.x + 50)
        {
            if (pos.y < GetMouseY() && GetMouseY() < pos.y + 50)
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

        DrawCircle(pos.x+25, pos.y+25, 10, YELLOW);

        bool mouse_inside = false;
        if (pos.x < GetMouseX() && GetMouseX() < pos.x + 50)
        {
            if (pos.y < GetMouseY() && GetMouseY() < pos.y + 50)
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
        
        if (interactable && mouse_inside && !_shield_hand.active)
        {
            _shield_hand = _loot_shields[shield];
            _loot_shields[shield].active = false;
        }
    }

    //draw description
    if (_description)
    {
        DrawTextEx(_fontJapanese, _description, (Vector2){638, 385}, 19, 2, WHITE);
    }

    //next level button
    {
        Vector2 pos = (Vector2){ 120, 10 };
        DrawTextEx(_fontJapanese, "next level", pos, 30, 2, BLACK);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (pos.x < GetMouseX() && pos.x + 80 > GetMouseX())
            {
                if(pos.y < GetMouseY() && pos.y + 40 > GetMouseY())
                {
                    printf("pressed next level!\n");
                    Battle_Start();
                }
            }
        }
    }

    //you won text
    {
        Vector2 pos = (Vector2){ 350, 10 };
        char str[STRING_LENGTH];
        snprintf(str, STRING_LENGTH, "completed level %i\npick your loot", _level);
        DrawTextEx(_fontJapanese, str, pos, 30, 2, BLACK);
    }

    draw_items_UI();
}

void next_level_generate()
{
    _screen = &next_level_frame;
}