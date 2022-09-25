//
// Created by 22max on 03.08.2022.
//

#include "TileStart.h"

TileStart::TileStart()
{
    this->type = Start;
    this->texBackgroundSheet = LoadTexture("assets/graphics/paths.png");
    this->texBackgroundRec.width = this->texBackgroundSheet.width / 8;
    this->texBackgroundRec.height = this->texBackgroundSheet.height / 16;
    this->texBackgroundRec.x = 0;
    this->texBackgroundRec.y = 0;

    this->texSheet = LoadTexture("assets/graphics/obstacles.png");
    this->texRec.width = this->texSheet.width / 16;
    this->texRec.height = this->texSheet.height / 16;
    this->texRec.x = this->texRec.width * 2;
    this->texRec.y = this->texRec.height * 9;
}

void TileStart::CustomDraw(Vector2 position)
{
    DrawTextureRec(this->texBackgroundSheet, this->texBackgroundRec, position, WHITE);
    DrawTextureRec(this->texSheet, this->texRec, position, WHITE);
}
