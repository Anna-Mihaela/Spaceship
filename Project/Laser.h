#pragma once
#include "Projectile.h"

class Laser : public Projectile
{
public:
	Laser(SDL_FPoint location, float speed, float angle);

	virtual void Update(float deltaTime) override;
};

