//
// Created by 22max on 28.09.2022.
//

#include "Flamberge.h"

Flamberge::Flamberge(int posX, int posY)
{
    this->position = {(float) posX, (float) posY};
    Image workingImg = LoadImage("assets/graphics/weapons.png");
    ImageResize(&workingImg, 1200, 1000);
    this->texSheet = LoadTextureFromImage(workingImg);
    this->texRec.width = this->texSheet.width / 24;
    this->texRec.height = this->texSheet.height / 20;
    this->texRec.x = 0;
    this->texRec.y = texRec.height * 6;
    this->type = TypeWeapon;
    this->weight = 1.4;
    this->price = 130;
    this->name = "Flamberge";
    description = "The most stylish sword\never made";
}
