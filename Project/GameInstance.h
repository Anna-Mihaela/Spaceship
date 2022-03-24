#pragma once
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

	void OnStart();
	void Input();
	void Update(float DeltaTime);
	void Draw();

	static GameInstance* GetInstance();

	void SetIsRunning(bool isRunning) { m_IsRunning = isRunning; }
	bool GetIsRunning() const { return m_IsRunning; }

	SDL_Renderer* GetRenderer() const { return m_Renderer; }
	SDL_Window* GetWindow() const { return m_Window; }
	GameMode* GetGameMode() const { return m_GameMode; }

private:
	GameInstance();

	void Quit();

private:
	static GameInstance* m_Instance;

	bool m_IsRunning;
	SDL_Renderer* m_Renderer;
	SDL_Window* m_Window;
	GameMode* m_GameMode;
};