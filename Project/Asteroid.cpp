#include "Asteroid.h"
#include "cmath"
#include "Main.h"
#include "Sprite.h"

Asteroid::Asteroid()
{
	m_Sprite = new Sprite("Resources/Images/Entity/Fireball.png", 4, 6, 20);
	SetRandomValues();
}

Asteroid::~Asteroid()
{
	delete m_Sprite;
}

void Asteroid::Update(float deltaTime)
{
	m_Sprite->Update(deltaTime);

	m_Location.x += m_Direction.x * m_Speed * deltaTime;
	m_Location.y += m_Direction.y * m_Speed * deltaTime;

	if (m_Location.x < 0 || m_Location.x > WINDOW_WIDTH || m_Location.y > WINDOW_HEIGHT)
	{
		SetRandomValues();
	}
}

void Asteroid::Draw()
{
	m_Sprite->Draw(m_Location, m_Angle, m_Flip);
}

void Asteroid::SetRandomValues()
{
	m_Speed = 100.0f + GetRandomFloat(100);
	m_Flip = SDL_RendererFlip(rand() % 3);
	m_Angle = GetRandomFloat(360);

	m_Location.x = GetRandomFloat(WINDOW_WIDTH);
	m_Location.y = -m_Sprite->GetSize().y;
	SDL_FPoint target{ GetRandomFloat(WINDOW_WIDTH), WINDOW_HEIGHT };

	SDL_FPoint delta{ target.x - m_Location.x, target.y - m_Location.y };
	const float scale = 1.0f / std::hypotf(delta.x, delta.y);
	m_Direction.x = delta.x * scale;
	m_Direction.y = delta.y * scale;
}

float Asteroid::GetRandomFloat(int n)
{
	return float(rand() % (n + 1));
}
