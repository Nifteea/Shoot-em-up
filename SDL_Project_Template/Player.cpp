#include "Player.h"

void Player::load(std::string textureID, int x, int y, int width, int height, double scale)
{
	GameObject::load(textureID, x, y, width, height, scale);
}

void Player::draw(SDL_Renderer* renderer)
{
	GameObject::draw(renderer);
}

void Player::update()
{
	//get keyboard
	const Uint8* key = SDL_GetKeyboardState(NULL);

	//to move the player
	if (key[SDL_SCANCODE_RIGHT])
	{
		position.x += 5;
	}
	if (key[SDL_SCANCODE_LEFT])
	{
		position.x -= 5;
	}
	if (key[SDL_SCANCODE_UP])
	{
		position.y -= 5;
	}
	if (key[SDL_SCANCODE_DOWN])
	{
		position.y += 5;
	}

	//prevent player from going out of bounds (off screen)
	if (position.x < -15)
	{
		position.x = -15;
	}
	if (position.x > 615)
	{
		position.x = 615;
	}
	if (position.y < -25)
	{
		position.y = -25;
	}
	if (position.y > 445)
	{
		position.y = 445;
	}

	currentFrame = int((SDL_GetTicks() / 100) % 9);
	//finds out the amount of milliseconds since SDL was initialised
	//then divide this by the amount of time (in ms) we want between frames and then use % to keep in range of the amout of frames in the sprite sheet
	//so this code (every 100 ms) shifts the value of source rectangle by 128 pixels (the width of a frame), multiplied by the current frame we want, giving the correct position
	currentRow = int((SDL_GetTicks() / 100) % 8);
}
