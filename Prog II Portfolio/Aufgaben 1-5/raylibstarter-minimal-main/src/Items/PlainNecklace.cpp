//
// Created by 22max on 23.09.2022.
//

#include "PlainNecklace.h"

PlainNecklace::PlainNecklace(int posX, int posY)
{
    this->position = {(float) posX, (float) posY};
    Image workingImg = LoadImage("assets/graphics/jewelry.png");
    ImageResize(&workingImg, 200, 50);
    this->texSheet = LoadTextureFromImage(workingImg);
    this->texRec.width = this->texSheet.width / 4;
    this->texRec.height = this->texSheet.height;
    this->texRec.x = texRec.width * 2;
    this->texRec.y = 0;
    this->type = TypeNecklace;
    this->weight = 0.3;
    this->price = 115;
    this->name = "Plain necklace";
    description = "An old necklace, usually worn by grandmas";
}
