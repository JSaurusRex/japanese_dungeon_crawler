#ifndef SHADOW_H
#define SHADOW_H

#include <raylib.h>

void add_shadow(Vector2 position, float size, bool perspective);
void render_shadows();

#endif