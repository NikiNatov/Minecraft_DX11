#pragma once

#include <meteor.h>
#include <random>

#include "block.h"
#include "world.h"
#include "helpers.h"

using namespace meteor;

class Chunk
{
public:
	Chunk(const glm::ivec2& positionInGrid, World* world);

	void					ReleaseEntity();
	void					Recreate();
	void					CreateMesh();
	void					CreateBlockMap();
	void					UpdateMeshData();
	void					ClearMeshData();
	void					EditBlock(mtrInt x, mtrInt y, mtrInt z, BlockType block);
	BlockType				GetBlock(mtrInt x, mtrInt y, mtrInt z);
	mtrBool					IsRenderable();
private:
	void					AddFaces(mtrInt x, mtrInt y, mtrInt z);
	void					AddTexture(mtrInt textureID);
	mtrBool					IsBlockInChunk(mtrInt x, mtrInt y, mtrInt z);
public:
	static const mtrInt		C_WIDTH = 16;
	static const mtrInt		C_HEIGHT = 256;

	static const mtrInt		VOID_LEVEL = 0;
	static const mtrInt		WATER_LEVEL = 40;
	static const mtrInt		CLOUD_LEVEL = 120;

	glm::ivec2				GridPosition;
private:
	Entity					m_ChunkEntity;

	BlockType				m_BlockMap[C_WIDTH][C_HEIGHT][C_WIDTH]{ Air };
	World* m_World;

	// Mesh data
	std::vector<glm::vec3>	m_Positions;
	std::vector<glm::vec2>	m_UVs;
	std::vector<glm::vec3>	m_Normals;
	std::vector<glm::vec4>	m_Colors;

	std::vector<mtrU32>		m_SolidIndices;
	std::vector<mtrU32>		m_TransparentIndices;
	std::vector<mtrU32>		m_WaterIndices;
	mtrU32					m_IndicesCount = 0;
};