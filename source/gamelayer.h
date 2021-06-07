#pragma once

#include <meteor.h>

#include "../assets/scripts/ui/toolbar.h"
#include "../assets/scripts/ui/inventory.h"
#include "../assets/scripts/ui/pausemenu.h"
#include "../assets/scripts/ui/statswindow.h"
#include "../assets/scripts/ui/crosshairwindow.h"

class GameLayer : public meteor::Layer
{
public:
	GameLayer();
	~GameLayer();

	virtual void					OnAttach() override;
	virtual void					OnDetach() override;

	virtual void					OnUpdate(meteor::Timestep ts) override;
	virtual void					OnEvent(meteor::Event& event) override;
	virtual void					OnImGuiRender() override;

private:
	bool							OnKeyPressed(meteor::KeyPressedEvent& event);
	bool							OnMouseButtonPressed(meteor::MouseButtonPressedEvent& event);
	bool							OnWindowResized(meteor::WindowResizedEvent& event);
private:
	meteor::Ref<meteor::Shader>		m_ChunkShader;
	meteor::Ref<meteor::Scene>		m_Scene;

	meteor::Ref<meteor::Material>	m_ChunkMaterial;

	meteor::Ref<meteor::Texture2D>	m_BlocksUITexture;
	meteor::Ref<meteor::Texture2D>  m_CrosshairTexture;

	meteor::Ref<ToolBar>			m_ToolBar;
	meteor::Ref<Inventory>			m_Inventory;
	meteor::Ref<PauseMenu>			m_PauseMenu;
	meteor::Ref<StatsWindow>	    m_StatsWindow;
	meteor::Ref<CrosshairWindow>	m_CrossHairWindow;
};