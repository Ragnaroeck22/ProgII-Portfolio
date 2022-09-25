//
// Created by 22max on 03.08.2022.
//

#pragma once
#include <string>
#include <raylib.h>

class ItemBase {

public:
    float weight;
    std::string name;
    std::string description;
    float price;

    Vector2 position; // Coordinates on map
    bool isActive; // True when not yet picked up by the player
};


