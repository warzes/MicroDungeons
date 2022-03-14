#include "stdafx.h"
#include "Enemy.h"

void Enemy::Init()
{
	m_enemyStartPos.x = rand() % 32 - 16;
	//enemyStartPos.y = -1.0f;
	m_enemyStartPos.y = 0.5f;

	m_enemyStartPos.z = rand() % 32 - 16;
	m_enemyBoxPos.x = m_enemyStartPos.x;
	m_enemyBoxPos.y = m_enemyStartPos.y;
	m_enemyBoxPos.z = m_enemyStartPos.z;
	m_enemyBoxSize = { 0.8f, 1.0f, 0.8f };
	m_active = true;
}

void Enemy::Draw()
{
	DrawCube(m_enemyBoxPos, 1.0f, 1.0f, 1.0f, RED);
}

void Enemy::Update(const Camera& cam, int framesCounter)
{
	// Bounding boxes

	// Attack Range
	struct BoundingBox range = {
		cam.target.x - 0.2f, cam.target.y - 0.0f, cam.target.z - 0.2f,
		cam.target.x + 0.2f, cam.target.y + 1.0f, cam.target.z + 0.2f
	};

	//// Player Bounding Box
	struct BoundingBox player = {
		cam.position.x - 0.2f, cam.position.y - 0.0f, cam.position.z - 0.2f,
		cam.position.x + 0.2f, cam.position.y + 0.6f, cam.position.z + 0.2f
	};

	if (m_active == true)
	{
		if (m_enemyBoxPos.y >= 0.5f)
		{
			// Follow player
			if (cam.position.x > m_enemyBoxPos.x - 1.0f)
			{
				m_enemyBoxPos.x += 0.04f;
			}
			if (cam.position.x < m_enemyBoxPos.x + 1.0f)
			{
				m_enemyBoxPos.x -= 0.04f;
			}
			if (cam.position.z > m_enemyBoxPos.z - 1.0f)
			{
				m_enemyBoxPos.z += 0.04f;
			}
			if (cam.position.z < m_enemyBoxPos.z + 1.0f)
			{
				m_enemyBoxPos.z -= 0.04f;
			}

			// Define Bounding Box for enemy

			struct BoundingBox cBounds = {
				m_enemyBoxPos.x - 0.4f, 0.0f, m_enemyBoxPos.z - 0.4f,
				m_enemyBoxPos.x + 0.4f, 1.0f, m_enemyBoxPos.z + 0.4f
			};

			m_enemyBounds = cBounds;

		}
		else if (m_enemyBoxPos.y < 0.5f)
		{
			m_enemyBoxPos.y += 0.01f;
		}

		// Attack player logic

		if (CheckCollisionBoxes(player, m_enemyBounds) && m_enemyBoxPos.y >= 1.0f)
		{
			if (((framesCounter / 30) % 2) == 1)
			{
				//hp -= rand() % level + 1;
				framesCounter = 0;
			}
		}
	}
	// Respawn
	else
	{
		m_enemyBoxPos.x = rand() % 32 - 16;
		//enemyBoxPos.y = -1.5f;
		m_enemyBoxPos.y = 0.5f;
		m_enemyBoxPos.z = rand() % 32 - 16;

		struct BoundingBox newBounds =
		{
			m_enemyBoxPos.x - 0.4f,
			0.0f,
			m_enemyBoxPos.z - 0.4f,
			m_enemyBoxPos.x + 0.4f,
			1.0f,
			m_enemyBoxPos.z + 0.4f
		};

		m_enemyBounds = newBounds;
		m_active = true;
	}
}

bool Enemy::PlayerAttack(const Camera& cam)
{
	// Attack Range
	struct BoundingBox range = {
		cam.target.x - 0.2f, cam.target.y - 0.0f, cam.target.z - 0.2f,
		cam.target.x + 0.2f, cam.target.y + 1.0f, cam.target.z + 0.2f
	};

	//// Player Bounding Box
	struct BoundingBox player = {
		cam.position.x - 0.2f, cam.position.y - 0.0f, cam.position.z - 0.2f,
		cam.position.x + 0.2f, cam.position.y + 0.6f, cam.position.z + 0.2f
	};

	if (CheckCollisionBoxes(range, m_enemyBounds))
	{
		//score += 100;
		//level++;
		m_active = false;
		return true;
	}

	return false;
}