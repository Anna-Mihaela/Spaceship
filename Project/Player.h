#pragma once
#include "SDL_events.h"
#include "SDL_rect.h"

class Image;

class Player
{
public:
	Player();
	~Player();

	void Input(SDL_Event& Event);
	void Update(float deltaTime);
	void Draw();

private:
	Image* m_Image;
	SDL_FPoint m_Location;
	double m_Angle;
	float m_Speed;

	bool m_IsUpPressed;
	bool m_IsDownPressed;
	bool m_IsLeftPressed;
	bool m_IsRightPressed;
};

