#ifndef RENDERING_H
#define RENDERING_H

#include <raylib.h>

void drawCircle(int x, int y, float radius, Color color);

void drawTextEx(Font font, const char * text, Vector2 position, float fontSize, float spacing, Color tint);

void drawRectangle(int x, int y, int width, int height, Color color);

void drawTexture(Texture2D texture, int x, int y, Color color);

void drawText(const char * text, int x, int y, float fontSize, Color color);

void drawTextureEx(Texture2D texture, Vector2 position, float rotation, float size, Color color);

int getMouseX();
int getMouseY();
Vector2 getMousePosition();

#endif