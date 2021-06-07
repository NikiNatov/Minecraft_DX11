#pragma once

#include "meteor.h"

#include "block.h"
#include "world.h"
#include "chunk.h"

using namespace meteor;

class Player : public Script
{
public:
	virtual void		OnCreate() override;
	virtual void		OnUpdate(Timestep ts) override;
	virtual void		OnDestroy() override;

	void				Jump();

	void				BreakBlock();
	void				PlaceBlock(BlockType blockID);
private:
	void				CollisionCheck(const glm::vec3& velocity);
private:
	mtrFloat			m_JumpForce = 10.0f;
	mtrFloat			m_GravityForce = -20.0f;
	mtrFloat			m_MouseSensitivity = 7.0f;
	mtrFloat			m_MovementSpeed = 7.0f;
	glm::vec3			m_Velocity = glm::vec3(0.0f);
	mtrBool				m_IsInAir = false;
	mtrBool				m_UnderWater = false;

	World*				m_World;
	Entity				m_Camera;
};
