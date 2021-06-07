#include "fpscamera.h"

// --------------------------------------------------------------------------------------------------------------------------------------
void FPSCamera::OnCreate()
{
}

// --------------------------------------------------------------------------------------------------------------------------------------
void FPSCamera::OnUpdate(meteor::Timestep ts)
{
	// Lock cursor
	if (meteor::Input::IsMouseButtonPressed(meteor::MouseButton::Right))
	{
		if (meteor::Input::IsCursorEnabled())
		{
			meteor::Input::SetMouseCursor(false);
		}
	}

	// If cursor is locked do calculations
	if (!meteor::Input::IsCursorEnabled())
	{
		glm::vec2 windowSize = { meteor::Application::GetApplication().GetWindow()->GetWidth(), meteor::Application::GetApplication().GetWindow()->GetHeight() };
		glm::vec2 windowCenter = { (mtrInt)(windowSize.x / 2.0f), (mtrInt)(windowSize.y / 2.0f) };

		glm::vec2 mousePos = meteor::Input::GetMousePosition();
		meteor::Input::SetMousePosition(windowCenter);
		glm::vec2 delta = mousePos - windowCenter;

		auto& transform = GetComponent<meteor::TransformComponent>();
		transform.Rotation.x -= delta.y * m_MouseSensitivity * ts;

		if (transform.Rotation.x > 89.0f)
			transform.Rotation.x = 89.0f;
		else if (transform.Rotation.x < -89.0f)
			transform.Rotation.x = -89.0f;

	}

	// Unlock cursor
	if (meteor::Input::IsKeyPressed(meteor::Key::LWinKey))
	{
		meteor::Input::SetMouseCursor(true);
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------
void FPSCamera::OnDestroy()
{

}