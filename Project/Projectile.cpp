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
}

void Projectile::Draw()
{
	if (m_Sprite != nullptr)
	{
		m_Sprite->Draw(m_Location, m_Angle);
	}
}

SDL_FPoint Projectile::GetLocation() const
{
	return m_Location;
}

SDL_FPoint Projectile::GetSize() const
{
	return m_Sprite->GetSize();
}
