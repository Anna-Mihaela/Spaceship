#pragma once
#include "SDL_render.h"
#include "Sprite.h"

class Asteroid
{
public:
	Asteroid();
	~Asteroid();

	void Update(float deltaTime);
	void Draw();

	void SetRandomValues();
	SDL_FPoint GetLocation() const { return m_Location; }
	SDL_FPoint GetSize() const { return m_Sprite->GetSize(); }

protected:
	float GetRandomFloat(int n);

private:
	Sprite* m_Sprite;
	SDL_FPoint m_Location;
	SDL_FPoint m_Direction;
	SDL_RendererFlip m_Flip;
	float m_Angle;
	float m_Speed;
};

