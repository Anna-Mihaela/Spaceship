#pragma once
#include "vector"

union SDL_Event;
class Asteroid;
class Player;

class GameMode
{
public:
	GameMode();
	~GameMode();

	void Input(SDL_Event& Event);
	void Update(float deltaTime);
	void Draw();

private:
	Player* m_Player;
	std::vector<Asteroid*> m_Asteroids;
};

