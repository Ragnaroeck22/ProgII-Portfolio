#pragma once
#include "Tile.h"

class TileTraversable : public Tile
{
	// Attributes
private:



	// Methods
public:
	TileTraversable();

protected:
    void CustomDraw(Vector2 position) override;
};