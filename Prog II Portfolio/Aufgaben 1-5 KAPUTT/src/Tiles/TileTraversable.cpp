#include "TileTraversable.h"

TileTraversable::TileTraversable()
{
    this->type = Traversable;
	this->texSheet = LoadTexture("assets/graphics/paths.png");
	this->texRec.width = this->texSheet.width / 8;
	this->texRec.height = this->texSheet.height / 16;
	this->texRec.x = 0;
	this->texRec.y = 0;
}

void TileTraversable::CustomDraw(Vector2 position)
{
    DrawTextureRec(this->texSheet, this->texRec, position, WHITE);
}


