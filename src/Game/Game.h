#pragma once

#include "Player.h"
#include "PlayerAnimation.h"
#include "World.h"
#include "Enemy.h"

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

#define ENEMIES 20
	Enemy m_enemies[ENEMIES];
};