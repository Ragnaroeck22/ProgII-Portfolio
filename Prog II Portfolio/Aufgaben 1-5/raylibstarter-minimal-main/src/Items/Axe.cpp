//
// Created by 22max on 23.09.2022.
//

#include "Axe.h"

Axe::Axe(int posX, int posY)
{
    this->position = {(float) posX, (float) posY};
    Image workingImg = LoadImage("assets/graphics/weapons.png");
    ImageResize(&workingImg, 1200, 1000);
    this->texSheet = LoadTextureFromImage(workingImg);
    this->texRec.width = this->texSheet.width / 24;
    this->texRec.height = this->texSheet.height / 20;
    this->texRec.x = 0;
    this->texRec.y = texRec.height * 11;
    this->type = TypeWeapon;
    this->weight = 1.5;
    this->price = 70;
    this->name = "Axe";
    description = "Great for chopping firewood and limbs!";
}
