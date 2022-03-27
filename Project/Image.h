#pragma once
#include <string>
#include "SDL_rect.h"

struct SDL_Renderer;
struct SDL_Surface;
struct SDL_Texture;

class Image
{
public:
	Image(std::string address);
	~Image();
	
	virtual void Draw(SDL_FPoint location, float angle);

protected:
	SDL_Renderer* m_Renderer;
	SDL_Surface* m_Surface;
	SDL_Texture* m_Texture;
};

