#include "stdafx.h"
#include "EnemyManager.h"
//-----------------------------------------------------------------------------
void EnemyManager::Init()
{
	for (int i = 0; i < ENEMIES; i++)
	{
		m_enemies[i].Init();
	}
}
//-----------------------------------------------------------------------------
void EnemyManager::Close()
{
}
//-----------------------------------------------------------------------------
void EnemyManager::Draw()
{
	for (int i = 0; i < ENEMIES; i++)
	{
		//m_enemies[i].Draw();
	}
}
//-----------------------------------------------------------------------------
void EnemyManager::Update(const oPlayerCamera& playerCamera, const oWorld& world, float deltaTime)
{
	for (int i = 0; i < ENEMIES; i++)
	{
		//m_enemies[i].Update(*this, playerCamera, world, deltaTime);
	}
}
//-----------------------------------------------------------------------------
void EnemyManager::PlayerAttack(const Vector3& playerPosition, const Ray& ray)
{
	for (int i = 0; i < ENEMIES; i++)
	{
		const float distance = Vector3Distance(playerPosition, m_enemies[i].GetPosition());
		if (distance < 1.5)
		{
			const BoundingBox enemyBBox = m_enemies[i].GetBoundingBox();
			const RayCollision boxHitInfo = GetRayCollisionBox(ray, enemyBBox);
			if ((boxHitInfo.hit) && (boxHitInfo.distance < 1.5))
			{
				std::cout << "hit=" << boxHitInfo.distance << std::endl;
				m_enemies[i].PlayerAttack();
			}
			else
			{
				std::cout << "no hit" << std::endl;
			}
		}
	}
}
//-----------------------------------------------------------------------------
bool EnemyManager::TestCollision(const Vector3& position, const BoundingBox& bbox) const
{
	for (int i = 0; i < ENEMIES; i++)
	{
		const Vector3 targetPosition = m_enemies[i].GetPosition();
		const float distance = Vector3Distance(position, targetPosition);
		if (distance > 2.0f) continue;
		if (targetPosition.x == position.x && targetPosition.y == position.y && targetPosition.z == position.z)
			continue;

		const BoundingBox targetBBox = m_enemies[i].GetBoundingBox();
		if (CheckCollisionBoxes(bbox, targetBBox)) 
			return true;
	}

	return false;
}
//-----------------------------------------------------------------------------