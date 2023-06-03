#include "GameObject.h"
#include "TextureManager.h"

void GameObject::load(std::string _textureID, int _x, int _y, int _width, int _height, double _scale)
{
	position.x = _x;
	position.y = _y;
	position.w = _width;
	position.h = _height;
	scale = _scale;
	textureID = _textureID;

	currentRow = 1;
	currentFrame = 1;
}

void GameObject::drawFrame(SDL_Renderer* renderer)
{
	//grabs needed texture from TextureManager using textureID and draws it according to the set values.
	theTextureManager::Instance()->drawFrame(textureID, position.x, position.y, position.w, position.h, scale, currentRow, currentFrame, renderer);
}

void GameObject::draw(SDL_Renderer* renderer)
{
	//grabs needed texture from TextureManager using textureID and draws it according to the set values.
	theTextureManager::Instance()->draw(textureID, position.x, position.y, position.w, position.h, scale, renderer);
}

void GameObject::getPosition()
{

}

void GameObject::update()
{

}
