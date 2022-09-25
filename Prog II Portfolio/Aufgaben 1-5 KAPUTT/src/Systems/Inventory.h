//
// Created by 22max on 10.08.2022.
//

#pragma once
#include <vector>

template<class TempItem>
class Inventory {

    // Attributes
private:
    std::vector<TempItem> inventory;

    int slotCount;

    // Methods
public:
    Inventory(int slotCount);

    TempItem getSlot(int slot);
    void setSlot(TempItem item, int slot);
};

