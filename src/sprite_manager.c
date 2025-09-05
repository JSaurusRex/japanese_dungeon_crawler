#include <stdlib.h>
#include <raylib.h>
#include "rendering.h"


Texture2D _sprite_shield1;
Texture2D _sprite_shadow;
Texture2D _next_level_screen;
Texture2D _gameover_screen;
Texture2D _hub_screen;
Texture2D _disabled_slot_sprite;
Texture2D _sword1_sprite;
Texture2D _firewand1_sprite;
Texture2D _goblin1_sprite;
Texture2D _inbetween_screen;

void sprite_manager_init()
{
    _sprite_shield1 = LoadTexture("data/sprites/shields/shield1.png");
    _sprite_shadow = LoadTexture("data/sprites/misc/shadow.png");
    _next_level_screen = LoadTexture("data/screen/next_level_screen.png");
    _gameover_screen = LoadTexture("data/screen/gameover_screen.png");
    _hub_screen = LoadTexture("data/screen/hub_screen.png");
    _disabled_slot_sprite = LoadTexture("data/sprites/misc/disabled_slot.png");
    _sword1_sprite = LoadTexture("data/sprites/attacks/sword1.png");
    _firewand1_sprite = LoadTexture("data/sprites/attacks/firewand1.png");
    _inbetween_screen = LoadTexture("data/screen/inbetween.png");
    _goblin1_sprite = LoadTexture("data/sprites/enemies/goblin1.png");
}