#include "Enemy.h"

void Enemy::load(std::string textureID, int x, int y, int width, int height, double scale)
{
	GameObject::load(textureID, x, y, width, height, scale);
}

void Enemy::draw(SDL_Renderer* renderer)
{
	GameObject::draw(renderer);
}

void Enemy::update()
{
	if (alive == true)
	{
		if (position.x > -15) //if enemy is not off the screen
		{
			position.x -= 3;
		}
		else
		{
			alive = false;
		}
	}

	currentFrame = int((SDL_GetTicks() / 100) % 9);
	currentRow = int((SDL_GetTicks() / 100) % 8);
}