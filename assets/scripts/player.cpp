#include "player.h"

// --------------------------------------------------------------------------------------------------------------------------------------
void Player::OnCreate()
{
	m_World = static_cast<World*>(GetEntity().GetScene()->FindEntity("World").GetComponent<ScriptComponent>().Instance);
	m_Camera = GetEntity().GetScene()->FindEntity("Camera");
}

// --------------------------------------------------------------------------------------------------------------------------------------
void Player::OnUpdate(Timestep ts)
{
	// If cursor is locked do calculations
	if (!Input::IsCursorEnabled())
	{
		glm::vec2 windowSize = { meteor::Application::GetApplication().GetWindow()->GetWidth(), meteor::Application::GetApplication().GetWindow()->GetHeight() };
		glm::vec2 windowCenter = { (mtrInt)(windowSize.x / 2.0f), (mtrInt)(windowSize.y / 2.0f) };

		glm::vec2 mousePos = Input::GetMousePosition();
		glm::vec2 delta = mousePos - windowCenter;

		auto& transform = GetComponent<TransformComponent>();

		transform.Rotation.y -= delta.x * m_MouseSensitivity * ts;
		m_Velocity = glm::vec3(0.0f, m_UnderWater ? 0.0f : m_Velocity.y, 0.0f);

		if (Input::IsKeyPressed(Key::W))
		{
			m_Velocity += transform.GetForwardVector() * m_MovementSpeed * (mtrFloat)ts;
		}
		else if (Input::IsKeyPressed(Key::S))
		{
			m_Velocity += -transform.GetForwardVector() * m_MovementSpeed * (mtrFloat)ts;
		}

		if (Input::IsKeyPressed(Key::D))
		{
			m_Velocity += transform.GetRightVector() * m_MovementSpeed * (mtrFloat)ts;
		}
		else if (Input::IsKeyPressed(Key::A))
		{
			m_Velocity += -transform.GetRightVector() * m_MovementSpeed * (mtrFloat)ts;
		}

		if (Input::IsKeyPressed(Key::Space))
		{
			if (m_UnderWater)
				m_Velocity += GetComponent<TransformComponent>().GetUpVector() * m_MovementSpeed;
			else
				Jump();
		}
		else if (Input::IsKeyPressed(Key::LShift))
		{
			if (m_UnderWater)
				m_Velocity += -GetComponent<TransformComponent>().GetUpVector() * m_MovementSpeed;
		}

		if (!m_UnderWater);
			m_Velocity.y += m_GravityForce * ts;

		transform.Position.x += m_Velocity.x;
		CollisionCheck({ m_Velocity.x, 0.0f, 0.0f });
		transform.Position.y += m_Velocity.y * ts;
		CollisionCheck({ 0.0f, m_Velocity.y * ts, 0.0f });
		transform.Position.z += m_Velocity.z;
		CollisionCheck({ 0.0f, 0.0f, m_Velocity.z });

		m_Velocity.x *= 0.90f;
		m_Velocity.z *= 0.90f;

		m_UnderWater = transform.Position.y < 40.0f;

		m_Camera.GetComponent<TransformComponent>().Position = GetComponent<TransformComponent>().Position;
		m_Camera.GetComponent<TransformComponent>().Rotation.y = GetComponent<TransformComponent>().Rotation.y;
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------
void Player::OnDestroy()
{

}

// --------------------------------------------------------------------------------------------------------------------------------------
void Player::Jump()
{
	if (!m_IsInAir)
	{
		m_Velocity += GetComponent<TransformComponent>().GetUpVector() * m_JumpForce;
		m_IsInAir = true;
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------
void Player::BreakBlock()
{
	Scene* scene = GetEntity().GetScene();
	World* m_World = static_cast<World*>(scene->FindEntity("World").GetComponent<ScriptComponent>().Instance);
	Entity camera = scene->FindEntity("Camera");

	glm::vec3 playerPos = m_World->Player.GetComponent<TransformComponent>().Position;
	glm::vec3 direction = camera.GetComponent<TransformComponent>().GetForwardVector();
	auto intersected = CastRayAndGetIntersectingBlocks(playerPos, direction, m_World->ChunkRenderDistance);

	glm::vec3 prevPosition = intersected.front();

	for (const auto& currentPosition : intersected)
	{
		BlockType block = m_World->GetBlock(currentPosition);

		if (block != Air && block != Water)
		{
			// Handle breaking blocks
			BlockType replacementBlock = Air;

			if (currentPosition.y == Chunk::WATER_LEVEL &&
				(m_World->GetBlock({ currentPosition.x - 1, currentPosition.y, currentPosition.z }) == Water ||
					m_World->GetBlock({ currentPosition.x + 1, currentPosition.y, currentPosition.z }) == Water ||
					m_World->GetBlock({ currentPosition.x, currentPosition.y, currentPosition.z - 1 }) == Water ||
					m_World->GetBlock({ currentPosition.x, currentPosition.y, currentPosition.z + 1 }) == Water))
			{
				replacementBlock = Water;
			}

			m_World->EditBlock(currentPosition, replacementBlock);
			m_World->GetChunk(currentPosition)->Recreate();

			break;
		}
		prevPosition = currentPosition;
	}
}

void Player::PlaceBlock(BlockType blockID)
{
	Scene* scene = GetEntity().GetScene();
	World* m_World = static_cast<World*>(scene->FindEntity("World").GetComponent<ScriptComponent>().Instance);
	Entity camera = scene->FindEntity("Camera");

	glm::vec3 playerPos = m_World->Player.GetComponent<TransformComponent>().Position;
	glm::vec3 direction = camera.GetComponent<TransformComponent>().GetForwardVector();
	auto intersected = CastRayAndGetIntersectingBlocks(playerPos, direction, m_World->ChunkRenderDistance);

	glm::vec3 prevPosition = intersected.front();

	for (const auto& currentPosition : intersected)
	{
		BlockType block = m_World->GetBlock(currentPosition);

		if (block != Air && block != Water)
		{
			// Handle placing blocks
			if (glm::distance(prevPosition, playerPos) >= 1.8f)
			{
				m_World->EditBlock(prevPosition, blockID);
				m_World->GetChunk(prevPosition)->Recreate();
			}

			break;
		}
		prevPosition = currentPosition;
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------
void Player::CollisionCheck(const glm::vec3& velocity)
{
	glm::vec3 bounds = { 0.3f, 2.0f, 0.3f };

	auto& position = GetComponent<TransformComponent>().Position;

	mtrFloat xFrom = position.x - bounds.x < 0.0f ? std::floor(position.x - bounds.x) : position.x - bounds.x;
	mtrFloat xTo = position.x + bounds.x;

	mtrFloat yFrom = position.y - bounds.y < 0.0f ? std::floor(position.y - bounds.y) : position.y - bounds.y;
	mtrFloat yTo = position.y + 0.5f;

	mtrFloat zFrom = position.z - bounds.z < 0.0f ? std::floor(position.z - bounds.z) : position.z - bounds.z;
	mtrFloat zTo = position.z + bounds.z;

	for (mtrInt x = xFrom; x < xTo; x++)
	{
		for (mtrInt y = yFrom; y < yTo; y++)
		{
			for (mtrInt z = zFrom; z < zTo; z++)
			{
				BlockType block = m_World->GetBlock({ x, y, z });
				if (block != Air && block != Water)
				{
					if (velocity.y > 0)
					{
						position.y = y - 0.5f;
						m_Velocity.y = 0;
					}
					else if (velocity.y < 0)
					{
						position.y = y + bounds.y + 1;
						m_Velocity.y = 0;
						m_IsInAir = false;
					}

					if (velocity.x > 0)
					{
						position.x = x - bounds.x;
					}
					else if (velocity.x < 0)
					{
						position.x = x + bounds.x + 1;
					}

					if (velocity.z > 0)
					{
						position.z = z - bounds.z;
					}
					else if (velocity.z < 0)
					{
						position.z = z + bounds.z + 1;
					}
				}
			}
		}
	}
}