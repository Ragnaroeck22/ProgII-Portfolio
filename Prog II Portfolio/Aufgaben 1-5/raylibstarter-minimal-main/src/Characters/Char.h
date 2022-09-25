//
// Created by 22max on 10.08.2022.
//

#pragma once
#include "../Enums.h"
#include <raylib.h>

class Char {

    // Attributes
public:
    bool updateCollision;

protected:
    Texture2D texture;
    Rectangle textureRec;

    Vector2 position; // Coordinates on map

    Direction facing;

    // For collision and movement
    bool blockUp;
    bool blockDown;
    bool blockLeft;
    bool blockRight;

    // Methods
public:
    virtual void Update();
    virtual void Draw(Vector2 position);

    virtual void CustomUpdate() = 0;
    virtual void CustomDraw(Vector2 position) = 0;

    virtual void move(Direction direction);

    virtual void blockDirection(Direction direction);
    virtual void unblockAll();

    virtual Vector2 getPosition();
    virtual void setPosition(Vector2 coordinates);
};

