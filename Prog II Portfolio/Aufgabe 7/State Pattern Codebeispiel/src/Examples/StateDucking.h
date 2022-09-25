#pragma once

#include "Character.h"

class StateDucking : public State
{
    // Attributes
public:
    int duckTimer;

    // Methods
public:
    State* handleInput(Character& character) override;

    virtual void Update(Character& character) override;

    void enter(Character &character) override;
};
