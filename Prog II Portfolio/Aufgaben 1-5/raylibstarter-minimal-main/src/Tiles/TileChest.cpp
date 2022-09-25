//
// Created by 22max on 03.08.2022.
//

#include "TileChest.h"

TileChest::TileChest(Texture2D texture, Texture2D texBackground)
{
    this->type = Chest;
    this->texSheet = texture;
    this->texRec.width = this->texSheet.width / 16;
    this->texRec.height = this->texSheet.height / 16;
    this->texRec.x = this->texRec.width * 4;
    this->texRec.y = this->texRec.height * 6;

    this->texBackgroundSheet = texBackground;
    this->texBackgroundRec.width = this->texBackgroundSheet.width / 8;
    this->texBackgroundRec.height = this->texBackgroundSheet.height / 16;
    this->texBackgroundRec.x = 0;
    this->texBackgroundRec.y = 0;
}

void TileChest::CustomDraw(Vector2 position)
{
    DrawTextureRec(this->texBackgroundSheet, this->texBackgroundRec, position, WHITE);
    DrawTextureRec(this->texSheet, this->texRec, position, WHITE);
}
