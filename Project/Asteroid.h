#pragma once
#include "SDL_render.h"

class Sprite;

class Asteroid
{
public:
	Asteroid();
	~Asteroid();

	void Update(float deltaTime);
	void Draw();

protected:
	void SetRandomValues();

private:
	Sprite* m_Sprite;
	SDL_FPoint m_Location;
	SDL_FPoint m_Direction;
	SDL_RendererFlip m_Flip;
	float m_Angle;
	float m_Speed;
};

