#pragma once
#include "Asteroid.h"
#include "Player.h"
#include "Projectile.h"
#include "vector"

union SDL_Event;

class GameMode
{
public:
	GameMode();
	~GameMode();

	void Input(SDL_Event& Event);
	void Update(float deltaTime);
	void Draw();

	void SpawnLaser(SDL_FPoint location, float speed, float angle);

private:
	Player m_Player;
	std::vector<Asteroid> m_Asteroids;
	std::vector<Projectile> m_Projectiles;
};

