#include "Projectile.h"
#include "Main.h"
#include "Sprite.h"

Projectile::Projectile()
{
	m_Sprite = nullptr;
	m_Location = { 0.0f, 0.0f };
	m_Direction = { 0.0f, 0.0f };
	m_Speed = 0.0f;
	m_Angle = 0.0f;
}

Projectile::~Projectile()
{
	delete m_Sprite;
}

void Projectile::Update(float deltaTime)
{
	if (m_Sprite != nullptr)
	{
		m_Sprite->Update(deltaTime);
	}

// 	if (m_Location.x < 0 || m_Location.x > WINDOW_WIDTH 
// 		|| m_Location.y < 0 || m_Location.y > WINDOW_HEIGHT)
// 	{
// 		delete this;
// 	}
}

void Projectile::Draw()
{
	if (m_Sprite != nullptr)
	{
		m_Sprite->Draw(m_Location, m_Angle);
	}
}
