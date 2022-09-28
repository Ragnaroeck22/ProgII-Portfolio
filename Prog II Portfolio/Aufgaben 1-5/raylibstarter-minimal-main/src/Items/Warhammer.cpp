//
// Created by 22max on 28.09.2022.
//

#include "Warhammer.h"

Warhammer::Warhammer(int posX, int posY)
{
    this->position = {(float) posX, (float) posY};
    Image workingImg = LoadImage("assets/graphics/weapons.png");
    ImageResize(&workingImg, 1200, 1000);
    this->texSheet = LoadTextureFromImage(workingImg);
    this->texRec.width = this->texSheet.width / 24;
    this->texRec.height = this->texSheet.height / 20;
    this->texRec.x = texRec.width * 21;
    this->texRec.y = texRec.height * 11;
    this->type = TypeWeapon;
    this->weight = 1.8;
    this->price = 85;
    this->name = "Warhammer";
    description = "Awesome for bashing\nin skulls!";
}
