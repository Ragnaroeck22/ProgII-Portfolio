//
// Created by 22max on 03.08.2022.
//

#pragma once
#include <string>
#include <raylib.h>
#include "../Enums.h"

class ItemBase {

public:
    // Everything regarding textures
    Texture2D texSheet;
    Rectangle texRec;

    ItemType type;

    std::string name;
    std::string description;

    // Attributes
    float weight;
    float price;
    float strength = 0;

    Vector2 position; // Coordinates on map

    // Methods
    virtual void Draw(Vector2 position);

};


