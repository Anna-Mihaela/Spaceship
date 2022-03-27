#include "Image.h"
#include "GameInstance.h"
#include "SDL_Image.h"

Image::Image(std::string address)
{
	GameInstance& gameInstance = GameInstance::GetInstance();
	m_Renderer = gameInstance.GetRenderer();

	m_Surface = IMG_Load(address.c_str());
	m_Texture = SDL_CreateTextureFromSurface(m_Renderer, m_Surface);
}

Image::~Image()
{
	SDL_DestroyTexture(m_Texture);
	SDL_FreeSurface(m_Surface);
}

void Image::Draw(SDL_FPoint location, float angle)
{
	SDL_FRect rectangle;
	rectangle.x = location.x;
	rectangle.y = location.y;
	rectangle.h = (float)m_Surface->h;
	rectangle.w = (float)m_Surface->w;

	SDL_RenderCopyExF(m_Renderer, m_Texture, nullptr, &rectangle, angle, nullptr, SDL_FLIP_NONE);
}
