#include "Bullet.h"

void Bullet::load(std::string textureID, int x, int y, int width, int height, double scale)
{
	GameObject::load(textureID, x, y, width, height, scale);
}

void Bullet::update()
{
	if (alive == true)
	{
		if (position.x < 615) //if bullet is not off the screen
		{
			position.x += 5;
		}
		else
		{
			alive = false;
		}
	}
}

void Bullet::draw(SDL_Renderer* renderer)
{
	GameObject::draw(renderer);
}