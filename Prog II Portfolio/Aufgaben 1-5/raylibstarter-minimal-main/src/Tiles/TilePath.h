//
// Created by 22max on 07.09.2022.
//

#pragma once
#include "Tile.h"

class TilePath : public Tile {
    // Attributes
private:
    Rectangle texBackgroundRec;

    // Methods
public:
    TilePath(Texture2D texture);

protected:
    void CustomDraw(Vector2 position) override;
};
