#pragma once

#include "Player.h"
#include "World.h"

class Game
{
public:
	Game();

	void Frame();
	void Update();

	void Close();

private:
	PlayerCamera m_playerCamera;
	PlayerLogic m_playerLogic;
	World m_world;
};