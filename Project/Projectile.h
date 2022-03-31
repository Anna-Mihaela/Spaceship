#pragma once
#include "SDL_rect.h"

class Sprite;

class Projectile
{
public:
	Projectile();
	~Projectile();

	virtual void Update(float deltaTime);
	void Draw();

protected:
	Sprite* m_Sprite;
	SDL_FPoint m_Location;
	SDL_FPoint m_Direction;
	float m_Speed;
	float m_Angle;
};

