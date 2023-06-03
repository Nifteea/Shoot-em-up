#ifndef _GAME_H_
#define _GAME_H_
#include <SDL.h>
#include <iostream>
#include <string>
#include <vector>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "TextureManager.h"
#include "GameObject.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"

const int FPS = 60;
const float DELAY_TIME = 1000.0f / FPS;
const int MAX_BULLETS = 4;
const int MAX_ENEMIES = 6; //maximum enemies on the screen at any given time
const int GOAL_SCORE = 10;

class Game
{
private:
	Game();
	~Game();

	void handleEvents();
	void update();
	void render();
	void clean();

	static Game* instance;

	SDL_Window* window;
	SDL_Renderer* renderer;
	Mix_Music* music;
	Mix_Chunk* explosionSfx;
	Mix_Chunk* shootSfx;
	TTF_Font* font;
	SDL_Surface* textSurface;
	SDL_Texture* textTexture;
	SDL_Rect textDest;

	int currentFrame;
	int currentRow;

	Uint32 frameStart;
	Uint32 frameTime; //the amount of time taken to process the frame
	bool gameRunning;
	SDL_Event event;
	bool menu = true;
	bool endScreen = false;
	int playerLives = 3;
	int score = 0;
	bool win;
	int enemyOdds = 200;

	Player player;
	Enemy enemies[MAX_ENEMIES];
	Bullet bullets[MAX_BULLETS];
	bool reloading;
public:
	static Game* Instance()
	{
		if (instance == 0)
		{
			instance = new Game();
		}
		return instance;
	}

	bool initialise(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	void gameLoop();
	void displayMenu();
	void displayEndScreen();
	void addEnemy();
	void addBullet();
	bool checkCollision(GameObject* go1, GameObject* go2);
	void updateScore(SDL_Renderer* renderer, std::string displayText, TTF_Font* font, SDL_Rect& textDest, int& score);
};

typedef Game theGame;

#endif