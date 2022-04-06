#include "stdafx.h"
#include "Engine.h"

https://www.unrealengine.com/marketplace/en-US/product/retrodungeons-the-forest
https://voxelvoid.itch.io/dark-lords-maze
https://www.youtube.com/watch?v=fu3ZpCcQImE
https://www.youtube.com/watch?v=puAVRRkYxQE



//-----------------------------------------------------------------------------
#if defined(_MSC_VER)
extern "C"
{
	// NVIDIA: Force usage of NVidia GPU in case there is an integrated graphics unit as well, if we don't do this we risk getting the integrated graphics unit and hence a horrible performance
	// -> See "Enabling High Performance Graphics Rendering on Optimus Systems" http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
	_declspec(dllexport) auto NvOptimusEnablement = 0x00000001UL;

	// AMD: Force usage of AMD GPU in case there is an integrated graphics unit as well, if we don't do this we risk getting the integrated graphics unit and hence a horrible performance
	// -> Named "Dynamic Switchable Graphics", found no official documentation, only https://community.amd.com/message/1307599#comment-1307599 - "Can an OpenGL app default to the discrete GPU on an Enduro system?"
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif
//-----------------------------------------------------------------------------
Engine::~Engine()
{
	if (m_currentState) m_currentState->Stop(this);

	CloseAudioDevice();
	rCloseWindow();
	m_log->Print("Engine end.");
	delete m_log;

#if SE_DEBUG && SE_COMPILER_MSVC && SE_PLATFORM_WINDOWS
	_CrtDumpMemoryLeaks(); // GLFW каким-то отключает вывод меморилика, поэтому приходится принудительно вызывать
#endif
}
//-----------------------------------------------------------------------------
bool Engine::Create(const EngineCreateInfo& createInfo)
{
#if SE_DEBUG && SE_COMPILER_MSVC && SE_PLATFORM_WINDOWS
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); // Get current flag
	flag |= _CRTDBG_LEAK_CHECK_DF;   // Turn on leak-checking bit
	flag |= _CRTDBG_CHECK_ALWAYS_DF; // Turn on CrtCheckMemory
	_CrtSetDbgFlag(flag); // Set flag to the new value
#endif

	m_log = new Log("../Log.txt");
	m_log->Print("Engine Init...");

	InitWindow(1024, 768, "Game");
	if (!IsWindowReady())
	{
		ErrorLog("InitWindow failed!");
		return false;
	}

	SetTargetFPS(60);

	InitAudioDevice();
	if (!IsAudioDeviceReady())
	{
		ErrorLog("Audio device failed!");
		return false;
	}

	return true;
}

void Engine::Run()
{
	while (!WindowShouldClose())
	{
		const float dt = GetFrameTime();

		if (m_currentState)
		{
			m_currentState->Update(this, dt);
			m_currentState->Frame(this, dt);
		}
	}
}