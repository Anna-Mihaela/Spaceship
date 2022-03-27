#include "Main.h"

#include "GameInstance.h"
#include "SDL.h"
#include "Player.h"

int main(int argv, char* args[])
{
	GameInstance& gameInstance = GameInstance::GetInstance();
	SDL_Renderer* renderer = gameInstance.GetRenderer();
	float currentTime = 0.0f;
	float previousTime = 0.0f;
	float deltaTime = 0.0f;

	gameInstance.StartGame();

	while (gameInstance.GetIsRunning())
	{
		previousTime = currentTime;
		currentTime = SDL_GetTicks() / 1000.0f;
		deltaTime = currentTime - previousTime;

		SDL_RenderClear(renderer);

		gameInstance.Input();
		gameInstance.Update(deltaTime);
		gameInstance.Draw();

		SDL_RenderPresent(renderer);
	}

	return 0;
}

