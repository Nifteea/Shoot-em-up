#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_
#include <iostream>
#include <SDL.h>

class GameObject
{
public:
	bool alive;
	SDL_Rect position;
	double scale;

	void load(std::string _textureID, int _x, int _y, int _width, int _height, double _scale);
	void drawFrame(SDL_Renderer* renderer);
	void draw(SDL_Renderer * renderer);
	void getPosition();
	void update();
	void clean();
	int getX() { return this->position.x; }
	int getY() { return this->position.y; }
	int getW() { return this->position.w * scale; }
	int getH() { return this->position.h * scale; }
	int setX(int _x) { this->position.x = _x; }
	int setY(int _y) { this->position.y = _y; }
protected:
	std::string textureID;

	int currentFrame;
	int currentRow;
};

#endif
