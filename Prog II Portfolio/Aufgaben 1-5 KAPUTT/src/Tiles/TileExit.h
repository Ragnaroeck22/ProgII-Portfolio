//
// Created by 22max on 03.08.2022.
//

#pragma once
#include "Tile.h"

class TileExit : public Tile {
    // Attributes
private:

    Texture2D texBackgroundSheet;
    Rectangle texBackgroundRec;

    // Methods
public:
    TileExit();

protected:
    void CustomDraw(Vector2 position) override;
};
