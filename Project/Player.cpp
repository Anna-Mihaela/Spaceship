#include "Player.h"
#include "cmath"
#include "GameInstance.h"
#include "SDL_image.h"
#include "Image.h"

Player::Player()
{
	m_Image = new Image("Resources/Images/Player/Spaceship.png");
	m_Location.x = 0.0f;
	m_Location.y = 0.0f;
	m_Angle = -75.0f;
	m_Speed = 300.0f;

	m_IsUpPressed = false;
	m_IsDownPressed = false;
	m_IsLeftPressed = false;
	m_IsRightPressed = false;
}

Player::~Player()
{
}

void Player::Input(SDL_Event& Event)
{
	if (Event.type != SDL_KEYDOWN && Event.type != SDL_KEYUP)
	{
		return;
	}

	bool isPressed = Event.type == SDL_KEYDOWN;
	switch (Event.key.keysym.sym)
	{
	case SDLK_w:
		m_IsUpPressed = isPressed;
		break;
	case SDLK_s:
		m_IsDownPressed = isPressed;
		break;
	case SDLK_a:
		m_IsLeftPressed = isPressed;
		break;
	case SDLK_d:
		m_IsRightPressed = isPressed;
		break;
	default:
		break;
	}
}

void Player::Update(float deltaTime)
{
	if (m_IsUpPressed)
	{
		m_Location.x += sin(m_Angle) * m_Speed * deltaTime;
		m_Location.y -= cos(m_Angle) * m_Speed * deltaTime;
	}
	if (m_IsDownPressed)
	{
		m_Location.x -= sin(m_Angle) * m_Speed * deltaTime;
		m_Location.y += cos(m_Angle) * m_Speed * deltaTime;
	}
	if (m_IsLeftPressed)
	{
		m_Location.x -= cos(m_Angle) * m_Speed * deltaTime;
		m_Location.y -= sin(m_Angle) * m_Speed * deltaTime;
	}
	if (m_IsRightPressed)
	{
		m_Location.x += cos(m_Angle) * m_Speed * deltaTime;
		m_Location.y += sin(m_Angle) * m_Speed * deltaTime;
	}

}

void Player::Draw()
{
	m_Image->Draw(m_Location, m_Angle);
}