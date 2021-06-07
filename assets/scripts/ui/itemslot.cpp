#include "itemslot.h"

#include "toolbar.h"
#include "../world.h"

// --------------------------------------------------------------------------------------------------------------------------------------
ItemSlot::ItemSlot()
{
}

// --------------------------------------------------------------------------------------------------------------------------------------
ItemSlot::ItemSlot(BlockType blockID, Ref<Texture2D> blockIconsSpritesheet)
    : m_ItemData(Item(blockID)), m_BlockIconsSpritesheet(blockIconsSpritesheet)
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
	ImGui::Image(m_BlockIconsSpritesheet->GetTextureHandle(), ImVec2{ 50.0f, 50.0f }, m_ItemData.BlockIconUVs[0], m_ItemData.BlockIconUVs[1]);

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