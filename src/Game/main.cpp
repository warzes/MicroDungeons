#include "stdafx.h"
/*
* 2d col - http://www.jeffreythompson.org/collision-detection/table_of_contents.php
* https://github.com/paulahemsi/cub3d
* https://github.com/DanielChappuis/reactphysics3d
* https://github.com/RandyGaul/qu3e
*/
//-----------------------------------------------------------------------------
#if defined(_MSC_VER)
#	pragma comment( lib, "winmm.lib" )
#	pragma comment( lib, "Engine.lib" )
#	pragma comment( lib, "3rdparty.lib" )
#endif
//-----------------------------------------------------------------------------
void ImplMain()
{
	EngineCreateInfo createInfo;
	Engine engine;
	if (engine.Create(createInfo))
	{
		TestState test;
		engine.SetActiveState(&test);

		engine.Run();
	}
}
//-----------------------------------------------------------------------------
int main(
	[[maybe_unused]] int   argc,
	[[maybe_unused]] char* argv[])
{
	ImplMain();
	return 0;
}
//-----------------------------------------------------------------------------