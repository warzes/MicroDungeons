#pragma once

class oWorld;
class EnemyManager;
class oPlayerCamera;

class Enemy
{
public:
	void Init();
	void Draw();
	void Update(const EnemyManager& enemyManager, const oPlayerCamera& playerCamera, const oWorld& world, float deltaTime);

	bool PlayerAttack();

	Vector3 GetPosition() const { return m_enemyBoxPos; }
	BoundingBox GetBoundingBox() const { return m_enemyBounds; }

private:
	bool testCollision(const EnemyManager& enemyManager, const oPlayerCamera& playerCamera, const oWorld& world) const;

	Vector3 m_enemyStartPos;
	Vector3 m_enemyBoxPos;
	Vector3 m_enemyBoxSize;
	BoundingBox m_enemyBounds;
	bool m_active;
};