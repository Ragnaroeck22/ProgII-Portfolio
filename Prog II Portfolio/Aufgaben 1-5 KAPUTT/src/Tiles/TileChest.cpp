//
// Created by 22max on 03.08.2022.
//

#include "TileChest.h"

TileChest::TileChest()
{
	this->type = Chest;
	this->texSheet = LoadTexture("assets/graphics/chest.png");
	this->texRec.width = this->texSheet.width / 9;
	this->texRec.height = this->texSheet.height / 4;
	this->texRec.x = 0;
	this->texRec.y = 0;
}

void TileChest::CustomDraw(Vector2 position)
{
	DrawTextureRec(this->texSheet, this->texRec, position, WHITE);
}
