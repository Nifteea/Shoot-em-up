#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "GameObject.h"

class Player : public GameObject
{
public:
	void load(std::string textureID, int x, int y, int width, int height, double scale);
	void draw(SDL_Renderer* renderer);
	void update();
	void clean();
};

#endif