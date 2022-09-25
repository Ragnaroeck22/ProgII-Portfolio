#pragma once
#include "raylib.h"
#include "../Enums.h"

class Tile
{
	// Attributes
public:
    Texture2D texSheet;
    Rectangle texRec;
    TileType type;

protected:

	// Methods
public:
    virtual void Draw(Vector2 position);

protected:
    virtual void CustomDraw(Vector2 position) = 0;
};