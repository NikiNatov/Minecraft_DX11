#include "toolbar.h"

// --------------------------------------------------------------------------------------------------------------------------------------
ToolBar::ToolBar(Ref<Texture2D> blockIconsSpriteSheet, Ref<Texture2D> slotBackgroundTexture)
    : m_BlockIconsSpritesheet(blockIconsSpriteSheet)
{
    m_Slots.reserve(7);

    for (mtrInt i = 0; i < 7; i++)
        m_Slots.emplace_back((BlockType)i, m_BlockIconsSpritesheet, slotBackgroundTexture);
}

// --------------------------------------------------------------------------------------------------------------------------------------
void ToolBar::SetActiveItemSlot(mtrU32 index)
{
    METEOR_ASSERT(index > m_Slots.size() - 1, "Index out of bounds!");

    m_ActiveSlotIndex = index;
    m_Slots[index].SetActive(true);

    for (mtrInt i = 0; i < m_Slots.size(); i++)
    {
        if (i != index)
            m_Slots[i].SetActive(false);
    }
}

// --------------------------------------------------------------------------------------------------------------------------------------
void ToolBar::OnImGuiRender()
{
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos({ viewport->GetWorkPos().x + viewport->GetWorkSize().x / 2.0f - 219, viewport->GetWorkPos().y + viewport->GetWorkSize().y - 80 });
    ImGui::SetNextWindowSize({ 534, 68 });
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });

    ImGui::Begin("Blocks", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);

    // Draw all slots
    for (ItemSlot& slot : m_Slots)
    {
        slot.OnImGuiRender();
        ImGui::SameLine();
    }

    ImGui::End();
    ImGui::PopStyleVar();
}
