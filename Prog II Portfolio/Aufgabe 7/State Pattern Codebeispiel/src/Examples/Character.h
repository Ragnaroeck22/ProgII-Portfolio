#pragma once
#include <string>
#include <memory>
#include <string>

/* For demonstrating static states
#include "StateStanding.h"
#include "StateJumping.h"
#include "StateDucking.h"
*/

// This has to be awful practice but I've found no other way to make it work
// The problem: I can't include "State.h" inside of "Character.h" AND "Character.h" inside of "State.h"
// Nystrom doesn't mention this in any way when he presents his code... I feel cheated

class Character; // I hate this

class State
{
public:
    /* For demonstrating static states
    static StateStanding standing;
    static StateDucking ducking;
    static StateJumping jumping;
    // ...
*/

    virtual void Update(Character& character);

    virtual State* handleInput(Character& character);

    virtual void enter(Character& character);
};

// God is dead
class Character
{
    // Attributes
public:
    std::string debugMessage; // For visualisation

    State* currentState;

    // Methods
public:
    Character();

    void Update();

    void Draw();

    void handleInput();
};
