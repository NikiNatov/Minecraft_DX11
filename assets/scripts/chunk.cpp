#include "chunk.h"

// --------------------------------------------------------------------------------------------------------------------------------------
Chunk::Chunk(const glm::ivec2& positionInGrid, World* world)
	: GridPosition(positionInGrid), m_World(world)
{
	m_ChunkEntity = m_World->GetEntity().GetScene()->CreateEntity("Chunk " + std::to_string(GridPosition.x) + ", " + std::to_string(GridPosition.y));
	m_ChunkEntity.GetComponent<TransformComponent>().Position = glm::vec3(GridPosition.x * C_WIDTH, 0.0f, GridPosition.y * C_WIDTH);
	m_ChunkEntity.AddComponent<MeshRendererComponent>();

	CreateBlockMap();
	UpdateMeshData();
}

// --------------------------------------------------------------------------------------------------------------------------------------
void Chunk::ReleaseEntity()
{
	m_ChunkEntity.GetScene()->DeleteEntity(m_ChunkEntity);
}

// --------------------------------------------------------------------------------------------------------------------------------------
void Chunk::Recreate()
{
	UpdateMeshData();
	CreateMesh();
}

// --------------------------------------------------------------------------------------------------------------------------------------
void Chunk::CreateMesh()
{
	Ref<MaterialInstance> blockMat = CreateRef<MaterialInstance>(m_World->BlocksMaterialSolid);
	Ref<MaterialInstance> transparentBlockMat = CreateRef<MaterialInstance>(m_World->BlocksMaterialTransparent);
	Ref<MaterialInstance> waterBlockMat = CreateRef<MaterialInstance>(m_World->BlocksMaterialWater);

	// Create the mesh
	Ref<Mesh> solidMesh = CreateRef<Mesh>(m_Positions, m_UVs, m_Normals, m_Colors, m_SolidIndices);
	solidMesh->SetMaterial(blockMat);
	Ref<Mesh> waterMesh = CreateRef<Mesh>(m_Positions, m_UVs, m_Normals, m_Colors, m_WaterIndices);
	waterMesh->SetMaterial(waterBlockMat);
	Ref<Mesh> transparentMesh = CreateRef<Mesh>(m_Positions, m_UVs, m_Normals, m_Colors, m_TransparentIndices);
	transparentMesh->SetMaterial(transparentBlockMat);

	m_ChunkEntity.GetComponent<MeshRendererComponent>().Meshes.push_back(solidMesh);
	m_ChunkEntity.GetComponent<MeshRendererComponent>().Meshes.push_back(transparentMesh);
	m_ChunkEntity.GetComponent<MeshRendererComponent>().Meshes.push_back(waterMesh);
}

// --------------------------------------------------------------------------------------------------------------------------------------
void Chunk::CreateBlockMap()
{
	mtrInt heights[C_WIDTH][C_WIDTH];

	Noise noise1(m_World->Seed, 6, 110, 205, 0.38, 18);
	Noise noise2(m_World->Seed, 4, 30, 200, 0.15, 0);

	for (mtrInt x = 0; x < C_WIDTH; x++)
	{
		for (mtrInt z = 0; z < C_WIDTH; z++)
		{
			mtrFloat blockGlobalX = x + GridPosition.x * C_WIDTH;
			mtrFloat blockGlobalZ = z + GridPosition.y * C_WIDTH;

			mtrFloat value1 = noise1.GetNoise(blockGlobalX, blockGlobalZ);
			mtrFloat value2 = noise2.GetNoise(blockGlobalX, blockGlobalZ);

			mtrFloat finalResult = value1 * value2;
			heights[x][z] = finalResult * noise1.GetAmplitude() + noise1.GetOffset();
		}
	}

	std::random_device device;
	std::mt19937 rng(device());
	std::uniform_int_distribution<mtrInt> treeDist(1, 95);
	std::uniform_int_distribution<mtrInt> treeHightDist(4, 6);

	for (mtrInt x = 0; x < C_WIDTH; x++)
	{
		for (mtrInt z = 0; z < C_WIDTH; z++)
		{
			for (mtrInt y = 0; y < C_HEIGHT; y++)
			{
				if (y > heights[x][z] && y > WATER_LEVEL && m_BlockMap[x][y][z] != BlockType::Wood && m_BlockMap[x][y][z] != BlockType::Leaf)
					m_BlockMap[x][y][z] = BlockType::Air;
				else if (y == WATER_LEVEL && y > heights[x][z])
					m_BlockMap[x][y][z] = BlockType::Water;
				else if (y > heights[x][z] && y < WATER_LEVEL)
					m_BlockMap[x][y][z] = BlockType::Air;
				else if (y == heights[x][z] && y < WATER_LEVEL + 2)
					m_BlockMap[x][y][z] = BlockType::Sand;
				else if (y == heights[x][z])
				{
					m_BlockMap[x][y][z] = BlockType::Grass;

					if (x >= 3 && x <= C_WIDTH - 3 && z >= 3 && z <= C_WIDTH - 3)
					{
						mtrInt randNum = treeDist(rng);

						if (randNum > 90)
						{
							mtrInt treeHeight = treeHightDist(rng);

							// Tree trunk
							for (mtrInt i = 1; i <= treeHeight; i++)
								m_BlockMap[x][y + i][z] = BlockType::Wood;

							// Tree crown
							for (mtrInt i = x - 2; i <= x + 2; i++)
								for (mtrInt j = z - 2; j <= z + 2; j++)
								{
									m_BlockMap[i][y + treeHeight + 1][j] = BlockType::Leaf;
									m_BlockMap[i][y + treeHeight + 2][j] = BlockType::Leaf;
								}

							for (mtrInt i = x - 1; i <= x + 1; i++)
								for (mtrInt j = z - 1; j <= z + 1; j++)
									m_BlockMap[i][y + treeHeight + 3][j] = BlockType::Leaf;

							for (mtrInt i = x - 1; i <= x + 1; i++)
								m_BlockMap[i][y + treeHeight + 4][z] = BlockType::Leaf;

							for (mtrInt i = z - 1; i <= z + 1; i++)
								m_BlockMap[x][y + treeHeight + 4][i] = BlockType::Leaf;
						}
					}
				}
				else if (y < heights[x][z] && y >= heights[x][z] - 5)
					m_BlockMap[x][y][z] = BlockType::Dirt;
				else if (y >= 3 && y < heights[x][z] - 5)
					m_BlockMap[x][y][z] = BlockType::Stone;
				else if (y < 3)
					m_BlockMap[x][y][z] = BlockType::Bedrock;
			}
		}
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------
void Chunk::UpdateMeshData()
{
	// Make sure all mesh data is reset
	ClearMeshData();

	// Fill mesh data
	for (mtrU32 y = 0; y < C_HEIGHT; y++)
	{
		for (mtrU32 x = 0; x < C_WIDTH; x++)
		{
			for (mtrU32 z = 0; z < C_WIDTH; z++)
			{
				if (m_BlockMap[x][y][z] != Air)
					AddFaces(x, y, z);
			}
		}
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------
void Chunk::ClearMeshData()
{
	m_ChunkEntity.GetComponent<MeshRendererComponent>().Meshes.clear();
	m_Positions.clear();
	m_UVs.clear();
	m_Normals.clear();
	m_Colors.clear();

	m_SolidIndices.clear();
	m_TransparentIndices.clear();
	m_WaterIndices.clear();
	m_IndicesCount = 0;
}

// --------------------------------------------------------------------------------------------------------------------------------------
void Chunk::EditBlock(mtrInt x, mtrInt y, mtrInt z, BlockType block)
{
	m_BlockMap[x][y][z] = block;
}

// --------------------------------------------------------------------------------------------------------------------------------------
BlockType Chunk::GetBlock(mtrInt x, mtrInt y, mtrInt z)
{
	if (IsBlockInChunk(x, y, z))
		return m_BlockMap[x][y][z];
	return Air;
}

// --------------------------------------------------------------------------------------------------------------------------------------
mtrBool Chunk::IsRenderable()
{
	if (!m_ChunkEntity || m_ChunkEntity.GetComponent<MeshRendererComponent>().Meshes.empty())
		return false;

	for (auto mesh : m_ChunkEntity.GetComponent<MeshRendererComponent>().Meshes)
		if (mesh == nullptr)
			return false;

	return true;
}

// --------------------------------------------------------------------------------------------------------------------------------------
void Chunk::AddFaces(mtrInt x, mtrInt y, mtrInt z)
{
	BlockType blockType = m_BlockMap[x][y][z];
	glm::vec3 position = { x, y, z };

	if (blockType == Water)
	{
		// If water add only top and bottom face
		for (mtrU32 face = TopFace; face <= BottomFace; face++)
		{
			m_Positions.push_back(glm::vec3(position.x, face == TopFace ? position.y - 0.2f : position.y + 0.8f, position.z) + BlockData::VertexPositions[BlockData::FaceTriangles[face][0]]);
			m_Positions.push_back(glm::vec3(position.x, face == TopFace ? position.y - 0.2f : position.y + 0.8f, position.z) + BlockData::VertexPositions[BlockData::FaceTriangles[face][1]]);
			m_Positions.push_back(glm::vec3(position.x, face == TopFace ? position.y - 0.2f : position.y + 0.8f, position.z) + BlockData::VertexPositions[BlockData::FaceTriangles[face][2]]);
			m_Positions.push_back(glm::vec3(position.x, face == TopFace ? position.y - 0.2f : position.y + 0.8f, position.z) + BlockData::VertexPositions[BlockData::FaceTriangles[face][3]]);

			AddTexture(BlockData::BlockTypes[Water].GetTextureID(face));

			m_Normals.push_back(BlockData::FaceDirections[face]);
			m_Normals.push_back(BlockData::FaceDirections[face]);
			m_Normals.push_back(BlockData::FaceDirections[face]);
			m_Normals.push_back(BlockData::FaceDirections[face]);

			m_Colors.push_back({ 0.0f, 0.0f, 0.0f, 1.0f });
			m_Colors.push_back({ 0.0f, 0.0f, 0.0f, 1.0f });
			m_Colors.push_back({ 0.0f, 0.0f, 0.0f, 1.0f });
			m_Colors.push_back({ 0.0f, 0.0f, 0.0f, 1.0f });

			m_WaterIndices.push_back(m_IndicesCount);
			m_WaterIndices.push_back(m_IndicesCount + 1);
			m_WaterIndices.push_back(m_IndicesCount + 2);
			m_WaterIndices.push_back(m_IndicesCount + 2);
			m_WaterIndices.push_back(m_IndicesCount + 1);
			m_WaterIndices.push_back(m_IndicesCount + 3);

			m_IndicesCount += 4;
		}
	}
	else
	{
		// Check every face
		for (mtrU32 face = 0; face < 6; face++)
		{
			glm::vec3 neighbourBlockPos = position + BlockData::FaceDirections[face];
			BlockType neighbourBlockType = GetBlock((mtrInt)neighbourBlockPos.x, (mtrInt)neighbourBlockPos.y, (mtrInt)neighbourBlockPos.z);

			// Only add face if the neighbour block is not solid
			if (neighbourBlockType == Air || BlockData::BlockTypes[neighbourBlockType].IsTransparent)
			{
				m_Positions.push_back(position + BlockData::VertexPositions[BlockData::FaceTriangles[face][0]]);
				m_Positions.push_back(position + BlockData::VertexPositions[BlockData::FaceTriangles[face][1]]);
				m_Positions.push_back(position + BlockData::VertexPositions[BlockData::FaceTriangles[face][2]]);
				m_Positions.push_back(position + BlockData::VertexPositions[BlockData::FaceTriangles[face][3]]);

				AddTexture(BlockData::BlockTypes[blockType].GetTextureID(face));

				m_Normals.push_back(BlockData::FaceDirections[face]);
				m_Normals.push_back(BlockData::FaceDirections[face]);
				m_Normals.push_back(BlockData::FaceDirections[face]);
				m_Normals.push_back(BlockData::FaceDirections[face]);

				mtrFloat lightLevel = 0.0f;

				switch (face)
				{
					case BackFace:
					case FrontFace:		
						lightLevel = 0.8f; 
						break;
					case TopFace:		
						lightLevel = 1.0f; 
						break;
					case BottomFace:	
						lightLevel = 0.4f; 
						break;
					case LeftFace:
					case RightFace:		
						lightLevel = 0.6f; 
						break;
				}

				m_Colors.push_back({ 0.0f, 0.0f, 0.0f, lightLevel });
				m_Colors.push_back({ 0.0f, 0.0f, 0.0f, lightLevel });
				m_Colors.push_back({ 0.0f, 0.0f, 0.0f, lightLevel });
				m_Colors.push_back({ 0.0f, 0.0f, 0.0f, lightLevel });

				if (!BlockData::BlockTypes[blockType].IsTransparent)
				{
					// Add to the solid submesh
					m_SolidIndices.push_back(m_IndicesCount);
					m_SolidIndices.push_back(m_IndicesCount + 1);
					m_SolidIndices.push_back(m_IndicesCount + 2);
					m_SolidIndices.push_back(m_IndicesCount + 2);
					m_SolidIndices.push_back(m_IndicesCount + 1);
					m_SolidIndices.push_back(m_IndicesCount + 3);
				}
				else
				{
					// Add to the transparent submesh
					m_TransparentIndices.push_back(m_IndicesCount);
					m_TransparentIndices.push_back(m_IndicesCount + 1);
					m_TransparentIndices.push_back(m_IndicesCount + 2);
					m_TransparentIndices.push_back(m_IndicesCount + 2);
					m_TransparentIndices.push_back(m_IndicesCount + 1);
					m_TransparentIndices.push_back(m_IndicesCount + 3);
				}

				m_IndicesCount += 4;
			}
		}
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------
void Chunk::AddTexture(mtrInt textureID)
{
	mtrFloat y = textureID / BlockData::TextureAtlasSizeInBlocks;
	mtrFloat x = textureID - (y * BlockData::TextureAtlasSizeInBlocks);

	x *= BlockData::TextureAtlasBlockSize;
	y *= BlockData::TextureAtlasBlockSize;

	y = y + BlockData::TextureAtlasBlockSize;

	m_UVs.push_back(glm::vec2(x, y));
	m_UVs.push_back(glm::vec2(x, y - BlockData::TextureAtlasBlockSize));
	m_UVs.push_back(glm::vec2(x + BlockData::TextureAtlasBlockSize, y));
	m_UVs.push_back(glm::vec2(x + BlockData::TextureAtlasBlockSize, y - BlockData::TextureAtlasBlockSize));
}

// --------------------------------------------------------------------------------------------------------------------------------------
mtrBool Chunk::IsBlockInChunk(mtrInt x, mtrInt y, mtrInt z)
{
	if (x < 0 || x > C_WIDTH - 1 || y < 0 || y > C_HEIGHT - 1 || z < 0 || z > C_WIDTH - 1)
		return false;

	return true;
}