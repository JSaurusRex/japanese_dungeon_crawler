#include <stdlib.h>
#include <raylib.h>


Texture2D _sprite_shield1;
Texture2D _sprite_shadow;
Texture2D _next_level_screen;

void sprite_manager_init()
{
    _sprite_shield1 = LoadTexture("data/sprites/shields/shield1.png");
    _sprite_shadow = LoadTexture("data/sprites/misc/shadow.png");
    _next_level_screen = LoadTexture("data/screen/next_level_screen.png");
}