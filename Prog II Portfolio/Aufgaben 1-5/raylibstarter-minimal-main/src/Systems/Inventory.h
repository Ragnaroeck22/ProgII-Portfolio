//
// Created by 22max on 10.08.2022.
//

#pragma once
#include <vector>
#include "../Items/Weapon.h"
#include "../Items/Ring.h"
#include "../Items/Necklace.h"
#include "../Items/Sword.h"
#include "../Items/Spear.h"
#include "../Items/Axe.h"
#include "../Items/Mace.h"
#include "../Items/Flamberge.h"
#include "../Items/Warhammer.h"
#include "../Items/PlainNecklace.h"
#include "../Items/BoneNecklace.h"
#include "../Items/RingMinor.h"
#include "../Items/RingMajor.h"
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

    // Sorting
    std::vector<Rectangle> sortRecs;
    bool lastAscending = false;

    // Carry weight
    float currentWeight;
    float maxWeight;
    bool overencumbered;

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

        calcWeight();


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

        // Sorting
        Rectangle helperRec;
        helperRec.width = panel.width / 15;
        helperRec.height = panel.height / 15;
        helperRec.x = exitButtonRec.x - panel.width / 10;
        helperRec.y = panel.y;

        for (int i = 0; i < 3; i++)
        {
            sortRecs.push_back(helperRec);
            helperRec.x = helperRec.x - panel.width / 10;
        }

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

            for (int i = 0; i < sortRecs.size(); i++)
            {
                if (CheckCollisionPointRec(GetMousePosition(), sortRecs[i]) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    if (lastAscending)
                    {
                        lastAscending = false;
                    }
                    else
                    {
                        lastAscending = true;
                    }
                    switch (i)
                    {
                        case 0:
                            sortInv(Name, lastAscending);
                            break;
                        case 1:
                            sortInv(Price, lastAscending);
                            break;
                        case 2:
                            sortInv(Weight, lastAscending);
                            break;
                        default:
                            sortInv(Weight, lastAscending);
                    }

                }
            }

            if (IsKeyPressed(KEY_R))
                sortInv(Weight, false);
            if (IsKeyPressed(KEY_T))
                sortInv(Price, false);
            if (IsKeyPressed(KEY_Y))
                sortInv(Name, false);

            if (IsKeyPressed(KEY_U))
            {
                createDemoInv();
            }

            getSlotHover();

            // Item buttons
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

    void sortInv(SortType sortType, bool ascending)
    {
        TempItem sortHelper;
        switch (sortType)
        {
            case Weight:
                TraceLog(LOG_INFO, "Sorting for weight");
                if (ascending)
                {
                    TraceLog(LOG_INFO, "Ascending");
                    for (int i = 0; i < inventory.size() - 1; i++)
                    {
                        for (int j = 0; j < (inventory.size() - i - 1); j++)
                        {
                            if (inventory[j].weight > inventory[j + 1].weight)
                            {
                                sortHelper = inventory[j];
                                inventory[j] = inventory[j + 1];
                                inventory[j + 1] = sortHelper;
                            }
                        }
                    }
                }
                else
                {
                    TraceLog(LOG_INFO, "Descending");
                    int flag = 1;
                    for (int i = 1; (i <= inventory.size()) && flag; i++)
                    {
                        flag = 0;
                        for (int j = 0; j < (inventory.size() - 1); j++)
                        {
                            if (inventory[j + 1].weight > inventory[j].weight)
                            {
                                sortHelper = inventory[j];
                                inventory[j] = inventory[j + 1];
                                inventory[j + 1] = sortHelper;
                                flag = 1;
                            }
                        }
                    }
                }
                break;
            case Price:
                TraceLog(LOG_INFO, "Sorting for price");
                if (ascending)
                {
                    TraceLog(LOG_INFO, "Ascending");
                    for (int i = 0; i < inventory.size() - 1; i++)
                    {
                        for (int j = 0; j < (inventory.size() - i - 1); j++)
                        {
                            if (inventory[j].price > inventory[j + 1].price)
                            {
                                sortHelper = inventory[j];
                                inventory[j] = inventory[j + 1];
                                inventory[j + 1] = sortHelper;
                            }
                        }
                    }
                }
                else
                {
                    TraceLog(LOG_INFO, "Descending");
                    int flag = 1;
                    for (int i = 1; (i <= inventory.size()) && flag; i++)
                    {
                        flag = 0;
                        for (int j = 0; j < (inventory.size() - 1); j++)
                        {
                            if (inventory[j + 1].price > inventory[j].price)
                            {
                                sortHelper = inventory[j];
                                inventory[j] = inventory[j + 1];
                                inventory[j + 1] = sortHelper;
                                flag = 1;
                            }
                        }
                    }
                }
                break;
            case Name:
                TraceLog(LOG_INFO, "Sorting for name");
                if (ascending)
                {
                    TraceLog(LOG_INFO, "Ascending");
                    for (int i = 0; i < inventory.size() - 1; i++)
                    {
                        for (int j = 0; j < (inventory.size() - i - 1); j++)
                        {
                            if (inventory[j].name > inventory[j + 1].name)
                            {
                                sortHelper = inventory[j];
                                inventory[j] = inventory[j + 1];
                                inventory[j + 1] = sortHelper;
                            }
                        }
                    }
                }
                else
                {
                    TraceLog(LOG_INFO, "Descending");
                    int flag = 1;
                    for (int i = 1; (i <= inventory.size()) && flag; i++)
                    {
                        flag = 0;
                        for (int j = 0; j < (inventory.size() - 1); j++)
                        {
                            if (inventory[j + 1].name > inventory[j].name)
                            {
                                sortHelper = inventory[j];
                                inventory[j] = inventory[j + 1];
                                inventory[j + 1] = sortHelper;
                                flag = 1;
                            }
                        }
                    }
                }
                break;
        }

    }

    float getCurrentWeight()
    {
        float weight = 0;
        for (int i = 0; i < inventory.size(); i++)
        {
            weight = weight + inventory[i].weight;
        }
        if (!slotWeapon.empty())
            weight = weight + slotWeapon[0].weight;
        if (!slotNecklace.empty())
            weight = weight + slotNecklace[0].weight;
        if (!slotRing.empty())
            weight = weight + slotRing[0].weight;

        return weight;
    }

    float getItemStrength()
    {
        float strength = 0;

        if (!slotWeapon.empty())
            strength = strength + slotWeapon[0].strength;
        if (!slotNecklace.empty())
            strength = strength + slotNecklace[0].strength;
        if (!slotRing.empty())
            strength = strength + slotRing[0].strength;

        return strength;
    }

    void calcWeight()
    {
        overencumbered = false;
        currentWeight = getCurrentWeight();
        maxWeight = (getItemStrength() + 3);
        if (currentWeight > maxWeight)
        {
            overencumbered = true;
        }
    }

    bool getOverencumbered()
    {
        return overencumbered;
    }

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

            // Draw carry weight
            Color outputColor = WHITE;
            std::string outputString = "Carry weight: ";
            outputString.append(std::to_string(getCurrentWeight()));
            outputString.append(" / ");
            outputString.append(std::to_string(maxWeight));
            if (getCurrentWeight() > maxWeight)
            {
                outputColor = RED;
            }
            DrawText(outputString.c_str(), panel.x + panel.width - (MeasureText(outputString.c_str(), 30) * 1.05),
                     panel.y + panel.height - panel.height / 20, 30, outputColor);


            // Draw hover
            if (slotIsHovered)
            {
                DrawRectangleLines(itemSlotRecs[hoveredSlot].x, itemSlotRecs[hoveredSlot].y,
                                   itemSlotRecs[hoveredSlot].width, itemSlotRecs[hoveredSlot].height, YELLOW);

                if (hoveredSlot < inventory.size())
                {
                    // Draw tooltip
                    std::string tooltip = "Name: ";
                    tooltip.append(getSlot(hoveredSlot).name);
                    tooltip.append("\nPrice: ");
                    tooltip.append(std::to_string(getSlot(hoveredSlot).price));
                    tooltip.append("$\nWeight: ");
                    tooltip.append(std::to_string(getSlot(hoveredSlot).weight));
                    tooltip.append("\n\nDesc: ");
                    tooltip.append(getSlot(hoveredSlot).description);

                    DrawText(tooltip.c_str(), GetMousePosition().x + panel.width * 0.05, GetMousePosition().y, 25,
                             WHITE);
                }
            }
            else if (bigSlotHovered)
            {
                float posY;
                switch (hoveredBigSlot)
                {
                    case TypeWeapon:
                        posY = slotWeaponRec.y;
                        if (!slotWeapon.empty())
                        {
                            // Draw tooltip
                            std::string tooltip = "Name: ";
                            tooltip.append(slotWeapon[0].name);
                            tooltip.append("\nPrice: ");
                            tooltip.append(std::to_string(slotWeapon[0].price));
                            tooltip.append("$\nWeight: ");
                            tooltip.append(std::to_string(slotWeapon[0].weight));
                            tooltip.append("\n\nDesc: ");
                            tooltip.append(slotWeapon[0].description);

                            DrawText(tooltip.c_str(), GetMousePosition().x + panel.width * 0.05, GetMousePosition().y, 25,
                                     WHITE);
                        }
                        break;
                    case TypeNecklace:
                        posY = slotNecklaceRec.y;
                        if (!slotNecklace.empty())
                        {
                            // Draw tooltip
                            std::string tooltip = "Name: ";
                            tooltip.append(slotNecklace[0].name);
                            tooltip.append("\nPrice: ");
                            tooltip.append(std::to_string(slotNecklace[0].price));
                            tooltip.append("$\nWeight: ");
                            tooltip.append(std::to_string(slotNecklace[0].weight));
                            tooltip.append("\n\nDesc: ");
                            tooltip.append(slotNecklace[0].description);

                            DrawText(tooltip.c_str(), GetMousePosition().x + panel.width * 0.05, GetMousePosition().y, 25,
                                     WHITE);
                        }
                        break;
                    case TypeRing:
                        posY = slotRingRec.y;
                        if (!slotRing.empty())
                        {
                            // Draw tooltip
                            std::string tooltip = "Name: ";
                            tooltip.append(slotRing[0].name);
                            tooltip.append("\nPrice: ");
                            tooltip.append(std::to_string(slotRing[0].price));
                            tooltip.append("$\nWeight: ");
                            tooltip.append(std::to_string(slotRing[0].weight));
                            tooltip.append("\n\nDesc: ");
                            tooltip.append(slotRing[0].description);

                            DrawText(tooltip.c_str(), GetMousePosition().x + panel.width * 0.05, GetMousePosition().y, 25,
                                     WHITE);
                        }
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

            // Draw sort buttons
            Color sortColor;
            if (lastAscending)
            {
                sortColor = GREEN;
            }
            else
            {
                sortColor = BLUE;
            }
            for (int i = 0; i < sortRecs.size(); i++)
            {
                DrawRectangle(sortRecs[i].x, sortRecs[i].y, sortRecs[i].width, sortRecs[i].height, sortColor);
            }
            DrawTextEx(GetFontDefault(), "N",
                       {sortRecs[0].x + sortRecs[0].width / 2 - MeasureTextEx(GetFontDefault(), "N", 30, 0).x/2,
                        sortRecs[0].y + sortRecs[0].height / 2 - MeasureTextEx(GetFontDefault(), "N", 30, 0).y/2},
                        30, 0, WHITE);
            DrawTextEx(GetFontDefault(), "$",
                       {sortRecs[1].x + sortRecs[1].width / 2 - MeasureTextEx(GetFontDefault(), "$", 30, 0).x/2,
                        sortRecs[1].y + sortRecs[1].height / 2 - MeasureTextEx(GetFontDefault(), "$", 30, 0).y/2},
                       30, 0, WHITE);
            DrawTextEx(GetFontDefault(), "W",
                       {sortRecs[2].x + sortRecs[2].width / 2 - MeasureTextEx(GetFontDefault(), "W", 30, 0).x/2,
                        sortRecs[2].y + sortRecs[2].height / 2 - MeasureTextEx(GetFontDefault(), "W", 30, 0).y/2},
                       30, 0, WHITE);

            DrawTextEx(GetFontDefault(), "Sort:",
                       {sortRecs[2].x - panel.width / 8 - MeasureTextEx(GetFontDefault(), "W", 30, 5).x/2,
                        sortRecs[2].y + sortRecs[2].height / 2 - MeasureTextEx(GetFontDefault(), "W", 30, 5).y/2},
                       30, 5, WHITE);

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
    };

    bool setSlot(TempItem item, int slot)
    {
        if (slot < this->inventory.size() && slot >= 0)
        {
            this->inventory[slot] = item;
            calcWeight();
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
            calcWeight();
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
            calcWeight();
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
            calcWeight();
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
        calcWeight();
    }

    void createDemoInv() // Warning: removes all current items
    {
        inventory.clear();
        Sword sword(0, 0);
        inventory.push_back(sword);
        Spear spear(0,0);
        inventory.push_back(spear);
        Axe axe(0, 0);
        inventory.push_back(axe);
        Mace mace(0, 0);
        inventory.push_back(mace);
        Flamberge flamberge(0, 0);
        inventory.push_back(flamberge);
        Warhammer warhammer(0, 0);
        inventory.push_back(warhammer);

        PlainNecklace plainNecklace (0, 0);
        inventory.push_back(plainNecklace);
        BoneNecklace boneNecklace (0, 0);
        inventory.push_back(boneNecklace);
        RingMinor ringMinor (0, 0);
        inventory.push_back(ringMinor);
        RingMajor ringMajor (0, 0);
        inventory.push_back(ringMajor);
    }

    // GUI
    void open()
    {
        this->isOpen = true;
    }

    bool getOpen()
    {
        return isOpen;
    }

};


