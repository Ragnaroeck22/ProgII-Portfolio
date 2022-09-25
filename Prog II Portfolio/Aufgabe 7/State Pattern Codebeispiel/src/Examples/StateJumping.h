#pragma once

#include "Character.h"

class StateJumping : public State
{
public:
    StateJumping();

    State* handleInput(Character& character) override;

    void enter(Character &character) override;
};
