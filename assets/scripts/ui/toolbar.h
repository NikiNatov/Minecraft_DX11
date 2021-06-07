#pragma once

#include <meteor.h>
#include <imgui\imgui.h>

#include "itemslot.h"

using namespace meteor;

class ToolBar
{
public:
    ToolBar(Ref<Texture2D> blockIconsSpriteSheet);

    void                    SetActiveItemSlot(mtrU32 index);
    void                    OnImGuiRender();

    inline const ItemSlot&  GetActiveItemSlot() const { return m_Slots[m_ActiveSlotIndex]; }
    inline Ref<Texture2D>   GetIconsSpriteSheet() const { return m_BlockIconsSpritesheet; }
private:
    Ref<Texture2D>          m_BlockIconsSpritesheet;
    std::vector<ItemSlot>   m_Slots;
    mtrU32                  m_ActiveSlotIndex = 0;
};