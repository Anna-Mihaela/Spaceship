#include "GameMode.h"
#include "SDL_events.h"
#include "Player.h"

GameMode::GameMode()
{
	m_Player = new Player;
}

GameMode::~GameMode()
{
	delete m_Player;
}

void GameMode::Input(SDL_Event& Event)
{
	m_Player->Input(Event);
}

void GameMode::Update(float deltaTime)
{
	m_Player->Update(deltaTime);
}

void GameMode::Draw()
{
	m_Player->Draw();
}
