#pragma once

#include "GameState.h"
#include "GameLogic.h"

class oGame
{
public:
	oGame();

	void Frame();
	void Update();

	void Close();

private:
	//GameState m_rootState; // TODO:
	GameLogic m_logic;
};