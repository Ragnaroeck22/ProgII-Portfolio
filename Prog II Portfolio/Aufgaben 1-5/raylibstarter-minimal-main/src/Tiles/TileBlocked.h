//
// Created by 22max on 03.08.2022.
//

#pragma once
#include "Tile.h"

class TileBlocked : public Tile {

    // Attributes
private:

    Texture2D texBackgroundSheet;
    Rectangle texBackgroundRec;

    // Methods
public:
    TileBlocked(Texture2D texture, Texture2D backgroundTex);

protected:
    void CustomDraw(Vector2 position) override;

};

