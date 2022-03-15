#pragma once

#include "Enemy.h"

class EnemyManager
{
public:
	void Init();
	void Close();
	void Draw();
	void Update(const PlayerCamera& playerCamera, const World& world, float deltaTime);

	void PlayerAttack(const Vector3& playerPosition, const Ray& ray);

	bool TestCollision(const Vector3& position, const BoundingBox& bbox) const;

private:
#define ENEMIES 10
	Enemy m_enemies[ENEMIES];
};