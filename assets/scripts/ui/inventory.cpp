#include "inventory.h"

// --------------------------------------------------------------------------------------------------------------------------------------
Inventory::Inventory(Ref<Texture2D> blockIconSpriteSheet)
	: m_BlockIconsSpritesheet(blockIconSpriteSheet)
{
	// Initialize slots
	for (mtrInt i = 0 ; i < BlockData::BlockTypes.size() - 1; i++)
	{
		mtrInt x = i % 16;
		mtrInt y = i / 16;

		m_Slots[y][x] = ItemSlot((BlockType)i, m_BlockIconsSpritesheet);
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------
void Inventory::Toggle()
{
	m_Open = !m_Open;
}

// --------------------------------------------------------------------------------------------------------------------------------------
void Inventory::OnImGuiRender()
{
	if (m_Open)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
		ImGui::SetNextWindowPos({ viewport->GetWorkPos().x + viewport->GetWorkSize().x / 2.0f - 260, viewport->GetWorkPos().y + viewport->GetWorkSize().y / 2.0f - 200 });
		ImGui::SetNextWindowSize({ 520, 400 });

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 10, 10 });
		ImGui::Begin("Inventory", &m_Open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

		for (mtrU32 i = 0; i < 16; i++)
		{
			for (mtrU32 j = 0; j < 16; j++)
			{
				const Item* itemData = m_Slots[i][j].GetItem();

				// Render each slot
				ImGui::Image(m_BlockIconsSpritesheet->GetTextureHandle(), ImVec2{ 32.0f, 32.0f }, itemData->BlockIconUVs[0], itemData->BlockIconUVs[1]);

				// Handle drag-and-drop. This element can only send data.
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
				{
					ImGui::SetDragDropPayload("ItemSlot", itemData, sizeof(Item));
					ImGui::Image(m_BlockIconsSpritesheet->GetTextureHandle(), ImVec2{ 50.0f, 50.0f }, itemData->BlockIconUVs[0], itemData->BlockIconUVs[1]);
					ImGui::EndDragDropSource();
				}

				ImGui::SameLine(0, 0);
			}
			ImGui::NewLine();
		}

		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
	}
}
