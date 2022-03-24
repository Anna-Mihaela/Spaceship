#include "GameInstance.h"
#include "GameMode.h"
#include "SDL_Image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

GameInstance* GameInstance::m_Instance = nullptr;

GameInstance* GameInstance::GetInstance()
{
    if (m_Instance == nullptr) 
    {
        m_Instance = new GameInstance();
    }

    return m_Instance;
}

GameInstance::GameInstance()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    TTF_Init();

    m_Window = SDL_CreateWindow("Ana's Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 720, SDL_WINDOW_OPENGL);
	m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);
	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
	SDL_RenderSetLogicalSize(m_Renderer, 1024, 720);
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

void GameInstance::OnStart()
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

void GameInstance::Update(float DeltaTime)
{
    m_GameMode->Update(DeltaTime);
}

void GameInstance::Draw()
{
    m_GameMode->Draw();
}

void GameInstance::Quit()
{
	m_IsRunning = false;
}
