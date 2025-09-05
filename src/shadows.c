#include <stdlib.h>
#include <raylib.h>
#include <stdbool.h>

#include "rendering.h"

#include "sprite_manager.h"

typedef struct
{
    Vector2 position;
    float size;
    bool perspective;
} sShadow;

#define MAX_SHADOWS 100

sShadow _shadows [MAX_SHADOWS] = {0};
int _shadow_counter = 0;

void add_shadow(Vector2 position, float size, bool perspective)
{
    if (_shadow_counter >= MAX_SHADOWS)
        return;
    
    _shadows[_shadow_counter].position = position;
    _shadows[_shadow_counter].size = size;
    _shadows[_shadow_counter].perspective = perspective;

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
        Texture2D * tex = &_sprite_shadow;

        if (_shadows[i].perspective)
            tex = &_sprite_shadow_perspective;
        drawTextureEx(*tex, (Vector2){_shadows[i].position.x - _shadows[i].size * 42, _shadows[i].position.y}, 0, _shadows[i].size * 0.5, WHITE);
    }

    clear_shadows();
}