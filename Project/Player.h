#pragma once
#include "SDL_events.h"
#include "SDL_rect.h"

class Sprite;

class Player
{
public:
	Player();
	~Player();

	void Input(SDL_Event& Event);
	void Update(float deltaTime);
	void Draw();

private:
	Sprite* m_Sprite;
	SDL_FPoint m_Location;
	float m_Angle;
	float m_MovementSpeed;
	float m_RotationSpeed;

	bool m_IsUpPressed;
	bool m_IsDownPressed;
	bool m_IsLeftPressed;
	bool m_IsRightPressed;

	bool m_IsRLeftPressed;
	bool m_IsRRightPressed;

	bool m_IsShootPressed;
};

