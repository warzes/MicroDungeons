#pragma once

#include "GameState.h"
#include "oPlayer.h"
#include "oPlayerAnimation.h"
#include "oWorld.h"
#include "EnemyManager.h"
#include "RenderObjectManager.h"

class GameLogic final : public GameState
{
public:
	GameLogic();

	void OnStart() final;
	void OnFrame() final;
	void OnUpdate() final;
	void OnStop() final;

private:
	oPlayerCamera m_playerCamera;
	oPlayerAnimation m_playerAnimation;
	PlayerLogic m_playerLogic;
	oWorld m_world;
	EnemyManager m_enemyManager;
	RenderObjectManager m_renderObjectManager;
};