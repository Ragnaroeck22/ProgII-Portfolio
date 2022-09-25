#pragma once

#include "Character.h"


class StateStanding : public State
{
    // Methods
public:
    StateStanding();

    State* handleInput(Character& character) override;

    void enter(Character& character) override;
};
