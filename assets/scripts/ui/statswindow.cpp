#include "statswindow.h"
#include <imgui\imgui.h>

// --------------------------------------------------------------------------------------------------------------------------------------
StatsWindow::StatsWindow(mtrU32 gameVersionMajor, mtrU32 gameVersionMinor, Entity player)
	: m_Player(player), m_GameVersionMajor(gameVersionMajor), m_GameVersionMinor(gameVersionMinor)
{
}

// --------------------------------------------------------------------------------------------------------------------------------------
void StatsWindow::OnImGuiRender()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->GetWorkPos());
	ImGui::Begin("Simple text", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);
	ImGui::Text("Minecraft %d.%d", m_GameVersionMajor, m_GameVersionMinor);

	if (m_Player)
	{
		const auto& playerPos = m_Player.GetComponent<TransformComponent>().Position;
		ImGui::Text("FPS: %.0f", ImGui::GetIO().Framerate);
		ImGui::Text("X: %.2f, Y: %.2f, Z: %.2f", playerPos.x, playerPos.y, playerPos.z);
	}

	ImGui::End();
}
