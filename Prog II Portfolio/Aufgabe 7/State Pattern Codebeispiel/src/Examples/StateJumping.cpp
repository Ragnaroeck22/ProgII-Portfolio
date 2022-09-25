#include "StateJumping.h"
#include "raylib.h"
#include "StateStanding.h"

StateJumping::StateJumping()
{

}

void StateJumping::enter(Character &character)
{
    character.debugMessage = "This character is jumping!";

    // Change texture
    // etc.
}

State *StateJumping::handleInput(Character &character)
{
    if (IsKeyPressed(KEY_DOWN)) // Doesn't make any sense but it's for demonstration purposes
    {
        character.debugMessage = "This character is standing!";

        // Change texture
        // etc.

        return new StateStanding;
    }
    else
    {
        return NULL;
    }
}
