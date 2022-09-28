//
// Created by 22max on 23.09.2022.
//

#include "RingMajor.h"

RingMajor::RingMajor(int posX, int posY)
{
    this->position = {(float) posX, (float) posY};
    Image workingImg = LoadImage("assets/graphics/jewelry.png");
    ImageResize(&workingImg, 200, 50);
    this->texSheet = LoadTextureFromImage(workingImg);

    this->texRec.width = this->texSheet.width / 4;
    this->texRec.height = this->texSheet.height;
    this->texRec.x = texRec.width;
    this->texRec.y = 0;
    this->type = TypeRing;
    this->weight = 0.2;
    this->price = 175;
    this->strength = 5;
    this->name = "Ring of major strength";
    description = "Better than protein powder";
}
