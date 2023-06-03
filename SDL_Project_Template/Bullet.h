#ifndef _BULLET_H_
#define _BULLET_H_

#include <SDL.h>
#include "GameObject.h"

class Bullet : public GameObject
{
public:
	Bullet() {}
	~Bullet () {}
	void load(std::string textureID, int x, int y, int width, int height, double scale);
	void draw(SDL_Renderer* renderer);
	void update();
private:

};

#endif