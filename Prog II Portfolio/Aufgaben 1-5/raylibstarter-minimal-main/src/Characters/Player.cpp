//
// Created by 22max on 10.08.2022.
//

#include "Player.h"

// TEST
#include "../Items/Sword.h"
#include "../Items/PlainNecklace.h"
#include "../Items/RingMinor.h"
#include "../Items/Axe.h"
#include "../Items/Spear.h"
#include "../Items/BoneNecklace.h"

Player::Player()
{
    this->myInventory = std::make_shared<Inventory<ItemBase>>(10);

    // TEST
    PlainNecklace testN(0,0);
    RingMinor testR(0,0);
    Sword testS(0, 0);
    BoneNecklace testB(0, 0);
    Spear testSp(0, 0);
    Axe testA(0, 0);
    this->myInventory->addItem(testN);
    this->myInventory->addItem(testR);
    this->myInventory->addItem(testS);
    this->myInventory->addItem(testSp);
    this->myInventory->addItem(testA);
    this->myInventory->addItem(testB);

    this->myInventory->equipItem(2);

    this->texture = LoadTexture("assets/graphics/obstacles.png");
    this->textureRec.width = this->texture.width / 16;
    this->textureRec.height = this->texture.height / 16;
    this->textureRec.x = this->textureRec.width;
    this->textureRec.y = this->textureRec.height * 2;

    this->updateCollision = true;
    this->blockUp = false;
    this->blockDown = false;
    this->blockLeft = false;
    this->blockRight = false;
    this->blockMovement = false;

    this->strength = 2;
    this->maxCarryWeight = this->strength * 2;
    this->currentWeight = 0;

}

void Player::CustomUpdate()
{
    this->handleInput();

    this->myInventory->Update();

    // For dropping items (god is dead and I have killed him)
    if (myInventory->shouldDropItem)
    {
        TraceLog(LOG_INFO, "Dropping item (player)");
        myInventory->shouldDropItem = false;
        this->itemToDrop = myInventory->itemToDrop;
        this->itemToDrop.position = this->position;
        this->shouldDropItem = true;
    }
}

void Player::CustomDraw(Vector2 position)
{
    DrawTextureRec(this->texture, this->textureRec, position, WHITE);

    myInventory->Draw();
}

void Player::handleInput()
{
    if ((this->currentWeight <= this->maxCarryWeight))
    {
        if (IsKeyPressed(KEY_UP) && this->blockUp == false)
        {
            this->move(Up);
        }
        else if (IsKeyPressed(KEY_DOWN) && this->blockDown == false)
        {
            this->move(Down);
        }
        else if (IsKeyPressed(KEY_LEFT) && this->blockLeft == false)
        {
            this->move(Left);
        }
        else if (IsKeyPressed(KEY_RIGHT) && this->blockRight == false)
        {
            this->move(Right);
        }
    }

    if (IsKeyPressed(KEY_I))
        this->myInventory->open();
}


