#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "GameObject.h"

class Enemy : public GameObject
{
private:
	int enemyID;
public:
	Enemy() {}
	~Enemy() {}
	void load(std::string textureID, int x, int y, int width, int height, double scale);
	void draw(SDL_Renderer* renderer);
	void update();
	int getEnemyId() { return this->enemyID; }
};

#endif