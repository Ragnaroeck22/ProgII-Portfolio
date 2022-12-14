//
// Created by 22max on 03.08.2022.
//

#pragma once
#include "Tile.h"

class TileStart : public Tile {

    // Attributes
private:

    Texture2D texBackgroundSheet;
    Rectangle texBackgroundRec;

    // Methods
public:
    TileStart(Texture2D texture, Texture2D texBackground);

protected:
    void CustomDraw(Vector2 position) override;

};

