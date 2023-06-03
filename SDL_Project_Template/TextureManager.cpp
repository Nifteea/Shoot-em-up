#include <iostream>
#include "TextureManager.h"

TextureManager* TextureManager::instance = 0;

bool TextureManager::load(std::string fileName, std::string id, SDL_Renderer* renderer)
{
	SDL_Surface* tempSurface = SDL_LoadBMP(fileName.c_str()); //temporarily stores image data
	if (tempSurface == 0)
	{
		return false;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface); //SDL_Texture is now ready to be drawn to the screen

	SDL_FreeSurface(tempSurface);

	//if loading the texture went ok, add the texture to list
	if (texture != 0)
	{
		textureMap[id] = texture;
		return true;
	}
	return false; //something went wrong if reach here
}

void TextureManager::draw(std::string id, int x, int y, int width, int height, double scale, SDL_Renderer* renderer, SDL_RendererFlip flip) //images are not flipped by default
{
	SDL_Rect sourceRectangle;
	SDL_Rect destinationRectangle;
	sourceRectangle.x = 0;
	sourceRectangle.y = 0;
	destinationRectangle.x = x;
	destinationRectangle.y = y;
	destinationRectangle.w = sourceRectangle.w = width;
	destinationRectangle.h = sourceRectangle.h = height;
	destinationRectangle.w *= scale;
	destinationRectangle.h *= scale;
	SDL_RenderCopyEx(renderer, textureMap[id], &sourceRectangle, &destinationRectangle, 0, 0, flip);
}

void TextureManager::drawFrame(std::string id, int x, int y, int width, int height, double scale, int currentRow, int currentFrame, SDL_Renderer* renderer, SDL_RendererFlip flip)
{
	SDL_Rect sourceRectangle;
	SDL_Rect destinationRectangle;
	sourceRectangle.x = width * currentFrame;
	sourceRectangle.y = height * currentRow;
	sourceRectangle.w = destinationRectangle.w = width;
	sourceRectangle.h = destinationRectangle.h = height;
	destinationRectangle.x = x;
	destinationRectangle.y = y;
	destinationRectangle.w *= scale;
	destinationRectangle.h *= scale;
	SDL_RenderCopyEx(renderer, textureMap[id], &sourceRectangle, &destinationRectangle, 0, 0, flip);
}

void TextureManager::clearFromTextureMap(std::string id)
{
	textureMap.erase(id);
}

