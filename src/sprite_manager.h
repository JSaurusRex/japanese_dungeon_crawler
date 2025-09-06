#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H

#include <raylib.h>

#define SPRITE(var, path) extern Texture2D var;

#include "sprites.h"

void sprite_manager_init();
void reload_all_sprites();

#endif