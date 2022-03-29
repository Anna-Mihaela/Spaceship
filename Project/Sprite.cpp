#include "Sprite.h"
#include "SDL_Image.h"

Sprite::Sprite(std::string address, int totalRows, int totalColumns, int fps)
	: Image(address)
{
	m_TotalRows = totalRows;
	m_TotalColumns = totalColumns;
	m_FPS = fps;

	m_Size.x = float(m_Surface->w / totalColumns);
	m_Size.y = float(m_Surface->h / totalRows);
	m_UpdateRate = 1.0f / fps;

	m_CurrentRow = 0;
	m_CurrentColumn = 0;
	m_ElapsedTime = 0.0f;
}

Sprite::~Sprite()
{

}

void Sprite::Update(float deltaTime)
{
	m_ElapsedTime += deltaTime;

	if (m_ElapsedTime >= m_UpdateRate)
	{
		m_ElapsedTime -= m_UpdateRate;
		m_CurrentColumn++;

		if (m_CurrentColumn == m_TotalColumns)
		{
			m_CurrentColumn = 0;
			m_CurrentRow++;

			if (m_CurrentRow == m_TotalRows)
			{
				m_CurrentRow = 0;
			}
		}
	}
}

void Sprite::Draw(SDL_FPoint location, float angle, SDL_RendererFlip flip)
{
	SDL_Rect source;
	source.x = m_Size.x * m_CurrentColumn;
	source.y = m_Size.y * m_CurrentRow;
	source.w = m_Size.x;
	source.h = m_Size.y;

	SDL_FRect rectangle;
	rectangle.x = location.x;
	rectangle.y = location.y;
	rectangle.w = m_Size.x;
	rectangle.h = m_Size.y;

	SDL_RenderCopyExF(m_Renderer, m_Texture, &source, &rectangle, angle, nullptr, flip);
}
