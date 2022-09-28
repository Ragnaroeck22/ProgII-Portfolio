//
// Created by 22max on 23.09.2022.
//

#include "BoneNecklace.h"

BoneNecklace::BoneNecklace(int posX, int posY)
{
    this->position = {(float) posX, (float) posY};
    Image workingImg = LoadImage("assets/graphics/jewelry.png");
    ImageResize(&workingImg, 200, 50);
    this->texSheet = LoadTextureFromImage(workingImg);
    this->texRec.width = this->texSheet.width / 4;
    this->texRec.height = this->texSheet.height;
    this->texRec.x = texRec.width * 3;
    this->texRec.y = 0;
    this->type = TypeNecklace;
    this->weight = 0.35;
    this->price = 180;
    this->name = "Bone necklace";
    description = "Extra swag, but at\nwhat cost?";
}
