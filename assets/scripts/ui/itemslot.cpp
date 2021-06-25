#include "itemslot.h"

#include "toolbar.h"
#include "../world.h"

// --------------------------------------------------------------------------------------------------------------------------------------
ItemSlot::ItemSlot()
{
}

// --------------------------------------------------------------------------------------------------------------------------------------
ItemSlot::ItemSlot(BlockType blockID, Ref<Texture2D> blockIconsSpritesheet, Ref<Texture2D> backgroundTexture)
    : m_ItemData(Item(blockID)), m_BlockIconsSpritesheet(blockIconsSpritesheet), m_BackgroundTexture(backgroundTexture)
{
}

// --------------------------------------------------------------------------------------------------------------------------------------
void ItemSlot::SetActive(mtrBool active)
{
    m_Active = active;
}

// --------------------------------------------------------------------------------------------------------------------------------------
void ItemSlot::OnImGuiRender()
{
	ImGui::ImageButton(m_BlockIconsSpritesheet->GetTextureHandle(), ImVec2{ 50.0f, 50.0f }, m_ItemData.BlockIconUVs[0], m_ItemData.BlockIconUVs[1], 8, m_Active ? ImVec4{ 1, 1, 1, 1 } : ImVec4{ 0.6, 0.6, 0.6, 1 });

	// Handle drag-and-drop. This element can only recieve data.
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ItemSlot");
		if (payload)
		{
			m_ItemData = *(Item*)(payload->Data);
		}
		ImGui::EndDragDropTarget();
	}
}