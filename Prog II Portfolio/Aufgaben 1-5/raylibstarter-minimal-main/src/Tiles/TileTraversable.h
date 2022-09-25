#pragma once
#include "Tile.h"

class TileTraversable : public Tile
{
    // Attributes
private:


    // Methods
public:
    TileTraversable(Texture2D texture);

protected:
    void CustomDraw(Vector2 position) override;
};