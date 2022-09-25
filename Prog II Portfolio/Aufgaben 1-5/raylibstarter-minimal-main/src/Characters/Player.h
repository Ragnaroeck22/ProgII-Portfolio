//
// Created by 22max on 10.08.2022.
//

#pragma once
#include "Char.h"
#include "../Systems/Inventory.h"
#include "../Items/ItemBase.h"
#include <memory>

const int test = 10;

class Player : public Char {

    // Attributes
public:
    // This sucks
    bool shouldDropItem = false;
    ItemBase itemToDrop;

    // Doesn't work without a pointer for some reason
    std::shared_ptr<Inventory<ItemBase>> myInventory;

private:
    bool blockMovement;

    int strength;

    int maxCarryWeight;
    int currentWeight;


    // Methods
public:
    Player();

private:
    void CustomUpdate() override;
    void CustomDraw(Vector2 position) override;

    void handleInput();

};

