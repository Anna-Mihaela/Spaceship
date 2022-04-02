#include "Laser.h"
#include "cmath"
#include "Main.h"
#include "Sprite.h"

Laser::Laser(SDL_FPoint location, float speed, float angle)
{
	m_Sprite = new Sprite("Resources/Images/Player/Lasers.png", 5, 1, 20);
	m_Location = location;
	m_Speed = speed;
	m_Angle = angle;

	float angleRadians = m_Angle * (M_PI * 0.0055555f); // inverse of 180
	m_Direction.x = std::cos(angleRadians);
	m_Direction.y = std::sin(angleRadians);
}

void Laser::Update(float deltaTime)
{
	Projectile::Update(deltaTime);

	m_Location.x += m_Direction.x * m_Speed * deltaTime;
	m_Location.y += m_Direction.y * m_Speed * deltaTime;
}
