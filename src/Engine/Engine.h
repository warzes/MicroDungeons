#pragma once

#include "IAppState.h"
#include "Log.h"

struct EngineCreateInfo
{
	struct
	{
		int width = 1024;
		int height = 768;
		const char* title = "Super Raycast Game";
		bool resize = true;
		bool vsync = false;
		bool fullscreen = false;
	} window;
};

class Engine
{
public:
	~Engine();

	bool Create(const EngineCreateInfo& createInfo);

	void SetActiveState(IAppState* newState)
	{
		if (m_currentState) m_currentState->Stop(this);
		m_currentState = newState;
		if (m_currentState)
		{
			if (!m_currentState->Start(this))
			{
				m_currentState->Stop(this);
				m_currentState = nullptr;
			}
		}
	}

	void Run();

private:
	IAppState* m_currentState = nullptr;
	Log *m_log = nullptr;
};