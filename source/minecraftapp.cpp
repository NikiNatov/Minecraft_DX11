#include <meteor\core\entrypoint.h>
#include "gamelayer.h"

class MinecraftApplication : public meteor::Application
{
public:
	MinecraftApplication()
	{
		PushLayer(new GameLayer());
	}

	~MinecraftApplication()
	{
	}
};

meteor::Application* meteor::CreateApplication()
{
	return new MinecraftApplication();
}