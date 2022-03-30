#include "stdafx.h"
#include "Game.h"
#include "ResourceManager.h"
//-----------------------------------------------------------------------------
#if defined(_MSC_VER)
#	pragma comment( lib, "winmm.lib" )
#	pragma comment( lib, "3rdparty.lib" )
#endif
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
void ImplMain()
{
	const int screenWidth = 1024;
	const int screenHeight = 768;
	InitWindow(screenWidth, screenHeight, "Game");
	SetTargetFPS(60);
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	//SetExitKey(0);

	{
		ResourceManager resourceManager;
		Game game;
		while (!WindowShouldClose())
		{
			game.Frame();
			game.Update();
		}
		game.Close();
	}

	rCloseWindow();
}

struct one
{
	uint8_t upheight;
	uint8_t downheight;
};

struct tile
{
	one o[100][100];
};

struct HeightMap
{
	tile t[100][100];
};

constexpr auto SizeMap = sizeof(HeightMap);


//-----------------------------------------------------------------------------
int main(
	[[maybe_unused]] int   argc,
	[[maybe_unused]] char* argv[])
{
	HeightMap *mm = new HeightMap;

	std::cout << SizeMap / 1024 / 1024 << " mb";






	ImplMain();
	return 0;
}
//-----------------------------------------------------------------------------