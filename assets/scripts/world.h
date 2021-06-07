#pragma once

#include <meteor.h>
#include <unordered_map>
#include <future>

#include "block.h"
#include "helpers.h"

#include <queue>

using namespace meteor;

class Chunk;

class World : public Script
{
    using ChunkMap = std::unordered_map<glm::ivec2, Ref<Chunk>, IVec2Hash>;
public:
    mtrInt                      Seed;
    Entity                      Player;

    Ref<Material>               BlocksMaterialSolid;
    Ref<Material>               BlocksMaterialTransparent;
    Ref<Material>               BlocksMaterialWater;
    mtrFloat                    ChunkRenderDistance = 10.0f;
private:
    ChunkMap                    m_ActiveChunks;

    // Threading
    std::thread                 m_ChunkThread;
    std::mutex                  m_ChunkUpdateThreadLock;
    mtrBool                     m_CloseChunkThread = false;
public:
    virtual void                OnCreate() override;
    virtual void                OnUpdate(Timestep ts) override;
    virtual void                OnDestroy() override;

    void                        UpdateChunks();

    void                        EditBlock(const glm::vec3& worldPosition, BlockType block);
    BlockType                   GetBlock(const glm::vec3& worldPosition);
    Ref<Chunk>                  GetChunk(const glm::vec3& worldPosition);
private:
    void                        CreateMaterials();    
};