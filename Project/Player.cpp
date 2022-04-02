#include "Player.h"
#include "algorithm"
#include "cmath"
#include "GameInstance.h"
#include "GameMode.h"
#include "Laser.h"
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
	m_RotationSpeed = 45.0f;
	m_FireRate = 0.5f;
	m_FireDelay = 0.0f;

	m_IsUpPressed = false;
	m_IsDownPressed = false;
	m_IsLeftPressed = false;
	m_IsRightPressed = false;
	m_IsRRightPressed = false;
	m_IsRLeftPressed = false;
	m_IsShootPressed = false;
}

Player::~Player()
{
	delete m_Sprite;
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
	case SDLK_SPACE:
		m_IsShootPressed = isPressed;
		break; 
	default:
		break;
	}
}

void Player::Update(float deltaTime)
{
	float rotationInput = 0.0f;
	if (m_IsRLeftPressed)
	{
		rotationInput = -1.0f;
	}
	if (m_IsRRightPressed)
	{
		rotationInput = 1.0f;
	}

	SDL_FPoint movementInput{ 0.0f, 0.0f };
	if (m_IsUpPressed)
	{
		movementInput.x += 1.0f;
	}
	if (m_IsDownPressed)
	{
		movementInput.x -= 1.0f;
	}
	if (m_IsLeftPressed)
	{
		movementInput.y -= 1.0f;
	}
	if (m_IsRightPressed)
	{
		movementInput.y += 1.0f;
	}

	// Fire
	if (m_FireDelay > 0.0f)
	{
		m_FireDelay -= deltaTime;
	}

	if (m_IsShootPressed && m_FireDelay <= 0.0f)
	{
		GameInstance& gameInstance = GameInstance::GetInstance();
		GameMode* gameMode = gameInstance.GetGameMode();

		gameMode->SpawnLaser(m_Location, 500.0f, m_Angle);
		m_FireDelay = m_FireRate;
	}

	if (rotationInput != 0.0f)
	{
		m_Angle += rotationInput * m_RotationSpeed * deltaTime;
	}

	if (movementInput.x != 0.0f || movementInput.y != 0.0f)
	{
		// x = 1, y = 1 -> x = 0.707, y = 0.707
		// Normalize direction
		const float scale = 1.0f / std::hypotf(movementInput.x, movementInput.y);
		movementInput.x *= scale;
		movementInput.y *= scale;

		// Rotate axis
		// Apply angle to direction
		float angleRadians = m_Angle * (M_PI * 0.0055555f);  // inverse of 180
		float cosAngle = std::cos(angleRadians);
		float sinAngle = std::sin(angleRadians);
		SDL_FPoint direction;
		direction.x = movementInput.x * cosAngle - movementInput.y * sinAngle;
		direction.y = movementInput.x * sinAngle + movementInput.y * cosAngle;

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