//
// Created by 22max on 23.09.2022.
//

#include "Spear.h"

Spear::Spear(int posX, int posY)
{
    this->position = {(float) posX, (float) posY};
    Image workingImg = LoadImage("assets/graphics/weapons.png");
    ImageResize(&workingImg, 1200, 1000);
    this->texSheet = LoadTextureFromImage(workingImg);
    this->texRec.width = this->texSheet.width / 24;
    this->texRec.height = this->texSheet.height / 20;
    this->texRec.x = this->texRec.width * 15;
    this->texRec.y = 0;
    this->type = TypeWeapon;
    this->weight = 0.9;
    this->price = 95;
    this->name = "Spear";
}
