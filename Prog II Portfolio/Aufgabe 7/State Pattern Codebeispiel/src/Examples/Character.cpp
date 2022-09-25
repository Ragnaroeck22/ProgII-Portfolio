#include "Character.h"
#include "StateStanding.h"
#include "raylib.h"

Character::Character()
{
    this->currentState = new StateStanding;
    this->currentState->enter(*this);
}

void Character::Update()
{
    this->currentState->Update(*this);
}

void Character::handleInput()
{
    State* state = this->currentState->handleInput(*this);

    // For instantiated states
    if (state != NULL)
    {
        delete this->currentState;
        this->currentState = state;


        this->currentState->enter(*this);
    }
}

void Character::Draw()
{
    DrawText(this->debugMessage.c_str(), 100, 100, 50, BLACK);
}
