#include "GameMode.h"
#include "Laser.h"
#include "Main.h"
#include "SDL_events.h"


GameMode::GameMode()
{
	for (int i = 0; i < ASTEROID_COUNT; i++)
	{
		m_Asteroids.push_back(Asteroid());
	}
}

GameMode::~GameMode()
{
}

void GameMode::Input(SDL_Event& Event)
{
	m_Player.Input(Event);
}

void GameMode::Update(float deltaTime)
{
	m_Player.Update(deltaTime);

	for (Asteroid& a : m_Asteroids)
	{
		a.Update(deltaTime);
	}

	for (Projectile& a : m_Projectiles)
	{
		a.Update(deltaTime);
	}
}

void GameMode::Draw()
{
	m_Player.Draw();

	for (Asteroid& a : m_Asteroids)
	{
		a.Draw();
	}

	for (Projectile& a : m_Projectiles)
	{
		a.Draw();
	}
}

void GameMode::SpawnLaser(SDL_FPoint location, float speed, float angle)
{
	m_Projectiles.push_back(Laser(location, speed, angle));
}
