#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "Enemy.h"
//-----------------------------------------------------------------------------
//rlFPCamera cam;
static Model wallModel;
static Vector2 posModel = { 3.0f, 3.0f };
static int framesCounter = 0;
//-----------------------------------------------------------------------------
Game::Game()
	: m_playerLogic(m_playerCamera)
{
	//cam.Setup(45, Vector3{ 1, 0, 0 });
	//cam.MoveSpeed.z = 10;
	//cam.MoveSpeed.x = 5;
	//cam.FarPlane = 5000;

	m_playerCamera.Init({ 5.0f, 0.0f, 10.0f }, 180, 90);
	m_playerAnimation.Init();
	m_world.Init();	

	wallModel = LoadModelFromMesh(GenMeshCube(0.5f, 0.5f, 0.5f));

	for (int i = 0; i < ENEMIES; i++)
	{
		m_enemies[i].Init();
	}
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

		DrawModel(wallModel, { posModel.x, 0.5f, posModel.y }, 1.0f, BLUE);

		for (int i = 0; i < ENEMIES; i++)
		{
			m_enemies[i].Draw();
		}
	}
	EndMode3D();
	//cam.EndMode3D();
	m_playerAnimation.Draw2D();

	DrawFPS(0, 0);
	EndDrawing();
}
//-----------------------------------------------------------------------------
void Game::Update()
{
	framesCounter++;

	//cam.Update();
	m_playerCamera.Update(m_world, GetFrameTime());
	
	// Enemy logic
	for (int i = 0; i < ENEMIES; i++)
	{
		m_enemies[i].Update(m_playerCamera.GetCamera(), framesCounter);
	}

	// Attack logic
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		m_playerAnimation.StartAttacking();
	}
	m_playerAnimation.Update(m_playerCamera.GetCamera());

	// Attack enemy logic

	if (m_playerAnimation.IsAttack())
	{
		for (int i = 0; i < ENEMIES; i++)
		{
			m_enemies[i].PlayerAttack(m_playerCamera.GetCamera());
		}
	}
}
//-----------------------------------------------------------------------------
void Game::Close()
{
	m_playerAnimation.Close();
	m_world.Close();
}
//-----------------------------------------------------------------------------