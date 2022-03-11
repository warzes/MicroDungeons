#include "stdafx.h"
#include "Game.h"
#include "Player.h"
//-----------------------------------------------------------------------------
Texture tx;
//rlFPCamera cam;
PlayerCamera playerCamera;
//-----------------------------------------------------------------------------
Game::Game()
{
	Image img = GenImageChecked(256, 256, 32, 32, DARKBLUE, WHITE);
	tx = LoadTextureFromImage(img);
	UnloadImage(img);
	SetTextureFilter(tx, TEXTURE_FILTER_POINT);
	SetTextureWrap(tx, TEXTURE_WRAP_CLAMP);
	
	//cam.Setup(45, Vector3{ 1, 0, 0 });
	//cam.MoveSpeed.z = 10;
	//cam.MoveSpeed.x = 5;
	//cam.FarPlane = 5000;

	playerCamera.Init({ 0.0f, 0.0f, 0.0f });
}
//-----------------------------------------------------------------------------
void Game::Frame()
{
	playerCamera.Update(nullptr, GetFrameTime());
	//cam.Update();

	BeginDrawing();
	//ClearBackground({ 140, 210, 240 });
	ClearBackground({ 50, 100, 200 });

	//cam.BeginMode3D();
	BeginMode3D(playerCamera.GetCamera());
	{
		DrawPlane(Vector3{ 0, 0, 0 }, Vector2{ 50, 50 }, BEIGE);
		DrawCubeTexture(tx, Vector3{ 0, 0.5f, 0 }, 1, 1, 1, WHITE);
	}
	EndMode3D();
	//cam.EndMode3D();

	DrawFPS(0, 0);
	EndDrawing();
}
//-----------------------------------------------------------------------------
void Game::Close()
{
	UnloadTexture(tx);
}
//-----------------------------------------------------------------------------