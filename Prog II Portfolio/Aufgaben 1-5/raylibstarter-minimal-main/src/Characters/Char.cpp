//
// Created by 22max on 10.08.2022.
//

#include "Char.h"

void Char::Update()
{
    this->CustomUpdate();
}

void Char::Draw(Vector2 position)
{
    this->CustomDraw(position);
}

void Char::move(Direction direction)
{
    switch (direction)
    {
        case Up:
            if (this->blockUp == false)
            {
                this->position.y--;
            }
            break;
        case Down:
            if (this->blockDown == false)
            {
                this->position.y++;
            }
            break;
        case Left:
            if (this->blockLeft == false)
            {
                this->position.x--;
            }
            break;
        case Right:
            if (this->blockRight == false)
            {
                this->position.x++;
            }
            break;
    }
    this->updateCollision = true;
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

void Char::unblockAll()
{
    this->blockUp = false;
    this->blockDown = false;
    this->blockLeft = false;
    this->blockRight = false;
}

Vector2 Char::getPosition()
{
    return this->position;
}

void Char::setPosition(Vector2 coordinates)
{
    this->position = coordinates;
}

