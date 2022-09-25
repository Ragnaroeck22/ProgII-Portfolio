//
// Created by 22max on 10.08.2022.
//

#pragma once
#include <vector>
#include "../Items/Weapon.h"
#include "../Items/Ring.h"
#include "../Items/Necklace.h"
#include "raylib.h"

template<class TempItem>
class Inventory {

    // Attributes
public:
    // This sucks
    bool shouldDropItem = false;
    TempItem itemToDrop;

private:

    std::vector<TempItem> inventory;
    std::vector<TempItem> slotWeapon;
    std::vector<TempItem> slotNecklace;
    std::vector<TempItem> slotRing;

    int slotCount;

    bool isOpen;


    // ======== GUI ========
    Rectangle panel;

    // exit-button
    Rectangle exitButtonRec;
    bool exitButtonHovered;

    // item buttons
    Rectangle buttonEquipRec;
    Rectangle buttonDropRec;
    Rectangle buttonMemoryRec;
    bool showItemButtons;
    bool buttonModeEquip;
    int slotToEdit;
    ItemType bigSlotToEdit;
    bool editingBigSlot;

    // item slots
    std::vector<Rectangle> itemSlotRecs;
    int hoveredSlot;
    bool slotIsHovered;

    // Big slots
    Rectangle slotWeaponRec;
    Rectangle slotNecklaceRec;
    Rectangle slotRingRec;
    ItemType hoveredBigSlot;
    bool bigSlotHovered;

    // Images & Textures
    Texture2D weaponSheetSmall;
    Texture2D weaponSheetLarge;
    Texture2D jewelrySheetSmall;
    Texture2D jewelrySheetLarge;


    // Methods
public:
    Inventory<TempItem>(int slots)
    {
        this->slotCount = slots;
        isOpen = false;

        panel.x = GetScreenWidth() * 0.1;
        panel.y = GetScreenHeight() * 0.1;
        panel.width = GetScreenWidth() * 0.8;
        panel.height = GetScreenHeight() * 0.8;

        exitButtonRec.width = panel.width / 15;
        exitButtonRec.height = panel.height / 15;
        exitButtonRec.x = panel.x + panel.width - exitButtonRec.width;
        exitButtonRec.y = panel.y;

        exitButtonHovered = false;
        bigSlotHovered = false;

        buttonEquipRec. width = panel.width / 7;
        buttonEquipRec.height = panel.height / 10;
        buttonDropRec. width = panel.width / 7;
        buttonDropRec.height = panel.height / 10;
        showItemButtons = false;

        // Item slots
        slotWeaponRec = {panel.x + panel.width / 25, panel.y + panel.height / 10,
                         panel.width / 5, panel.height / 5};
        slotNecklaceRec = {slotWeaponRec.x, slotWeaponRec.y + slotWeaponRec.height + panel.height / 10,
                        slotWeaponRec.width, slotWeaponRec.height};
        slotRingRec = {slotWeaponRec.x, slotNecklaceRec.y + slotWeaponRec.height + panel.height / 10,
                        slotWeaponRec.width, slotWeaponRec.height};

        hoveredSlot = 0;
        slotIsHovered = false;
        float slotY = panel.y + panel.height / 10;
        for (int i = 0; i < slotCount; i++)
        {
            itemSlotRecs.push_back({panel.x + panel.width / 2, slotY,
                                 panel.width / 14, panel.height / 14});
            slotY = slotY + panel.height / 12;
        }

        // Manage images
        Image workingImage = LoadImage("assets/graphics/weapons.png");
        ImageResize(&workingImage, itemSlotRecs[0].width * 24, itemSlotRecs[0].height * 20);
        weaponSheetSmall = LoadTextureFromImage(workingImage);
        ImageResize(&workingImage, slotWeaponRec.width * 24, slotWeaponRec.height * 20);
        weaponSheetLarge = LoadTextureFromImage(workingImage);
        workingImage = LoadImage("assets/graphics/jewelry.png");
        ImageResize(&workingImage, itemSlotRecs[0].width * 4, itemSlotRecs[0].height);
        jewelrySheetSmall = LoadTextureFromImage(workingImage);
        ImageResize(&workingImage, slotWeaponRec.width * 4, slotWeaponRec.height);
        jewelrySheetLarge = LoadTextureFromImage(workingImage);

    };

    // ======== UPDATE-FUNCTIONS ========

    void Update()
    {
        if (isOpen)
        {
            // Detect exit button
            if (CheckCollisionPointRec(GetMousePosition(), exitButtonRec))
            {
                exitButtonHovered = true;
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    isOpen = false;
                }
            }
            else
            {
                exitButtonHovered = false;
            }

            getSlotHover();

            if (showItemButtons)
            {
                if (CheckCollisionPointRec(GetMousePosition(), buttonEquipRec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    if (buttonModeEquip)
                    {
                        equipItem(slotToEdit);
                    }
                    else
                    {
                        unequipItem(bigSlotToEdit);
                    }
                    showItemButtons = false;
                }
                if (!editingBigSlot)
                {
                    if (CheckCollisionPointRec(GetMousePosition(), buttonDropRec) &&
                        IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        if (slotToEdit < inventory.size() && slotToEdit >= 0)
                        {
                            itemToDrop = dropItem(slotToEdit);
                            shouldDropItem = true;
                        }
                        showItemButtons = false;
                    }
                }

            }

            // Open button menu
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && (slotIsHovered || bigSlotHovered))
            {
                Rectangle recToMatch;
                if (slotIsHovered)
                {
                    recToMatch = itemSlotRecs[hoveredSlot];
                    slotToEdit = hoveredSlot;
                    buttonModeEquip = true;
                    editingBigSlot = false;
                }
                else if (bigSlotHovered)
                {
                    switch (hoveredBigSlot)
                    {
                        case TypeWeapon:
                            recToMatch = slotWeaponRec;
                            bigSlotToEdit = TypeWeapon;
                            break;
                        case TypeNecklace:
                            recToMatch = slotNecklaceRec;
                            bigSlotToEdit = TypeNecklace;
                            break;
                        case TypeRing:
                            recToMatch = slotRingRec;
                            bigSlotToEdit = TypeRing;
                            break;
                    }
                    editingBigSlot = true;
                    buttonModeEquip = false;
                }
                // Check if recToMatch == buttonMemoryRec
                if (recToMatch.x == buttonMemoryRec.x && recToMatch.y == buttonMemoryRec.y &&
                    recToMatch.width == buttonMemoryRec.width && recToMatch.height == buttonMemoryRec.height &&
                    showItemButtons)
                {
                    showItemButtons = false;
                }
                else
                {
                    showItemButtons = true;
                }
                buttonMemoryRec = recToMatch;

                buttonEquipRec.y = recToMatch.y + recToMatch.height / 2 - buttonEquipRec.height / 2;
                buttonDropRec.y = buttonEquipRec.y;
                buttonEquipRec.x = recToMatch.x + recToMatch.width + panel.width / 20;
                buttonDropRec.x = buttonEquipRec.x + buttonEquipRec.width + panel.width / 20;
            }

        }
    };

    void getSlotHover()
    {
        // Detect slot hover
        bool hoverSlotFound = false;
        for (int i = 0; i < itemSlotRecs.size(); i++)
        {
            if (CheckCollisionPointRec(GetMousePosition(), itemSlotRecs[i]))
            {
                hoverSlotFound = true;
                slotIsHovered = true;
                hoveredSlot = i;
            }
        }
        if (CheckCollisionPointRec(GetMousePosition(), slotWeaponRec))
        {
            hoverSlotFound = true;
            bigSlotHovered = true;
            hoveredBigSlot = TypeWeapon;
        }
        else if (CheckCollisionPointRec(GetMousePosition(), slotNecklaceRec))
        {
            hoverSlotFound = true;
            bigSlotHovered = true;
            hoveredBigSlot = TypeNecklace;
        }
        else if (CheckCollisionPointRec(GetMousePosition(), slotRingRec))
        {
            hoverSlotFound = true;
            bigSlotHovered = true;
            hoveredBigSlot = TypeRing;
        }
        if (!hoverSlotFound)
        {
            slotIsHovered = false;
            bigSlotHovered = false;
        }
    }

    void Draw()
    {
        if (isOpen)
        {
            DrawRectangle(panel.x, panel.y, panel.width, panel.height, LIGHTGRAY);

            //DrawRectangle(panel.x, panel.y, panel.width, exitButtonRec.height, GRAY);

            // Draw exit button
            if (!exitButtonHovered)
                DrawRectangle(exitButtonRec.x, exitButtonRec.y, exitButtonRec.width, exitButtonRec.height, RED);
            else
                DrawRectangle(exitButtonRec.x, exitButtonRec.y, exitButtonRec.width, exitButtonRec.height, ORANGE);
            int exitFontSize = 30;
            DrawText("x", exitButtonRec.x + exitButtonRec.width / 2 - MeasureText("x", exitFontSize) / 2,
                     exitButtonRec.y + 0.175 * exitButtonRec.height, exitFontSize, WHITE);


            // Draw item slots
            for (int i = 0; i < slotCount; i++)
            {
                DrawRectangle(itemSlotRecs[i].x, itemSlotRecs[i].y,
                              itemSlotRecs[i].width, itemSlotRecs[i].height, BLACK);
            }
            // Draw big slots
            DrawText("Equipment", slotWeaponRec.x + slotWeaponRec.width / 2 - MeasureText("Equipment", 20) / 2,
                     slotWeaponRec.y - panel.height / 20, 20, BLACK);
            DrawRectangle(slotWeaponRec.x, slotWeaponRec.y, slotWeaponRec.width, slotWeaponRec.height, BLACK);
            DrawRectangle(slotNecklaceRec.x, slotNecklaceRec.y, slotNecklaceRec.width, slotNecklaceRec.height, BLACK);
            DrawRectangle(slotRingRec.x, slotRingRec.y, slotRingRec.width, slotRingRec.height, BLACK);

            // Draw hover
            if (slotIsHovered)
            {
                DrawRectangleLines(itemSlotRecs[hoveredSlot].x, itemSlotRecs[hoveredSlot].y,
                                   itemSlotRecs[hoveredSlot].width, itemSlotRecs[hoveredSlot].height, YELLOW);
            }
            else if (bigSlotHovered)
            {
                float posY;
                switch (hoveredBigSlot)
                {
                    case TypeWeapon:
                        posY = slotWeaponRec.y;
                        break;
                    case TypeNecklace:
                        posY = slotNecklaceRec.y;
                        break;
                    case TypeRing:
                        posY = slotRingRec.y;
                        break;
                }
                DrawRectangleLines(slotWeaponRec.x, posY,
                                   slotWeaponRec.width, slotWeaponRec.height, YELLOW);
            }

            // -----Draw items-----

            // Draw inv items
            for (int i = 0; i < inventory.size(); i++)
            {
                Rectangle texRecAdjusted;
                Texture2D texToUse;
                Vector2 factor;
                if (inventory[i].type == TypeWeapon)
                {
                    texToUse = weaponSheetSmall;
                    factor.x = (itemSlotRecs[0].width * 24) / inventory[i].texSheet.width;
                    factor.y = (itemSlotRecs[0].height * 20) / inventory[i].texSheet.height;
                }
                else
                {
                    factor.x = (itemSlotRecs[0].width * 4) / inventory[i].texSheet.width;
                    factor.y = (itemSlotRecs[0].height) / inventory[i].texSheet.height;
                    texToUse = jewelrySheetSmall;
                }
                texRecAdjusted = {inventory[i].texRec.x * factor.x, inventory[i].texRec.y * factor.y,
                                  inventory[i].texRec.width * factor.x, inventory[i].texRec.height * factor.y};
                DrawTextureRec(texToUse, texRecAdjusted,
                               {itemSlotRecs[i].x, itemSlotRecs[i].y}, WHITE);

            }
            // Draw big slot items
            Vector2 factor;
            Rectangle texRecAdjusted;
            if (!slotWeapon.empty())
            {
                factor.x = (slotWeaponRec.width * 24) / slotWeapon[0].texSheet.width;
                factor.y = (slotWeaponRec.height * 20) / slotWeapon[0].texSheet.height;
                texRecAdjusted = {slotWeapon[0].texRec.x * factor.x, slotWeapon[0].texRec.y * factor.y,
                                  slotWeapon[0].texRec.width * factor.x, slotWeapon[0].texRec.height * factor.y};
                DrawTextureRec(weaponSheetLarge, texRecAdjusted, {slotWeaponRec.x, slotWeaponRec.y}, WHITE);
            }
            if (!slotNecklace.empty())
            {
                factor.x = (slotNecklaceRec.width * 4) / slotNecklace[0].texSheet.width;
                factor.y = (slotNecklaceRec.height) / slotNecklace[0].texSheet.height;
                texRecAdjusted = {slotNecklace[0].texRec.x * factor.x, slotNecklace[0].texRec.y * factor.y,
                                  slotNecklace[0].texRec.width * factor.x, slotNecklace[0].texRec.height * factor.y};
                DrawTextureRec(jewelrySheetLarge, texRecAdjusted, {slotNecklaceRec.x, slotNecklaceRec.y}, WHITE);
            }
            if (!slotRing.empty())
            {
                factor.x = (slotRingRec.width * 4) / slotRing[0].texSheet.width;
                factor.y = (slotRingRec.height) / slotRing[0].texSheet.height;
                texRecAdjusted = {slotRing[0].texRec.x * factor.x, slotRing[0].texRec.y * factor.y,
                                  slotRing[0].texRec.width * factor.x, slotRing[0].texRec.height * factor.y};
                DrawTextureRec(jewelrySheetLarge, texRecAdjusted, {slotRingRec.x, slotRingRec.y}, WHITE);
            }

            // Draw item buttons
            if (showItemButtons)
            {
                DrawRectangle(buttonEquipRec.x, buttonEquipRec.y, buttonEquipRec.width, buttonEquipRec.height, GRAY);

                if (!editingBigSlot)
                    DrawRectangle(buttonDropRec.x, buttonDropRec.y, buttonDropRec.width, buttonDropRec.height, GRAY);

                std::string buttonText;
                float itemFontSize = 22;
                float itemFontSpacing = 5;

                if (buttonModeEquip)
                {
                    buttonText = "Equip";
                }
                else
                {
                    buttonText = "Unequip";
                }

                // Equip button
                DrawTextEx(GetFontDefault(), buttonText.c_str(),
                           {buttonEquipRec.x + buttonEquipRec.width / 2 - MeasureTextEx(GetFontDefault(), buttonText.c_str(), itemFontSize, itemFontSpacing).x / 2,
                            buttonEquipRec.y + buttonEquipRec.width / 2 - MeasureTextEx(GetFontDefault(), buttonText.c_str(), itemFontSize, itemFontSpacing).x / 2},
                            itemFontSize, itemFontSpacing, WHITE);
                buttonText = "Drop";

                // Drop button
                if (!editingBigSlot)
                {
                    DrawTextEx(GetFontDefault(), buttonText.c_str(),
                               {buttonDropRec.x + buttonDropRec.width / 2 -
                                MeasureTextEx(GetFontDefault(), buttonText.c_str(), itemFontSize, itemFontSpacing).x /
                                2,
                                buttonDropRec.y + buttonDropRec.width / 2 -
                                MeasureTextEx(GetFontDefault(), buttonText.c_str(), itemFontSize, itemFontSpacing).x /
                                2},
                               itemFontSize, itemFontSpacing, WHITE);
                }
            }

        }
    };

    TempItem getSlot(int slot)
    {
        if (slot < this->inventory.size() && slot >= 0)
        {
            return inventory[slot];
        }
        else
        {
            TraceLog(LOG_INFO, "Error: Requested slot out of bounds (Inventory::getSlot() )");
            return NULL;
        }
    };

    bool setSlot(TempItem item, int slot)
    {
        if (slot < this->inventory.size() && slot >= 0)
        {
            this->inventory[slot] = item;
            return true;
        }
        else
        {
            TraceLog(LOG_INFO, "Error: Requested slot out of bounds (Inventory::setSlot() )");
            return false;
        }
    };

    bool addItem(TempItem item)
    {
        if (this->inventory.size() < this->slotCount)
        {
            this->inventory.push_back(item);
            return true;
        }
        else
        {
            TraceLog(LOG_INFO, "Item couldn't be added; inventory is full");
            return false;
        }
    };

    TempItem dropItem(int slot)
    {
            TempItem item = this->inventory[slot];
            this->inventory.erase(inventory.cbegin() + slot);
            return item;
    };

    void equipItem(int itemSlot)
    {
        TraceLog(LOG_INFO, "Equipping item");
        if (itemSlot < inventory.size())
        {
            switch (this->inventory[itemSlot].type)
            {
                case TypeWeapon:
                    // Unequip item if item is equipped
                    if (!slotWeapon.empty())
                    {
                        this->inventory.push_back(slotWeapon[0]);
                        slotWeapon.clear();
                    }
                    // Equip and remove from inventory container
                    slotWeapon.push_back(this->inventory[itemSlot]);
                    this->inventory.erase(inventory.cbegin() + itemSlot);
                    break;
                case TypeNecklace:
                    // Unequip item if item is equipped
                    if (!slotNecklace.empty())
                    {
                        this->inventory.push_back(slotNecklace[0]);
                        slotNecklace.clear();
                    }
                    // Equip and remove from inventory container
                    slotNecklace.push_back(this->inventory[itemSlot]);
                    this->inventory.erase(inventory.cbegin() + itemSlot);
                    break;
                case TypeRing:
                    // Unequip item if item is equipped
                    if (!slotRing.empty())
                    {
                        this->inventory.push_back(slotRing[0]);
                        slotRing.clear();
                    }
                    // Equip and remove from inventory container
                    slotRing.push_back(this->inventory[itemSlot]);
                    this->inventory.erase(inventory.cbegin() + itemSlot);
                    break;
            }
        }


        /*  Didn't work for some sad reason

        std::vector<TempItem>* slotToEdit;
        // determine which slot should be filled
        switch (this->inventory[itemSlot].type)
        {
            case TypeWeapon:
                slotToEdit = &slotWeapon;
                break;
            case TypeNecklace:
                slotToEdit = &slotNecklace;
                break;
            case TypeRing:
                slotToEdit = &slotRing;
                break;
        }
        // Unequip item if item is equipped
        if (!slotToEdit->empty())
        {

            this->inventory.push_back(slotToEdit[0]);
            slotToEdit->clear();
        }
        // Equip and remove from inventory container
        slotToEdit->push_back(this->inventory[itemSlot]);
        this->inventory.erase(this->inventory.cbegin() + itemSlot);


         */
    }

    bool unequipItem(ItemType slot)
    {
        switch (slot)
        {
            case TypeWeapon:
                if (!slotWeapon.empty())
                {
                    if (addItem(slotWeapon[0])) // Returns true if adding the item was successful
                    {
                        slotWeapon.erase(slotWeapon.cbegin());
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                break;
            case TypeNecklace:
                if (!slotNecklace.empty())
                {
                    if (addItem(slotNecklace[0])) // Returns true if adding the item was successful
                    {
                        slotNecklace.erase(slotNecklace.cbegin());
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                break;
            case TypeRing:
                if (!slotRing.empty())
                {
                    if (addItem(slotRing[0])) // Returns true if adding the item was successful
                    {
                        slotRing.erase(slotRing.cbegin());
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
        }
    }



    // GUI
    void open()
    {
        this->isOpen = true;
    }

};


