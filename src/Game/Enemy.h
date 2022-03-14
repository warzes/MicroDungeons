#pragma once

class Enemy
{
public:
	void Init();
	void Draw();
	void Update(const Camera& cam, int framesCounter);

	bool PlayerAttack(const Camera& cam);

private:
	Vector3 m_enemyStartPos;
	Vector3 m_enemyBoxPos;
	Vector3 m_enemyBoxSize;
	BoundingBox m_enemyBounds;
	bool m_active;
};