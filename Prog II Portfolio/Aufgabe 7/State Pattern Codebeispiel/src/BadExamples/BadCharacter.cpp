#include "BadCharacter.h"
#include "raylib.h"

BadCharacter::BadCharacter()
{
    this->state = STATE_STANDING;
    this->debugMessage = "This character is standing!";
}

void BadCharacter::Update()
{
    switch (this->state)
    {
        case STATE_DUCKING:
            // Counts how many frames the player has been ducking for
            // (stupid but it's for demonstration purposes)
            this->duckTimer++;

            TraceLog(LOG_INFO, std::to_string(duckTimer).c_str());

            break;

            // Here can go more cases that define state behaviour
    }
}

void BadCharacter::Draw()
{
}

void BadCharacter::handleInput()
{
    switch (this->state)
    {
        case STATE_STANDING:
            if (IsKeyPressed(KEY_SPACE))
            {
                this->state = STATE_JUMPING;
                this->debugMessage = "This character is jumping!";
                this->duckTimer = 0;

                // Adjust player velocity
                // Change texture
                // etc.
            }
            else if (IsKeyPressed(KEY_DOWN))
            {
                this->state = STATE_DUCKING;
                this->debugMessage = "This character is ducking!";

                // Change texture
                // etc.
            }
            break;
        case STATE_DUCKING:
            if (IsKeyReleased(KEY_DOWN))
            {
                state = STATE_STANDING;
                this->debugMessage = "This character is standing!";

                // Change texture
                // etc.
            }
            break;
        case STATE_JUMPING:
            if (IsKeyPressed(KEY_DOWN)) // Doesn't make any sense but it's for demonstration purposes
            {
                state = STATE_STANDING;
                this->debugMessage = "This character is standing!";

                // Change texture
                // etc.
            }
    }
}
