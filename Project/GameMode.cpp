#include "GameMode.h"
#include "Asteroid.h"
#include "Main.h"
#include "Player.h"
#include "SDL_events.h"


GameMode::GameMode()
{
	m_Player = new Player;

	for (int i = 0; i < ASTEROID_COUNT; i++)
	{
		m_Asteroids.push_back(new Asteroid);
	}
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
	for (Asteroid* a : m_Asteroids)
	{
		a->Update(deltaTime);
	}
}

void GameMode::Draw()
{
	m_Player->Draw();
	for (Asteroid* a : m_Asteroids)
	{
		a->Draw();
	}
}
