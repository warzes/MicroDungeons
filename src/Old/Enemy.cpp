#include "stdafx.h"
#include "Enemy.h"
#include "oWorld.h"
#include "EnemyManager.h"
#include "oPlayer.h"
//-----------------------------------------------------------------------------
void Enemy::Init()
{
	m_enemyStartPos.x = static_cast<float>(rand() % 32);
	m_enemyStartPos.y = 0.3f;
	m_enemyStartPos.z = static_cast<float>(rand() % 32);

	m_enemyBoxPos.x = m_enemyStartPos.x;
	m_enemyBoxPos.y = m_enemyStartPos.y;
	m_enemyBoxPos.z = m_enemyStartPos.z;
	m_enemyBoxSize = { 0.4f, 0.6f, 0.4f };
	m_active = true;
}
//-----------------------------------------------------------------------------
void Enemy::Draw()
{
	DrawCube(m_enemyBoxPos, 0.4f, 0.6f, 0.4f, MAROON);
}
//-----------------------------------------------------------------------------
void Enemy::Update(const EnemyManager& enemyManager, const oPlayerCamera& playerCamera, const oWorld& world, float deltaTime)
{
	if (m_active == true)
	{
		// Follow player
		int isStepX = 0;
		int isStepZ = 0;
		float stepSize = 2.0f*deltaTime;
		if (playerCamera.GetPosition().x > m_enemyBoxPos.x)
		{
			m_enemyBoxPos.x += stepSize;
			if (testCollision(enemyManager, playerCamera, world)) m_enemyBoxPos.x -= stepSize;
			else isStepX = 1;
		}
		if (playerCamera.GetPosition().x < m_enemyBoxPos.x)
		{
			m_enemyBoxPos.x -= stepSize;
			if (testCollision(enemyManager, playerCamera, world)) m_enemyBoxPos.x += stepSize;
			else isStepX = -1;
		}
		if (playerCamera.GetPosition().z > m_enemyBoxPos.z)
		{
			m_enemyBoxPos.z += stepSize;
			if (testCollision(enemyManager, playerCamera, world)) m_enemyBoxPos.z -= stepSize;
			else isStepZ = 1;
		}
		if (playerCamera.GetPosition().z < m_enemyBoxPos.z)
		{
			m_enemyBoxPos.z -= stepSize;
			if (testCollision(enemyManager, playerCamera, world)) m_enemyBoxPos.z += stepSize;
			else isStepZ = -1;
		}

		//// Define Bounding Box for enemy
		struct BoundingBox cBounds = {
			{m_enemyBoxPos.x - 0.2f, 0.0f, m_enemyBoxPos.z - 0.2f},
			{m_enemyBoxPos.x + 0.2f, 0.6f, m_enemyBoxPos.z + 0.2f}
		};
		m_enemyBounds = cBounds;

		//// Attack player logic
		//// Player Bounding Box
		//struct BoundingBox player = {
		//	cam.position.x - 0.2f, cam.position.y - 0.0f, cam.position.z - 0.2f,
		//	cam.position.x + 0.2f, cam.position.y + 0.6f, cam.position.z + 0.2f
		//};
		//if (CheckCollisionBoxes(player, m_enemyBounds) && m_enemyBoxPos.y >= 1.0f)
		//{
		//	if (((framesCounter / 30) % 2) == 1)
		//	{
		//		//hp -= rand() % level + 1;
		//		framesCounter = 0;
		//	}
		//}
	}
	// Respawn
	else
	{
		m_enemyBoxPos.x = static_cast<float>(rand() % 32);
		m_enemyBoxPos.y = 0.3f;
		m_enemyBoxPos.z = static_cast<float>(rand() % 32);

		struct BoundingBox newBounds =
		{
			{m_enemyBoxPos.x - 0.2f, 0.0f, m_enemyBoxPos.z - 0.2f},
			{m_enemyBoxPos.x + 0.2f, 0.6f, m_enemyBoxPos.z + 0.2f}
		};

		m_enemyBounds = newBounds;
		m_active = true;
	}
}
//-----------------------------------------------------------------------------
bool Enemy::PlayerAttack()
{
	m_active = false;
	return true;
}
//-----------------------------------------------------------------------------
bool Enemy::testCollision(const EnemyManager& enemyManager, const oPlayerCamera& playerCamera, const oWorld& world) const
{
	struct BoundingBox cBounds = {
		{ m_enemyBoxPos.x - 0.2f, 0.0001f, m_enemyBoxPos.z - 0.2f },
		{ m_enemyBoxPos.x + 0.2f, 0.6f, m_enemyBoxPos.z + 0.2f }
	};

	if (world.TestCollision(cBounds)) return true;
	if (enemyManager.TestCollision(m_enemyBoxPos, cBounds)) return true;
	if (CheckCollisionBoxes(cBounds, playerCamera.GetBoundingBox())) return true;

	return false;
}
//-----------------------------------------------------------------------------