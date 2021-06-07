#pragma once

#include <meteor.h>
#include "itemslot.h"

using namespace meteor;

class Inventory
{
public:
    Inventory(Ref<Texture2D> blockIconSpriteSheet);

    void                     OnImGuiRender();
                             
    void                     Toggle();
    inline mtrBool           IsOpen() { return m_Open; }
private:
    static constexpr mtrInt  C_INVENTORY_SIZE = 16;

    Ref<Texture2D>           m_BlockIconsSpritesheet;
    ItemSlot                 m_Slots[C_INVENTORY_SIZE][C_INVENTORY_SIZE];
    mtrBool                  m_Open = false;
};