#pragma once

#include "Player.h"
#include "PlayerAnimation.h"
#include "World.h"
#include "EnemyManager.h"
#include "GameState.h"

class Game
{
public:
	Game();

	void Frame();
	void Update();

	void Close();

private:
	PlayerCamera m_playerCamera;
	PlayerAnimation m_playerAnimation;
	PlayerLogic m_playerLogic;
	World m_world;
	EnemyManager m_enemyManager;
};