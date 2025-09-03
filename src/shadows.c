#include <stdlib.h>
#include <raylib.h>

#include "sprite_manager.h"

typedef struct
{
    Vector2 position;
    float size;
} sShadow;

#define MAX_SHADOWS 100

sShadow _shadows [MAX_SHADOWS] = {0};
int _shadow_counter = 0;

void add_shadow(Vector2 position, float size)
{
    if (_shadow_counter >= MAX_SHADOWS)
        return;
    
    _shadows[_shadow_counter].position = position;
    _shadows[_shadow_counter].size = size;

    _shadow_counter++;
}

void clear_shadows()
{
    _shadow_counter = 0;
}

void render_shadows()
{
    for(int i = 0; i < _shadow_counter && i < MAX_SHADOWS; i++)
    {
        DrawTextureEx(_sprite_shadow, (Vector2){_shadows[i].position.x - _shadows[i].size * 42, _shadows[i].position.y}, 0, _shadows[i].size * 0.5, WHITE);
    }

    clear_shadows();
}