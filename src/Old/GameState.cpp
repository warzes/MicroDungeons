#include "stdafx.h"
#include "GameState.h"

GameState::~GameState()
{
}

void GameState::Start()
{
	OnStart();
}

void GameState::Frame()
{
	OnFrame();
}

void GameState::Update()
{
	OnUpdate();
}

void GameState::Stop()
{
	OnStop();
}