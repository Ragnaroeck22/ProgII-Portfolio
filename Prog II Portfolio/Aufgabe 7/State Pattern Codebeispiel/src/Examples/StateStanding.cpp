#include "StateStanding.h"
#include "raylib.h"
#include "StateDucking.h"
#include "StateJumping.h"

State* StateStanding::handleInput(Character& character)
{
    if (IsKeyPressed(KEY_SPACE))
    {
        character.debugMessage = "This character is jumping!";

        // Adjust player velocity
        // Change texture (jumping)
        // etc.
        return new StateJumping;
    }
    else if (IsKeyPressed(KEY_DOWN))
    {
        character.debugMessage = "This character is ducking!";

        // Adjust player velocity
        // Change texture (ducking)
        // etc.
        return new StateDucking;
    }
    else
    {
        return NULL;
    }

}

StateStanding::StateStanding()
{

}

void StateStanding::enter(Character &character)
{
    // Change texture
    // etc.

    character.debugMessage = "This character is standing!";
}
