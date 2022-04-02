#pragma once
#include "Image.h"


class Sprite : public Image
{
public:
	Sprite(std::string address, int totalRows, int totalColumns, int fps);
	
	void Update(float deltaTime);
	virtual void Draw(SDL_FPoint location, float angle, SDL_RendererFlip flip = SDL_FLIP_NONE) override;
	SDL_FPoint GetSize() const { return m_Size; }

private:
	int m_TotalRows;
	int m_TotalColumns;
	int m_FPS;
	SDL_FPoint m_Size;
	float m_UpdateRate;

	int m_CurrentRow;
	int m_CurrentColumn;
	float m_ElapsedTime;
};

