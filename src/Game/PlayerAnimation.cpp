#include "stdafx.h"
#include "PlayerAnimation.h"
//-----------------------------------------------------------------------------
#define NUM_FRAMES_PER_LINE 4
//-----------------------------------------------------------------------------
void PlayerAnimation::Init()
{
	m_sword = LoadTexture("../data/textures/sword.png");
	m_frameRec.x = 0.0f;
	m_frameRec.y = 0.0f;
	m_frameRec.width = (float)m_sword.width / 4;
	m_frameRec.height = (float)m_sword.height / 2;
}
//-----------------------------------------------------------------------------
void PlayerAnimation::Close()
{
	UnloadTexture(m_sword);
}
//-----------------------------------------------------------------------------
void PlayerAnimation::Draw2D()
{
	DrawTextureRec(m_sword, m_frameRec, m_swordPosition, WHITE);
}
//-----------------------------------------------------------------------------
void PlayerAnimation::Update(float deltaTime)
{
	m_swordOffsetPosition = { GetScreenWidth() - 800.0f, GetScreenHeight() - 430.0f };
	m_swordPosition.y = 50.0f + m_swordOffsetPosition.x;
	m_swordPosition.x = 340.0f + m_swordOffsetPosition.y;

	if (m_attacking)
	{
		m_frameRec.y = 400.0f;
		m_frameRec.x = 400.0f * m_currentFrame;
		m_swordPosition.x = 240.0f + m_swordOffsetPosition.x;
		m_swordPosition.y = 145.0f + m_swordOffsetPosition.y;
		m_attackCounter += m_animationAttackSpeed*deltaTime;
		if (m_attackCounter > 3.0f)
		{
			m_currentFrame++;
			if (m_currentFrame >= NUM_FRAMES_PER_LINE)
			{
				m_currentFrame = 0;
				m_attacking = false;
				m_isEndAnimationAttack = true;
			}
			m_attackCounter = 0.0f;
		}
	}
	else
	{
		m_frameRec.y = 0.0f;
		m_frameRec.x = 0.0f;
	}
}
//-----------------------------------------------------------------------------
bool PlayerAnimation::IsEndAnimationAttack()
{
	bool ret = m_isEndAnimationAttack;
	m_isEndAnimationAttack = false;
	return ret;
}
//-----------------------------------------------------------------------------
void PlayerAnimation::StartAnimationAttacking()
{
	m_attacking = true;
}
//-----------------------------------------------------------------------------