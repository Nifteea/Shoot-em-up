#ifndef _TEXTUREMANAGER_H_
#define _TEXTUREMANAGER_H_
#include <iostream>
#include <SDL.h>
#include <map>
#include <string>

class TextureManager
{
private:
	TextureManager() {} //makes TextureManager a singleton class - only need one instance and can reuse class throughout the game
	~TextureManager() {}
	
	static TextureManager* instance;
public:
	std::map<std::string, SDL_Texture*> textureMap; //array of pointers to SDL_Texture objects, keyed using std::strings

	bool load(std::string fileName, std::string id, SDL_Renderer* renderer); //used to load a texture into memory and store it in the "textureMap array"
	void draw(std::string id, int x, int y, int width, int height, double scale, SDL_Renderer* renderer, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void drawFrame(std::string id, int x, int y, int width, int height, double scale, int currentRow, int currentFrame, SDL_Renderer* renderer, SDL_RendererFlip flip = SDL_FLIP_NONE); //for drawing spritesheets
	void clearFromTextureMap(std::string id);
	
	static TextureManager* Instance() //checks if there is already an instance of TextureManager. If not, then constructs it, otherwise returns the static instance.
	{
		if (instance == 0)
		{
			instance = new TextureManager();
			return instance;
		}
		return instance;
	}
};

typedef TextureManager theTextureManager;

#endif