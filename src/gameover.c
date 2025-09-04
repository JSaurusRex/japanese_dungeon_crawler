#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <raylib.h>
#include <math.h>

#include "main.h"
#include "battle.h"
#include "questions.h"
#include "particles.h"
#include "shadows.h"
#include "hub.h"

#include "sprite_manager.h"

#define MAX_EXIT_SHIELDS 1
#define MAX_EXIT_ITEMS 2

sShield _exit_shields[MAX_EXIT_SHIELDS] = {0};
sItem _exit_items[MAX_EXIT_ITEMS] = {0};

int _exit_shield_index = 0;
int _exit_item_index = 0;

void GameOver_Frame();
void GameOver_item_result();

void Start_GameOver()
{
    for(int i = 0; i < MAX_EXIT_SHIELDS; i++)
        _exit_shields[i].active = false;

    for(int i = 0; i < MAX_EXIT_ITEMS; i++)
        _exit_items[i].active = false;
    
    _exit_item_index = 0;
    _exit_shield_index = 0;
    
    _screen = &GameOver_Frame;
}

bool current_exit_item_exist()
{
    if(_exit_item_index < MAX_EXIT_ITEMS)
        return _exit_items[_exit_item_index].active;
    else
        return _exit_shields[_exit_shield_index].active;
}

void Start_quiz_for_item()
{
    while(_exit_shield_index < MAX_EXIT_SHIELDS)
    {
        if(_exit_item_index < MAX_EXIT_ITEMS)
            _exit_item_index++;
        else
            _exit_shield_index++;
        
        if (_exit_shield_index < MAX_EXIT_SHIELDS && current_exit_item_exist())
            break;
    }

    if (_exit_shield_index >= MAX_EXIT_SHIELDS)
    {
        //empty inventory
        for(int i = 0; i < MAX_ITEMS; i++)
        {
            _inventory[i].active = false;
        }

        for(int i = 0; i < MAX_SHIELDS; i++)
        {
            _shield_inventory[i].active = false;
        }

        for(int i = 0; i < MAX_EXIT_ITEMS; i++)
            _inventory[i] = _exit_items[i];
        
        for(int i = 0; i < MAX_EXIT_SHIELDS; i++)
            _shield_inventory[i] = _exit_shields[i];
        
        _screen = &hub_frame;
        return;    
    }

    char * pack = 0;
    int level = 0;

    if (_exit_item_index < MAX_EXIT_ITEMS)
    {
        pack = _exit_items[_exit_item_index].pack;
        level = _exit_items[_exit_item_index].level;
    }
    else
    {
        pack = _exit_shields[_exit_shield_index].pack;
        level = _exit_shields[_exit_shield_index].level;
    }

    level += 1; //make it harder to keep item

    if(pack[0] == 0)
    {
        printf("error: GameOver_item_result_Frame: pack for item doesn't exist\n");
        GameOver_item_result();
        return;
    }

    Start_Questions(5, 3, pack, level, &GameOver_item_result);
}

void GameOver_item_result_Frame()
{
    if (_answers_incorrect >= _quiz_hearts)
    {
        ClearBackground(RED);
        DrawTextEx(_fontJapanese, "You lost the item", (Vector2){ 240, 200 }, 70, 2, WHITE);
    }
    else
    {
        ClearBackground(GREEN);
        DrawTextEx(_fontJapanese, "You kept the item!", (Vector2){ 240, 200 }, 70, 2, WHITE);
    }

    //next button
    {
        Vector2 pos = (Vector2){ 640, 100};
        DrawTextEx(_fontJapanese, "next", pos, 30, 2, WHITE);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (pos.x < GetMouseX() && pos.x + 80 > GetMouseX())
            {
                if(pos.y < GetMouseY() && pos.y + 40 > GetMouseY())
                {
                    Start_quiz_for_item();
                }
            }
        }
    }
}

void GameOver_item_result()
{
    _screen = &GameOver_item_result_Frame;

    if (_answers_incorrect >= _quiz_hearts)
    {
        //failed
        if(_exit_item_index < MAX_EXIT_ITEMS)
        {
            _exit_items[_exit_item_index].active = false;
        }else
        {
            _exit_shields[_exit_shield_index].active = false;
        }
    }
    else
    {
        //succeeded
    }
}

void GameOver_Frame()
{
    DrawTexture(_gameover_screen, 0, 0, WHITE);

    DrawTextEx(_fontJapanese, "GameOver", (Vector2){ 240, 20 }, 70, 2, WHITE);
    DrawTextEx(_fontJapanese, "You can keep a few items for next game", (Vector2){ 160, 100 }, 20, 2, WHITE);

    for(int item = 0; item < MAX_EXIT_ITEMS; item++)
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

        if (!_exit_items[item].active)
        {
            bool interactable = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
            if (mouse_inside && _item_hand.active && interactable)
            {
                _exit_items[item] = _item_hand;
                _item_hand.active = false;
            }

            continue;
        }


        draw_item(pos, &_exit_items[item]);
        
        //item input / mouse behaviour
        if (mouse_inside)
        {
            _description = _exit_items[item].description;

            bool interactable = _exit_items[item].rounds_disabled <= 0&& IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

            if (!_item_hand.active && !_shield_hand.active && interactable)
            {
                printf("clicked item!\n");
                //pick up item
                _item_hand = _exit_items[item];
                _exit_items[item].active = false;
                _item_hand.active = true;
            }

            if (_item_hand.active && interactable && _item_hand.effect_item)
            {
                _item_hand.effect_item(&_item_hand, &_exit_items[item]);
            }
        }
    }

    for(int shield = 0; shield < MAX_EXIT_SHIELDS; shield++)
    {
        Vector2 pos = (Vector2){450 + shield * 80, 200};

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

        if (!_exit_shields[shield].active)
        {
            if (mouse_inside && _shield_hand.active && interactable)
            {
                _exit_shields[shield] = _shield_hand;
                _shield_hand.active = false;
            }
            continue;
        }

        if (_exit_shields[shield].render)
            _exit_shields[shield].render(&_exit_shields[shield], pos);
        
        if (interactable && mouse_inside && !_shield_hand.active)
        {
            _shield_hand = _exit_shields[shield];
            _exit_shields[shield].active = false;
        }
    }

    //draw description
    if (_description)
    {
        DrawTextEx(_fontJapanese, _description, (Vector2){638, 385}, 19, 2, WHITE);
    }

    //exit button
    {
        Vector2 pos = (Vector2){ 640, 100};
        DrawTextEx(_fontJapanese, "exit", pos, 30, 2, WHITE);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (pos.x < GetMouseX() && pos.x + 80 > GetMouseX())
            {
                if(pos.y < GetMouseY() && pos.y + 40 > GetMouseY())
                {
                    printf("Exited to main lobby!\n");
                    Start_quiz_for_item();
                }
            }
        }
    }


    draw_inventory();

    draw_items_UI();
}