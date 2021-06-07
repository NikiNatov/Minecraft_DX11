#include "helpers.h"
#include "chunk.h"

// --------------------------------------------------------------------------------------------------------------------------------------
glm::ivec3 GetBlockPositionInChunk(const glm::vec3& globalPosition)
{
	return 
	{
		(Chunk::C_WIDTH + ((mtrInt)globalPosition.x % Chunk::C_WIDTH)) % Chunk::C_WIDTH,
		(Chunk::C_HEIGHT + ((mtrInt)globalPosition.y % Chunk::C_HEIGHT)) % Chunk::C_HEIGHT,
		(Chunk::C_WIDTH + ((mtrInt)globalPosition.z % Chunk::C_WIDTH)) % Chunk::C_WIDTH
	};
}

// --------------------------------------------------------------------------------------------------------------------------------------
glm::ivec3 GetBlockCoordinates(const glm::vec3& position)
{
	return { (mtrInt)std::floor(position.x), (mtrInt)std::floor(position.y), (mtrInt)std::floor(position.z) };
}

// --------------------------------------------------------------------------------------------------------------------------------------
glm::ivec2 GetChunkPositionFromWorldPosition(const glm::vec3& worldPosition)
{
	mtrInt x, z;

	if (worldPosition.x >= 0 || (mtrInt)worldPosition.x % Chunk::C_WIDTH == 0)
		x = worldPosition.x / Chunk::C_WIDTH;
	else
		x = worldPosition.x / Chunk::C_WIDTH - 1;

	if (worldPosition.z >= 0 || (mtrInt)worldPosition.z % Chunk::C_WIDTH == 0)
		z = worldPosition.z / Chunk::C_WIDTH;
	else
		z = worldPosition.z / Chunk::C_WIDTH - 1;


	return { x, z };
}

// --------------------------------------------------------------------------------------------------------------------------------------
std::vector<glm::vec3> CastRayAndGetIntersectingBlocks(const glm::vec3& origin, const glm::vec3& direction, mtrFloat distance)
{
	// Raycasting algorithm:
	// https://gamedev.stackexchange.com/questions/47362/cast-ray-to-select-block-in-voxel-game

	glm::ivec3 currentCubePosition = GetBlockCoordinates(origin);
	glm::vec3 endPoint = origin + distance * direction;

	mtrInt stepX = direction.x > 0.0f ? 1 : (direction.x < 0.0f ? -1 : 0);
	mtrInt stepY = direction.y > 0.0f ? 1 : (direction.y < 0.0f ? -1 : 0);
	mtrInt stepZ = direction.z > 0.0f ? 1 : (direction.z < 0.0f ? -1 : 0);

	glm::vec3 delta = { (stepX != 0) ? fmin(stepX / (endPoint.x - origin.x), FLT_MAX) : FLT_MAX,
						(stepY != 0) ? fmin(stepY / (endPoint.y - origin.y), FLT_MAX) : FLT_MAX,
						(stepZ != 0) ? fmin(stepZ / (endPoint.z - origin.z), FLT_MAX) : FLT_MAX };

	mtrFloat tMaxX = (stepX > 0.0f) ? delta.x * (1.0f - origin.x + currentCubePosition.x) : delta.x * (origin.x - currentCubePosition.x);
	mtrFloat tMaxY = (stepY > 0.0f) ? delta.y * (1.0f - origin.y + currentCubePosition.y) : delta.y * (origin.y - currentCubePosition.y);
	mtrFloat tMaxZ = (stepZ > 0.0f) ? delta.z * (1.0f - origin.z + currentCubePosition.z) : delta.z * (origin.z - currentCubePosition.z);

	std::vector<glm::vec3> intersected;

	while (intersected.size() < distance * 3)
	{
		if (tMaxX < tMaxY)
		{
			if (tMaxX < tMaxZ)
			{
				tMaxX += delta.x;
				currentCubePosition.x += stepX;
			}
			else
			{
				tMaxZ += delta.z;
				currentCubePosition.z += stepZ;
			}
		}
		else
		{
			if (tMaxY < tMaxZ)
			{
				tMaxY += delta.y;
				currentCubePosition.y += stepY;
			}
			else
			{
				tMaxZ += delta.z;
				currentCubePosition.z += stepZ;
			}
		}
		if (tMaxX > 1 && tMaxY > 1 && tMaxZ > 1)
			break;

		intersected.push_back(currentCubePosition);
	}

	return intersected;
}