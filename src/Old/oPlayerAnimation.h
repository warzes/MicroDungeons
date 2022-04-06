#pragma once

class oPlayerAnimation
{
public:
	void Init();
	void Close();

	void Draw2D();
	void Update(float deltaTime);

	void StartAnimationAttacking();
	bool IsEndAnimationAttack();

private:
	Texture2D m_sword = {};
	Rectangle m_frameRec = {0.0f};
	Vector2 m_swordPosition = { 340.0f, 120.0f };
	Vector2 m_swordOffsetPosition = { 340.0f, 120.0f };
	bool m_attacking = false;
	bool m_isEndAnimationAttack = false;
	int m_currentFrame = 0;
	float m_attackCounter = 0.0f;
	float m_animationAttackSpeed = 50.0f;
};