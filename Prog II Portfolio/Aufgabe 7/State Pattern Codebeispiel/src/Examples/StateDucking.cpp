#include "StateDucking.h"
#include "raylib.h"
#include "StateStanding.h"

State *StateDucking::handleInput(Character &character)
{
    if (IsKeyReleased(KEY_DOWN))
    {
        character.debugMessage = "This character is standing!";

        // Adjust player velocity
        // Change texture (jumping)
        // etc
        return new StateStanding;
    }
    else
    {
        return NULL;
    }
}

void StateDucking::Update(Character &character)
{
    this->duckTimer++;
    TraceLog(LOG_INFO, std::to_string(this->duckTimer).c_str());
}

void StateDucking::enter(Character &character)
{
    character.debugMessage = "This character is ducking!";
    this->duckTimer = 0;

    // Change texture (Ducking)
    // Adjust hitbox
    // etc.
}
