//
// Created by 22max on 23.09.2022.
//

#include "RingMinor.h"

RingMinor::RingMinor(int posX, int posY)
{
    this->position = {(float) posX, (float) posY};
    Image workingImg = LoadImage("assets/graphics/jewelry.png");
    ImageResize(&workingImg, 200, 50);
    this->texSheet = LoadTextureFromImage(workingImg);
    this->texRec.width = this->texSheet.width / 4;
    this->texRec.height = this->texSheet.height;
    this->texRec.x = 0;
    this->texRec.y = 0;
    this->type = TypeRing;
    this->weight = 0.15;
    this->price = 125;
    this->strength = 2;
    this->name = "Ring of minor strength";
    description = "Grants you the strength of ten really puny men";
}
