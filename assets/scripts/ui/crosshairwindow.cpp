#include "crosshairwindow.h"
#include <imgui\imgui.h>

CrosshairWindow::CrosshairWindow(Ref<Texture2D> crosshairTexture)
    : m_Texture(crosshairTexture)
{
}

void CrosshairWindow::OnImGuiRender()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos({ viewport->GetWorkPos().x + viewport->GetWorkSize().x / 2.0f - 25, viewport->GetWorkPos().y + viewport->GetWorkSize().y / 2.0f - 25 });
	ImGui::SetNextWindowSize({ 50, 50 });
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });

	ImGui::Begin("Crosshair", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);
	ImGui::Image(m_Texture->GetTextureHandle(), { 50.0f, 50.0f });
	ImGui::End();

	ImGui::PopStyleVar();
}
