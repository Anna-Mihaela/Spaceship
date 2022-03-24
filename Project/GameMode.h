#pragma once

union SDL_Event;
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
};

