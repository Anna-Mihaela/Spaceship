#pragma once
#include <string>
#include "SDL_render.h"

class Image
{
public:
	Image(std::string address);
	~Image();
	
	virtual void Draw(SDL_FPoint location, float angle, SDL_RendererFlip flip = SDL_FLIP_NONE);

protected:
	SDL_Renderer* m_Renderer;
	SDL_Surface* m_Surface;
	SDL_Texture* m_Texture;
};

