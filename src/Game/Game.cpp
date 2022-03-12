#include "stdafx.h"
#include "Game.h"
#include "Player.h"
//-----------------------------------------------------------------------------
//rlFPCamera cam;
//-----------------------------------------------------------------------------
Game::Game()
{
	//cam.Setup(45, Vector3{ 1, 0, 0 });
	//cam.MoveSpeed.z = 10;
	//cam.MoveSpeed.x = 5;
	//cam.FarPlane = 5000;

	m_playerCamera.Init({ 5.0f, 0.0f, 0.0f }, 180, 90);
	m_world.Init();
}
//-----------------------------------------------------------------------------
void Game::Frame()
{
	BeginDrawing();
	//ClearBackground({ 140, 210, 240 });
	ClearBackground({ 50, 100, 200 });

	//cam.BeginMode3D();
	BeginMode3D(m_playerCamera.GetCamera());
	{
		//DrawPlane(Vector3{ 0, 0, 0 }, Vector2{ 50, 50 }, BEIGE);
		m_world.Frame();
	}
	EndMode3D();
	//cam.EndMode3D();

	DrawFPS(0, 0);
	EndDrawing();
}
//-----------------------------------------------------------------------------
void Game::Update()
{
	//cam.Update();
	m_playerCamera.Update(m_world, GetFrameTime());
}
//-----------------------------------------------------------------------------
void Game::Close()
{
	m_world.Close();
}
//-----------------------------------------------------------------------------