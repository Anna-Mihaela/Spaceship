#pragma once
#include "Main.h"
#include <memory>
#include <string>

class GameMode;
struct SDL_Renderer;
struct SDL_Window;

class GameInstance
{
public:
	~GameInstance();
	GameInstance(const GameInstance& A) = delete;
	GameInstance operator=(const GameInstance& A) = delete;

	void StartGame();
	void Input();
	void Update(float deltaTime);
	void Draw();

	static GameInstance& GetInstance();

	void SetIsRunning(bool isRunning) { m_IsRunning = isRunning; }
	bool GetIsRunning() const { return m_IsRunning; }

	SDL_Renderer* GetRenderer() const { return m_Renderer; }
	GameMode* GetGameMode() const { return m_GameMode; }

private:
	GameInstance();

	void Quit();

private:
	bool m_IsRunning;
	SDL_Window* m_Window;
	SDL_Renderer* m_Renderer;
	GameMode* m_GameMode;
};