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
	World m_world;
};