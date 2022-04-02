#include "GameMode.h"
#include "cmath"
#include "Laser.h"
#include "Main.h"
#include "SDL_events.h"


GameMode::GameMode()
{
	m_Asteroids.resize(ASTEROID_COUNT);
}

GameMode::~GameMode()
{
	m_Asteroids.clear();

	for (Projectile* p : m_Projectiles)
	{
		delete p;
	}
	m_Projectiles.clear();
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

	for (int i = 0; i < m_Projectiles.size(); i++)
	{
		m_Projectiles[i]->Update(deltaTime);

		SDL_FPoint location = m_Projectiles[i]->GetLocation();
		SDL_FPoint size = m_Projectiles[i]->GetSize();
		if (location.x < -size.x || location.x > WINDOW_WIDTH
			|| location.y < -size.y || location.y > WINDOW_HEIGHT)
		{
			delete m_Projectiles[i];
			m_Projectiles.erase(m_Projectiles.begin() + i);
			i--;
		}
	}

	for (Asteroid& a : m_Asteroids)
	{
		for (int i = 0; i < m_Projectiles.size(); i++)
		{
			SDL_FPoint locA = a.GetLocation();
			SDL_FPoint locP = m_Projectiles[i]->GetLocation();
			SDL_FPoint delta(locA.x - locP.x, locA.y - locP.y);
			float distance = std::hypotf(delta.x, delta.y);

			SDL_FPoint sizeP = m_Projectiles[i]->GetSize();
			SDL_FPoint sizeA = a.GetSize();
			float radius = (sizeP.x + sizeP.y + sizeA.x + sizeA.y) / 5;

			if (distance < radius)
			{
				a.SetRandomValues();
				delete m_Projectiles[i];
				m_Projectiles.erase(m_Projectiles.begin() + i);
				i--;
			}
		}
	}
}

void GameMode::Draw()
{
	m_Player.Draw();

	for (Asteroid& a : m_Asteroids)
	{
		a.Draw();
	}

	for (Projectile* p : m_Projectiles)
	{
		p->Draw();
	}
}

void GameMode::SpawnLaser(SDL_FPoint location, float speed, float angle)
{
	m_Projectiles.push_back(new Laser(location, speed, angle));
}
