#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "Enemy.h"
//-----------------------------------------------------------------------------
//rlFPCamera cam;
static Model wallModel;
static Vector2 posModel = { 3.0f, 3.0f };
//-----------------------------------------------------------------------------
Game::Game()
	: m_playerLogic(m_playerCamera)
{
	//cam.Setup(45, Vector3{ 0, 10, 0 });
	//cam.MoveSpeed.z = 10;
	//cam.MoveSpeed.x = 5;
	//cam.FarPlane = 5000;

	m_playerCamera.Init({ 5.0f, 0.0f, 10.0f }, 180, 90);
	m_playerAnimation.Init();
	m_world.Init();	

	wallModel = LoadModelFromMesh(GenMeshCube(0.5f, 0.5f, 0.5f));

	m_enemyManager.Init();
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

		m_enemyManager.Draw();
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
	/*auto pos = m_playerCamera.GetPosition();
	pos.y = 10;
	cam.SetCameraPosition(pos);
	cam.Update();*/

	// Player logic
	m_playerCamera.Update(m_world, m_enemyManager, GetFrameTime());
	m_playerAnimation.Update(GetFrameTime());

	// Enemy logic
	m_enemyManager.Update(m_playerCamera, m_world, GetFrameTime());

	// Player Attack Events
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		m_playerAnimation.StartAnimationAttacking();
	}

	// Attack logic
	if (m_playerAnimation.IsEndAnimationAttack())
	{
		const Vector2 rayMousePos = { static_cast<float>(GetScreenWidth() / 2), static_cast<float>(GetScreenHeight() / 2) };
		const Ray ray = GetMouseRay(rayMousePos, m_playerCamera.GetCamera());
		m_enemyManager.PlayerAttack(m_playerCamera.GetCamera().position, ray);
	}
}
//-----------------------------------------------------------------------------
void Game::Close()
{
	m_enemyManager.Close();
	m_playerAnimation.Close();
	m_world.Close();
}
//-----------------------------------------------------------------------------