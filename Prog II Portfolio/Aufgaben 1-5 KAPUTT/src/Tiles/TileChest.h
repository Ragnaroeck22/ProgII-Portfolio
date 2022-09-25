//
// Created by 22max on 03.08.2022.
//

#pragma once
#include "Tile.h"

class TileChest : public Tile 
{
	// Methods
public:
	TileChest();

protected:
	void CustomDraw(Vector2 position) override;
};
