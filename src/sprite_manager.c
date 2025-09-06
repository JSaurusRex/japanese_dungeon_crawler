#include <stdlib.h>
#include <raylib.h>
#include "rendering.h"

#define SPRITE(var, path) Texture2D var;

#include "sprites.h"

#undef SPRITE
#define SPRITE(var, path) var = LoadTexture(path);

void sprite_manager_init()
{
    #include "sprites.h"
}

void reload_all_sprites()
{
    #undef SPRITE
    #define SPRITE(var, path) UnloadTexture(var);
    #include "sprites.h"
    
    #undef SPRITE
    #define SPRITE(var, path) var = LoadTexture(path);
    #include "sprites.h"
}