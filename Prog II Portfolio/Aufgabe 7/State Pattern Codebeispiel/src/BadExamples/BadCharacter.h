#pragma once

#include "Enums.h"
#include <string>

class BadCharacter
{
    // Attributes
public:
    BadState state; // Enum

    int duckTimer;
    std::string debugMessage; // For visualisation

    // Methods
public:
    BadCharacter();

    void Update();

    void Draw();

    void handleInput();
};
