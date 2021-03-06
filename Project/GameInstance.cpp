#include "GameInstance.h"
#include "GameMode.h"
#include "SDL_Image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

GameInstance& GameInstance::GetInstance()
{
	static GameInstance instance;
    return instance;
}

GameInstance::GameInstance()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	TTF_Init();

	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");

    m_Window = SDL_CreateWindow(GAME_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
	m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);
    m_IsRunning = false;
    m_GameMode = nullptr;
}

GameInstance::~GameInstance()
{
    delete m_GameMode;
    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window);
	TTF_Quit();
    Mix_Quit();
	IMG_Quit();
    SDL_Quit();
}

void GameInstance::StartGame()
{
	m_IsRunning = true;
    m_GameMode = new GameMode;
}

void GameInstance::Input()
{
    SDL_Event Event;
    while (SDL_PollEvent(&Event))
    {
        if (Event.type == SDL_QUIT)
        {
            Quit();
        }

        m_GameMode->Input(Event);
    }
}

void GameInstance::Update(float deltaTime)
{
    m_GameMode->Update(deltaTime);
}

void GameInstance::Draw()
{
    m_GameMode->Draw();
}

void GameInstance::Quit()
{
	m_IsRunning = false;
}
