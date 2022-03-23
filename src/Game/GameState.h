#pragma once

class GameState
{
public:
	virtual ~GameState();

	void Start();
	void Frame();
	void Update();
	void Stop();

	virtual void OnStart() {}
	virtual void OnFrame() {}
	virtual void OnUpdate() {}
	virtual void OnStop() {}

	void SetPause(bool pause) { m_pause = pause; }

private:
	std::queue<GameState*> m_childs;
	bool m_pause = true;
};