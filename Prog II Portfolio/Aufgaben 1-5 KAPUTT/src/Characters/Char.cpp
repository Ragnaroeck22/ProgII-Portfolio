//
// Created by 22max on 10.08.2022.
//

#include "Char.h"

void Char::Update()
{
    this->CustomUpdate();
}

void Char::Draw()
{
    this->CustomDraw();
}

void Char::move(Direction direction)
{

}

void Char::blockDirection(Direction direction)
{
    switch(direction)
    {
        case Up:
            this->blockUp = true;
            break;
        case Down:
            this->blockDown = true;
            break;
        case Left:
            this->blockLeft = true;
            break;
        case Right:
            this->blockRight = true;
    }
}
