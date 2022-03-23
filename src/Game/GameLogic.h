#pragma once

#include "GameState.h"
#include "Player.h"
#include "PlayerAnimation.h"
#include "World.h"
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
	PlayerCamera m_playerCamera;
	PlayerAnimation m_playerAnimation;
	PlayerLogic m_playerLogic;
	World m_world;
	EnemyManager m_enemyManager;
	RenderObjectManager m_renderObjectManager;
};