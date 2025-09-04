#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H

#include <raylib.h>

extern Texture2D _sprite_shield1;
extern Texture2D _sprite_shadow;
extern Texture2D _next_level_screen;
extern Texture2D _gameover_screen;
extern Texture2D _hub_screen;
extern Texture2D _disabled_slot_sprite;
extern Texture2D _sword1_sprite;

void sprite_manager_init();

#endif