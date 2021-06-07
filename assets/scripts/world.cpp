#include "world.h"
#include "chunk.h"
#include "helpers.h"

// --------------------------------------------------------------------------------------------------------------------------------------
void World::OnCreate()
{
#if 0
	std::random_device device;
	std::mt19937 rng(device());
	std::uniform_int_distribution<mtrInt> dist(1, 10000);
	Seed = dist(rng);
#endif

	Seed = 40713;
    Player = GetEntity().GetScene()->FindEntity("Player");
	Player.GetComponent<TransformComponent>().Position = { 380.0f, 55.0f, 161.0f };

    CreateMaterials();

	// Start chunk update thread
	m_ChunkThread = std::thread([&]()
	{
		while (!m_CloseChunkThread)
		{
			UpdateChunks();
			std::this_thread::sleep_for(std::chrono::microseconds(100));
		}	
	});

#if defined (METEOR_DEBUG)
	// Bigger thread pause time for debug since chuncks load slower
	std::this_thread::sleep_for(std::chrono::milliseconds(16000));
#else
	std::this_thread::sleep_for(std::chrono::milliseconds(6000));
#endif
}

// --------------------------------------------------------------------------------------------------------------------------------------
void World::OnUpdate(Timestep ts)
{
	for (auto [pos, chunk] : m_ActiveChunks)
	{
		if (!chunk->IsRenderable())
			chunk->CreateMesh();
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------
void World::OnDestroy()
{
	m_CloseChunkThread = true;
	m_ChunkThread.join();
}

// --------------------------------------------------------------------------------------------------------------------------------------
void World::UpdateChunks()
{
	glm::vec3 playerPos = Player.GetComponent<TransformComponent>().Position;

	mtrInt currentChunkX = playerPos.x / Chunk::C_WIDTH;
	mtrInt currentChunkZ = playerPos.z / Chunk::C_WIDTH;

	for (auto it = m_ActiveChunks.begin(); it != m_ActiveChunks.end();)
	{
		if (it->first.x > currentChunkX + ChunkRenderDistance || it->first.x < currentChunkX - ChunkRenderDistance ||
			it->first.y > currentChunkZ + ChunkRenderDistance || it->first.y < currentChunkZ - ChunkRenderDistance)
		{
			std::lock_guard<std::mutex> lock(m_ChunkUpdateThreadLock);
			m_ActiveChunks[it->first]->ReleaseEntity();
			it = m_ActiveChunks.erase(it);
			continue;
		}
		else
			it++;
	}

	if (m_ActiveChunks.find({ currentChunkX, currentChunkZ }) == m_ActiveChunks.end())
	{
		std::lock_guard<std::mutex> lock(m_ChunkUpdateThreadLock);
		m_ActiveChunks[{ currentChunkX, currentChunkZ }] = CreateRef<Chunk>(glm::vec2(currentChunkX, currentChunkZ), this);
	}

	for (mtrInt x = -ChunkRenderDistance; x <= ChunkRenderDistance; x++)
	{
		for (mtrInt z = -ChunkRenderDistance; z <= ChunkRenderDistance; z++)
		{
			if (m_ActiveChunks.find({ currentChunkX + x, currentChunkZ + z }) == m_ActiveChunks.end())
			{
				std::lock_guard<std::mutex> lock(m_ChunkUpdateThreadLock);
				m_ActiveChunks[{ currentChunkX + x, currentChunkZ + z }] = CreateRef<Chunk>(glm::vec2(currentChunkX + x, currentChunkZ + z), this);
			}
		}
	}
	
}

// --------------------------------------------------------------------------------------------------------------------------------------
Ref<Chunk> World::GetChunk(const glm::vec3& position)
{
    glm::ivec2 chunkPos = GetChunkPositionFromWorldPosition(position);
    METEOR_ASSERT(m_ActiveChunks.find(chunkPos) != m_ActiveChunks.end(), "Chunk does not exist!");
    return m_ActiveChunks.at(chunkPos);
}

// --------------------------------------------------------------------------------------------------------------------------------------
void World::EditBlock(const glm::vec3& worldPosition, BlockType block)
{
    glm::ivec3 blockPosInChunk = GetBlockPositionInChunk(worldPosition);
    GetChunk(worldPosition)->EditBlock(blockPosInChunk.x, blockPosInChunk.y, blockPosInChunk.z, block);
}

// --------------------------------------------------------------------------------------------------------------------------------------
BlockType World::GetBlock(const glm::vec3& worldPosition)
{
    glm::ivec3 blockPos = GetBlockPositionInChunk(worldPosition);
    return GetChunk(worldPosition)->GetBlock(blockPos.x, blockPos.y, blockPos.z);
}

// --------------------------------------------------------------------------------------------------------------------------------------
void World::CreateMaterials()
{
    Ref<Shader> chunkShader = GfxContext::GetDevice()->CreateShader("assets/shaders/ChunkShader.hlsl");

    TextureSpecification spec;
    spec.Wrap = TextureWrap::Clamp;
    spec.Filter = TextureFilter::Linear;
    Ref<Texture2D> spriteSheet = GfxContext::GetDevice()->CreateTexture2D("assets/textures/minecraft_spritesheet.png", spec);

    BlocksMaterialSolid = CreateRef<Material>(chunkShader);
	BlocksMaterialSolid->SetTexture("u_Texture", spriteSheet);

    BlocksMaterialTransparent = CreateRef<Material>(chunkShader, Material::Transparent);
    BlocksMaterialTransparent->SetTexture("u_Texture", spriteSheet);

	BlocksMaterialWater = CreateRef<Material>(chunkShader, Material::Water | Material::Transparent);
	BlocksMaterialWater->SetTexture("u_Texture", spriteSheet);
}






