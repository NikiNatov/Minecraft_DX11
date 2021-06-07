#include "gamelayer.h"
#include <glm\gtc\matrix_transform.hpp>

#include "../assets/scripts/fpscamera.h"
#include "../assets/scripts/player.h"
#include "../assets/scripts/world.h"

#include "ImGui\imgui.h"

using namespace meteor;

// --------------------------------------------------------------------------------------------------------------------------------------
GameLayer::GameLayer()
{
}

// --------------------------------------------------------------------------------------------------------------------------------------
GameLayer::~GameLayer()
{
	m_Scene->OnSceneEnd();
}

// --------------------------------------------------------------------------------------------------------------------------------------
void GameLayer::OnAttach()
{
	// Load shaders
	m_ChunkShader = GfxContext::GetDevice()->CreateShader("assets/shaders/ChunkShader.hlsl");
	GfxContext::SetShader(m_ChunkShader);

	// Load textures
	TextureSpecification spec;
	spec.Wrap = TextureWrap::Clamp;
	spec.Filter = TextureFilter::Linear;

	m_CrosshairTexture = GfxContext::GetDevice()->CreateTexture2D("assets/textures/crosshair.png", spec);
	m_BlocksUITexture = GfxContext::GetDevice()->CreateTexture2D("assets/textures/blockiconsinventory.png", spec);

	// Create scene
	m_Scene = CreateRef<Scene>();

	// Add camera entity
	Entity camera = m_Scene->CreateEntity("Camera");
	camera.AddComponent<ScriptComponent>().Bind<FPSCamera>();
	camera.AddComponent<CameraComponent>().IsPrimary = true;
	auto& cameraTransform = camera.GetComponent<TransformComponent>();
	cameraTransform.Position = { -4.0f, 5.0f, -4.0f };
	cameraTransform.Rotation = { -45.0f, 230.0f, 0.0f };

	// Add player entity
	Entity player = m_Scene->CreateEntity("Player");
	player.AddComponent<ScriptComponent>().Bind<Player>();

	// Add world entity
	Entity world = m_Scene->CreateEntity("World");
	world.AddComponent<ScriptComponent>().Bind<World>();

	// Create UI elements
	m_ToolBar = CreateRef<ToolBar>(m_BlocksUITexture);
	m_Inventory = CreateRef<Inventory>(m_BlocksUITexture);
	m_PauseMenu = CreateRef<PauseMenu>();
	m_StatsWindow = CreateRef<StatsWindow>(1, 4, player);
	m_CrossHairWindow = CreateRef<CrosshairWindow>(m_CrosshairTexture);

	// Start scene
	m_Scene->OnSceneStart();
	Input::SetMouseCursor(false);
}

// --------------------------------------------------------------------------------------------------------------------------------------
void GameLayer::OnDetach()
{
}

// --------------------------------------------------------------------------------------------------------------------------------------
void GameLayer::OnUpdate(Timestep ts)
{
	// Update and render scene
	m_Scene->OnSceneUpdate(ts);
	m_Scene->OnSceneRender();
}

// --------------------------------------------------------------------------------------------------------------------------------------
void GameLayer::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);

	dispatcher.Dispatch<KeyPressedEvent>(METEOR_BIND_EVENT_FN(GameLayer::OnKeyPressed));
	dispatcher.Dispatch<MouseButtonPressedEvent>(METEOR_BIND_EVENT_FN(GameLayer::OnMouseButtonPressed));
	dispatcher.Dispatch<WindowResizedEvent>(METEOR_BIND_EVENT_FN(GameLayer::OnWindowResized));
}

// --------------------------------------------------------------------------------------------------------------------------------------
void GameLayer::OnImGuiRender()
{
	m_StatsWindow->OnImGuiRender();
	m_ToolBar->OnImGuiRender();
	m_Inventory->OnImGuiRender();
	m_PauseMenu->OnImGuiRender();
	m_CrossHairWindow->OnImGuiRender();
}

// --------------------------------------------------------------------------------------------------------------------------------------
bool GameLayer::OnKeyPressed(KeyPressedEvent& event)
{
	if (event.GetKeyCode() == Key::I)
	{
		m_Inventory->Toggle();
		Input::SetMouseCursor(m_Inventory->IsOpen());

		if (m_PauseMenu->IsOpen())
			m_PauseMenu->Toggle();
	}
	else if (event.GetKeyCode() == Key::Key1)
	{
		m_ToolBar->SetActiveItemSlot(0);
	}
	else if (event.GetKeyCode() == Key::Key2)
	{
		m_ToolBar->SetActiveItemSlot(1);
	}
	else if (event.GetKeyCode() == Key::Key3)
	{
		m_ToolBar->SetActiveItemSlot(2);
	}
	else if (event.GetKeyCode() == Key::Key4)
	{
		m_ToolBar->SetActiveItemSlot(3);
	}
	else if (event.GetKeyCode() == Key::Key5)
	{
		m_ToolBar->SetActiveItemSlot(4);
	}
	else if (event.GetKeyCode() == Key::Key6)
	{
		m_ToolBar->SetActiveItemSlot(5);
	}
	else if (event.GetKeyCode() == Key::Key7)
	{
		m_ToolBar->SetActiveItemSlot(6);
	}
	else if (event.GetKeyCode() == Key::Esc)
	{
		m_PauseMenu->Toggle();
		Input::SetMouseCursor(m_PauseMenu->IsOpen());

		if (m_Inventory->IsOpen())
			m_Inventory->Toggle();
	}
	return true;
}

// --------------------------------------------------------------------------------------------------------------------------------------
bool GameLayer::OnMouseButtonPressed(meteor::MouseButtonPressedEvent& event)
{
	Player* player = static_cast<Player*>(m_Scene->FindEntity("Player").GetComponent<ScriptComponent>().Instance);

	if (event.GetButton() == MouseButton::Left)
		player->BreakBlock();
	else if (event.GetButton() == MouseButton::Right)
		player->PlaceBlock(m_ToolBar->GetActiveItemSlot().GetItem()->BlockID);

	return false;
}

// --------------------------------------------------------------------------------------------------------------------------------------
bool GameLayer::OnWindowResized(WindowResizedEvent& event)
{
	m_Scene->OnViewportResize(event.GetWidth(), event.GetHeight());
	return false;
}
