#include "TileTraversable.h"

TileTraversable::TileTraversable(Texture2D texture)
{
    this->type = Traversable;
    this->texSheet = texture;
    this->texRec.width = this->texSheet.width / 8;
    this->texRec.height = this->texSheet.height / 16;
    this->texRec.x = 0;
    this->texRec.y = 0;
}

void TileTraversable::CustomDraw(Vector2 position)
{
    DrawTextureRec(this->texSheet, this->texRec, position, WHITE);
}


