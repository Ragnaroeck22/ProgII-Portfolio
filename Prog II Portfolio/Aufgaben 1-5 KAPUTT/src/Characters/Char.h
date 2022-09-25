//
// Created by 22max on 10.08.2022.
//

#pragma once
#include "../Enums.h"
#include <raylib.h>

class Char {

    // Attributes
protected:
    Vector2 position;

    Direction facing;

    // For collision
    bool blockUp;
    bool blockDown;
    bool blockLeft;
    bool blockRight;

    // Methods
public:
    virtual void Update();
    virtual void Draw();

    virtual void CustomUpdate() = 0;
    virtual void CustomDraw() = 0;

    virtual void move(Direction direction);

    virtual void blockDirection(Direction direction);
};

