//
// Created by 22max on 10.08.2022.
//

#include "Inventory.h"
#include <raylib.h>

template<class TempItem>
Inventory<TempItem>::Inventory(int slotCount)
{
    this->slotCount = slotCount;
}

template<class TempItem>
TempItem Inventory<TempItem>::getSlot(int slot)
{
    if (slot < this->inventory.size() && slot >= 0)
    {
        return inventory[slot];
    }
    else
    {
        TraceLog(LOG_INFO, "Error: Requested slot out of bounds (Inventory::getSlot() )");
        return nullptr;
    }
}

template<class TempItem>
void Inventory<TempItem>::setSlot(TempItem item, int slot)
{
    if (slot < this->inventory.size() && slot >= 0)
    {
        this->inventory[slot] = item;
    }
    else
    {
        TraceLog(LOG_INFO, "Error: Requested slot out of bounds (Inventory::setSlot() )");
    }
}

