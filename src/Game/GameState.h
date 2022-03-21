#pragma once

class GameState
{
public:
	virtual ~GameState();

private:
	std::queue<GameState> m_childs;
};