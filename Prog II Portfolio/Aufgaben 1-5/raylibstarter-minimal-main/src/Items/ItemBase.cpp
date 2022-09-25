//
// Created by 22max on 03.08.2022.
//

#include "ItemBase.h"

void ItemBase::Draw(Vector2 position)
{
    DrawTextureRec(this->texSheet, this->texRec, position, WHITE);
}
