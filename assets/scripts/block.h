#pragma once

#include <meteor.h>

using namespace meteor;

enum BlockFace
{
	BackFace	= 0,
	FrontFace	= 1,
	TopFace		= 2,
	BottomFace  = 3,
	LeftFace	= 4,
	RightFace	= 5
};

enum BlockType
{
	Air		= -1,
	Grass	= 0,
	Dirt	= 1,
	Stone	= 2,
	Bedrock = 3,
	Sand	= 4,
	Wood	= 5,
	Leaf	= 6,
	Plank	= 7,
	Glass	= 8,
	Water	= 9
};

struct Block
{
	BlockType	Type;
	mtrBool		IsSolid;
	mtrBool		IsTransparent;

	mtrInt		BackFaceTexture;
	mtrInt		FrontFaceTexture;
	mtrInt		TopFaceTexture;
	mtrInt		BottomFaceTexture;
	mtrInt		LeftFaceTexture;
	mtrInt		RightFaceTexture;

	mtrInt		BlockIconID;

	Block() = default;

	Block(BlockType type,
		mtrInt backTexID, mtrInt frontTexID,
		mtrInt topTexID, mtrInt bottomTexID,
		mtrInt leftTexID, mtrInt rightTexID,
		mtrBool solid, mtrBool transparent, mtrInt blockIconID)
		: Type(type), BackFaceTexture(backTexID), FrontFaceTexture(frontTexID), TopFaceTexture(topTexID), BottomFaceTexture(bottomTexID), LeftFaceTexture(leftTexID), RightFaceTexture(rightTexID), IsSolid(solid), IsTransparent(transparent), BlockIconID(blockIconID)
	{

	}

	mtrInt GetTextureID(mtrInt face)
	{
		switch (face) {

		case 0:
			return BackFaceTexture;
		case 1:
			return FrontFaceTexture;
		case 2:
			return TopFaceTexture;
		case 3:
			return BottomFaceTexture;
		case 4:
			return LeftFaceTexture;
		case 5:
			return RightFaceTexture;
		default:
			return 0;
		}
	}
};

namespace BlockData
{
	static constexpr mtrInt		TextureAtlasSizeInBlocks = 16;
	static constexpr mtrFloat	TextureAtlasBlockSize = 1.0f / (mtrFloat)TextureAtlasSizeInBlocks;
	static constexpr glm::vec3	VertexPositions[8] =
	{
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(1.0f, 0.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.0f, 1.0f, 1.0f),
	};

	static constexpr glm::vec3 FaceDirections[6] =
	{
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f)
	};

	static constexpr mtrU32 FaceTriangles[6][4] =
	{
		{ 0, 3, 1, 2 }, // Back Face
		{ 5, 6, 4, 7 }, // Front Face
		{ 3, 7, 2, 6 }, // Top Face
		{ 1, 5, 0, 4 }, // Bottom Face
		{ 4, 7, 0, 3 }, // Left Face
		{ 1, 2, 5, 6 } // Right Face
	};

	static std::vector<Block> BlockTypes =
	{
		Block(Grass, 3, 3, 0, 2, 3, 3, true, false, 1),
		Block(Dirt, 2, 2, 2, 2, 2, 2, true, false, 2),
		Block(Stone, 1, 1, 1, 1, 1, 1, true, false, 0),
		Block(Bedrock, 17, 17, 17, 17, 17, 17, true, false, 8),
		Block(Sand, 18, 18, 18, 18, 18, 18, true, false, 11),
		Block(Wood, 20, 20, 21, 21, 20, 20, true, false, 17),
		Block(Leaf, 52, 52, 52, 52, 52, 52, true, false, 20),
		Block(Plank, 4, 4, 4, 4, 4, 4, true, false, 4),
		Block(Glass, 49, 49, 49, 49, 49, 49, true, true, 25),
		Block(Water, 205, 205, 205, 205, 205, 205, false, true, 9)
	};
};
