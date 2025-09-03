#include <stdlib.h>
#include <raylib.h>


Texture2D _sprite_shield1;
Texture2D _sprite_shadow;

void sprite_manager_init()
{
    _sprite_shield1 = LoadTexture("data/sprites/shields/shield1.png");
    _sprite_shadow = LoadTexture("data/sprites/misc/shadow.png");
}