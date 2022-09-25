//
// Created by 22max on 07.09.2022.
//

#include "TilePath.h"

TilePath::TilePath(Texture2D texture)
{
    this->type = Path;
    this->texSheet = texture;
    this->texRec.width = this->texSheet.width / 8;
    this->texRec.height = this->texSheet.height / 16;
    this->texRec.x = 0;
    this->texRec.y = 2 * this->texRec.height;

    this->texBackgroundRec.width = this->texSheet.width / 8;
    this->texBackgroundRec.height = this->texSheet.height / 16;
    this->texBackgroundRec.x = 0;
    this->texBackgroundRec.y = 0;


}

void TilePath::CustomDraw(Vector2 position)
{
    if (IsKeyDown(KEY_L))
    {
        DrawTextureRec(this->texSheet, this->texRec, position, WHITE);
    }
    else
    {
        DrawTextureRec(this->texSheet, this->texBackgroundRec, position, WHITE);
    }
}
