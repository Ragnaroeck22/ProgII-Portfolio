//
// Created by 22max on 03.08.2022.
//

#pragma once
#include "Tile.h"

class TileChest : public Tile
{
    // Attributes
public:
    Texture2D texBackgroundSheet;
    Rectangle texBackgroundRec;

    // Methods
public:
    TileChest(Texture2D texture, Texture2D texBackground);

protected:
    void CustomDraw(Vector2 position) override;
};
