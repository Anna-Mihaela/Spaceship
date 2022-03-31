#include "Laser.h"
#include "cmath"
#include "Sprite.h"

Laser::Laser(SDL_FPoint location, float speed, float angle)
{
	m_Sprite = new Sprite("Resources/Images/Player/Lasers.png", 5, 1, 20);
	m_Location = location;
	m_Speed = speed;
	m_Angle = angle;

	float angleRadians = m_Angle * (M_PI * 0.0055555f); // inverse of 180
	float cosAngle = std::cos(angleRadians);
	float sinAngle = std::sin(angleRadians);
	m_Direction.x = cosAngle - sinAngle;
	m_Direction.y = sinAngle + cosAngle;
}

Laser::~Laser()
{
	
}

void Laser::Update(float deltaTime)
{
	Projectile::Update(deltaTime);

	m_Location.x += m_Direction.x * m_Speed * deltaTime;
	m_Location.y += m_Direction.y * m_Speed * deltaTime;
}
