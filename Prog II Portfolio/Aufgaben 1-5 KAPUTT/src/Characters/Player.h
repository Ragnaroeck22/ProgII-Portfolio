//
// Created by 22max on 10.08.2022.
//

#pragma once
#include "Char.h"

class Player : public Char {

    // Methods
public:
    Player();

private:
    void CustomUpdate() override;
    void CustomDraw() override;


};

