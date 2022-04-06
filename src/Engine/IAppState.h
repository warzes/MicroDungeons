#pragma once

class IAppState
{
public:
	virtual ~IAppState() = default;

	virtual bool Start(Engine* engine) = 0;
	virtual void Stop(Engine* engine) = 0;

	virtual void Update(Engine* engine, float deltaTime) = 0;
	virtual void Frame(Engine* engine, float deltaTime) = 0;
};

class TestState : public IAppState
{
public:
	bool Start(Engine* engine) final { return true; };
	void Stop(Engine* engine) final {};
	void Update(Engine* engine, float deltaTime) final {};
	void Frame(Engine* engine, float deltaTime) final {};
};