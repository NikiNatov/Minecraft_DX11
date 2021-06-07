#include "pausemenu.h"
#include <imgui\imgui.h>

// --------------------------------------------------------------------------------------------------------------------------------------
void PauseMenu::Toggle()
{
    m_Open = !m_Open;
}

// --------------------------------------------------------------------------------------------------------------------------------------
void PauseMenu::OnImGuiRender()
{
    if (m_Open)
    {
        Application& app = Application::GetApplication();

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos({ viewport->GetWorkPos().x + viewport->GetWorkSize().x / 2.0f - 260, viewport->GetWorkPos().y + viewport->GetWorkSize().y / 2.0f - 200 });
        ImGui::SetNextWindowSize({ 520, 400 });

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 30, 55 });
        ImGui::Begin("Pause", &m_Open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        
        if (ImGui::Button("Return to game", ImVec2{ 460, 50 }))
        {
            m_Open = false;
        }

        ImGui::Dummy(ImVec2(0.0f, 40.0f));
        
        if (ImGui::Button(app.GetWindow()->IsFullscreen() ? "Exit Fullscreen" : "Enter Fullscreen" , ImVec2{ 460, 50 }))
        {
            app.GetWindow()->ToogleFullscreen();
        }
        
        ImGui::Dummy(ImVec2(0.0f, 40.0f));

        if (ImGui::Button("Exit", ImVec2{ 460, 50 }))
        {
            app.Close();
        }

        ImGui::End();
        ImGui::PopStyleVar();
    }
}
