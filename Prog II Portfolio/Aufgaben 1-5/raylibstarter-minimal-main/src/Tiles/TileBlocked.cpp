//
// Created by 22max on 03.08.2022.
//

#include "TileBlocked.h"

TileBlocked::TileBlocked(Texture2D texture, Texture2D backgroundTex)
{
    this->type = Blocked;
    this->texBackgroundSheet = backgroundTex;
    this->texBackgroundRec.width = this->texBackgroundSheet.width / 8;
    this->texBackgroundRec.height = this->texBackgroundSheet.height / 16;
    this->texBackgroundRec.x = 0;
    this->texBackgroundRec.y = 0;

    this->texSheet = texture;
    this->texRec.width = this->texSheet.width / 16;
    this->texRec.height = this->texSheet.height / 16;
    this->texRec.x = this->texRec.width;
    this->texRec.y = this->texRec.height * 13;
}

void TileBlocked::CustomDraw(Vector2 position)
{
    DrawTextureRec(this->texBackgroundSheet, this->texBackgroundRec, position, WHITE);
    DrawTextureRec(this->texSheet, this->texRec, position, WHITE);
}
