#include "Player.h"
#include "algorithm"
#include "cmath"
#include "GameInstance.h"
#include "SDL_image.h"
#include "Sprite.h"

#undef M_PI
#define M_PI       3.14159265358979323846f   // pi

Player::Player()
{
	m_Sprite = new Sprite("Resources/Images/Player/Spaceship.png", 2, 1, 20);
	m_Location.x = 0.0f;
	m_Location.y = 0.0f;
	m_Angle = -90.0f;
	m_MovementSpeed = 300.0f;
	m_RotationSpeed = 30.0f;

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
	case SDLK_q:
		m_IsRLeftPressed = isPressed;
		break;
	case SDLK_e:
		m_IsRRightPressed = isPressed;
		break;
	default:
		break;
	}
}

void Player::Update(float deltaTime)
{
	float rotation = 0.0f;
	if (m_IsRLeftPressed)
	{
		rotation = -1.0f;
	}
	if (m_IsRRightPressed)
	{
		rotation = 1.0f;
	}

	if (rotation != 0.0f)
	{
		m_Angle = rotation * m_RotationSpeed * deltaTime;
	}

	SDL_FPoint direction{ 0.0f, 0.0f };
	if (m_IsUpPressed)
	{
		direction.x += 1.0f;
	}
	if (m_IsDownPressed)
	{
		direction.x -= 1.0f;
	}
	if (m_IsLeftPressed)
	{
		direction.y -= 1.0f;
	}
	if (m_IsRightPressed)
	{
		direction.y += 1.0f;
	}

	if (direction.x != 0.0f || direction.y != 0.0f)
	{
		// x = 1, y = 1 -> x = 0.707, y = 0.707
		// Normalize direction
		const float scale = 1.0f / std::hypotf(direction.x, direction.y);
		direction.x *= scale;
		direction.y *= scale;

		// Rotate axis
		// Apply angle to direction
		float angleRadians = m_Angle * (M_PI / 180.0f);
		float cosAngle = std::cos(angleRadians);
		float sinAngle = std::sin(angleRadians);
		direction = SDL_FPoint(direction.x * cosAngle - direction.y * sinAngle, 
			direction.x * sinAngle + direction.y * cosAngle);

		// Apply movement
		float speed = m_MovementSpeed * deltaTime;
		m_Location.x += direction.x * speed;
		m_Location.y += direction.y * speed;

		// Limit movement to boundaries
		SDL_FPoint size = m_Sprite->GetSize();
		m_Location.x = std::clamp<float>(m_Location.x, 0, WINDOW_WIDTH - size.x);
		m_Location.y = std::clamp<float>(m_Location.y, 0, WINDOW_HEIGHT - size.y);
	}

	m_Sprite->Update(deltaTime);
}

void Player::Draw()
{
	m_Sprite->Draw(m_Location, m_Angle);
}