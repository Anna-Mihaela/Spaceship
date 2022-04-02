#pragma once
#include "SDL_rect.h"

#undef M_PI
#define M_PI       3.14159265358979323846f   // pi

#define GAME_NAME		"Spaceship"
#define WINDOW_WIDTH	1024
#define WINDOW_HEIGHT	720
#define ASTEROID_COUNT	6

const SDL_FPoint WINDOW_CENTER{ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f };