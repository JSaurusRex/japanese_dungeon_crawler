#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include "rendering.h"
#include <raymath.h>

#include "main.h"

Vector2 game_to_window_scaling(float x, float y)
{
    float scale_x = GetRenderWidth() / 800.0;
    float scale_y = GetRenderHeight() / 600.0;

    return (Vector2){x * scale_x, y * scale_y};
}

float game_to_window_scaling_f()
{
    return GetRenderHeight() / 600.0;
}

void drawCircle(int x, int y, float radius, Color color)
{
    Vector2 pos = game_to_window_scaling(x, y);
    DrawCircle(pos.x, pos.y, radius * game_to_window_scaling_f(), color);
}

void drawTextEx(Font font, const char * text, Vector2 position, float fontSize, float spacing, Color tint)
{
    DrawTextEx(_fontJapanese, text, game_to_window_scaling(position.x, position.y), fontSize * game_to_window_scaling_f(), spacing * game_to_window_scaling_f(), tint);
}

void drawRectangle(int x, int y, int width, int height, Color color)
{
    Vector2 pos = game_to_window_scaling(x, y);
    Vector2 size = game_to_window_scaling(width, height);
    DrawRectangle(pos.x, pos.y, size.x, size.y, color);
}

void drawTextureEx(Texture2D texture, Vector2 position, float rotation, float size, Color color)
{
    position = game_to_window_scaling(position.x, position.y);
    Vector2 scale = game_to_window_scaling(size, size);
    Rectangle s_rect = {0, 0, texture.width, texture.height};
    Rectangle d_rect = {position.x, position.y, texture.width*scale.x, texture.height*scale.y};
    DrawTexturePro(texture, s_rect, d_rect, (Vector2){0, 0}, rotation, color);
    // DrawTextureEx(texture, game_to_window_scaling(position.x, position.y), rotation, size * game_to_window_scaling_f(), color);
}

void drawTexture(Texture2D texture, int x, int y, Color color)
{
    drawTextureEx(texture, (Vector2){x, y}, 0, 1, color);
}

void drawText(const char * text, int x, int y, float fontSize, Color color)
{
    Vector2 pos = game_to_window_scaling(x, y);
    DrawText(text, pos.x, pos.y, fontSize * game_to_window_scaling_f(), color);
}

int getMouseX()
{
    return getMousePosition().x;
}

int getMouseY()
{
    return getMousePosition().y;
}

Vector2 getMousePosition()
{
    return Vector2Multiply(GetMousePosition(), (Vector2){800.0/GetRenderWidth(), 600.0/GetRenderHeight()});
}
