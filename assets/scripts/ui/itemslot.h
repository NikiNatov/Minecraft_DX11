#pragma once

#include <meteor.h>
#include <imgui\imgui.h>

#include "../world.h"

using namespace meteor;

struct Item
{
    ImVec2          BlockIconUVs[2];
    mtrU32          BlockIconID;   // The position in the spritesheet grid
    BlockType       BlockID;

    Item()
    {

    }

    Item(BlockType blockID)
    {
        BlockID = blockID;
        BlockIconID = BlockData::BlockTypes[BlockID].BlockIconID;
        CalculateUVs();
    }

    void CalculateUVs()
    {
        mtrInt textureAtlasSizeInBlocks = 16;
        mtrFloat textureAtlasBlockSize = 1.0f / (mtrFloat)textureAtlasSizeInBlocks;

        mtrFloat y = BlockIconID / textureAtlasSizeInBlocks;
        mtrFloat x = BlockIconID - (y * textureAtlasSizeInBlocks);

        x *= textureAtlasBlockSize;
        y *= textureAtlasBlockSize;

        y = y + textureAtlasBlockSize;

        BlockIconUVs[0] = ImVec2{ x + textureAtlasBlockSize, y - textureAtlasBlockSize };
        BlockIconUVs[1] = ImVec2{ x, y };
    }
};

class ToolBar;

class ItemSlot
{
public:
    ItemSlot();
    ItemSlot(BlockType blockID, Ref<Texture2D> blockIconsSpritesheet);

    void                    SetActive(mtrBool active);
    void                    OnImGuiRender();

    inline const Item*      GetItem() const { return &m_ItemData; }
    inline mtrBool          IsActive() { return m_Active; }
private:
    Item                    m_ItemData;
    Ref<Texture2D>          m_BorderImage;
    mtrBool                 m_Active;
    Ref<Texture2D>          m_BlockIconsSpritesheet;
};