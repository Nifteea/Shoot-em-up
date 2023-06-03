#include <SDL.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Game.h"
#include "TextureManager.h"

Game* Game::instance = 0;

Game::Game()
{
	std::cout << "success creating game" << std::endl;
	bool test = initialise("OOP Assignment Game", 100, 100, 640, 480, false); //initialise SDL and create window
	if (test == false)
	{
		exit(0);
	}
}

bool Game::initialise(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;
	if (fullscreen) //if fullscreen is true
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}
	// try initialising SDL, log error and pause if fail
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << "\n";
		system("pause");
		return false;
	}

	// try to create the window, log error and pause if fail
	window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
	if (window == NULL)
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		system("pause");
		return false;
	}

	// try to create the renderer, log error and pause if fail
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
	{
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		return false;
	}

	srand(time(NULL));

	//load audio
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0)
	{
		std::cout << "Error initializing SDL_mixer: " << Mix_GetError() << std::endl;
		return false;
	}
	music = Mix_LoadMUS("assets/backgroundMusic.wav");
	explosionSfx = Mix_LoadWAV("assets/explosion.wav");
	shootSfx = Mix_LoadWAV("assets/shoot.wav");
	if (!music || !explosionSfx || !shootSfx)
	{
		std::cout << "Error loading sound: " << Mix_GetError() << std::endl;
		return false;
	}

	//load font 
	if (TTF_Init() < 0)
	{
		std::cout << "Error initializing SDL_ttf: " << TTF_GetError() << std::endl;
	}
	font = TTF_OpenFont("assets/terminal.ttf", 24);
	if (!font) 
	{
		std::cout << "Error loading font: " << TTF_GetError() << std::endl;
		return false;
	}
	textDest = { 0, 0, 0, 0 };

	//load images
	if (!theTextureManager::Instance()->load("assets/explosion.bmp", "explosion", renderer))
	{
		return false;
	}
	if (!theTextureManager::Instance()->load("assets/background.bmp", "background", renderer))
	{
		return false;
	}
	if (!theTextureManager::Instance()->load("assets/menu.bmp", "menu", renderer))
	{
		return false;
	}
	if (!theTextureManager::Instance()->load("assets/endScreenLose.bmp", "lose", renderer))
	{
		return false;
	}if (!theTextureManager::Instance()->load("assets/endScreenWin.bmp", "win", renderer))
	{
		return false;
	}
	if (!theTextureManager::Instance()->load("assets/spaceship.bmp", "spaceship", renderer))
	{
		return false;
	}
	if (!theTextureManager::Instance()->load("assets/enemy.bmp", "enemy", renderer))
	{
		return false;
	}
	if (!theTextureManager::Instance()->load("assets/playerBullet.bmp", "bullet", renderer))
	{
		return false;
	}
	return true;
}

void Game::gameLoop()
{
	//play background music
	int result = Mix_PlayMusic(music, -1);
	if (result != 0)
	{
		std::cout << "Failed to play music: " << Mix_GetError() << std::endl;
	}

	//create player
	player.load("spaceship", 300, 300, 512, 512, 0.1); //initial x and y coords and width and height

	//create enemies
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		enemies[i].alive = false;
		enemies[i].load("enemy", 600, 0, 809, 434, 0.1); //load textures
	}

	//create bullets
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		bullets[i].alive = false;
		bullets[i].load("bullet", 0, 0, 131, 50, 0.1); //load textures
	}

	//main game loop
	gameRunning = true;
	while (gameRunning)
	{
		while (menu == true) //displays menu screen until player clicks
		{
			handleEvents();
			render();
		}
		while (endScreen == true)
		{
			handleEvents();
			render();
		}

		frameStart = SDL_GetTicks();

		update(); //updates player, enemy and bullet positions and checks for collisions
		handleEvents(); //if player closes game, the program quits
		render(); //draws

		frameTime = SDL_GetTicks() - frameStart;
		if (frameTime < DELAY_TIME)
		{
			SDL_Delay((int)(DELAY_TIME - frameTime));
		}
		else
		{
			SDL_Delay((int) (DELAY_TIME));
		}
	}
	clean();
}

void Game::update()
{
	//get keyboard
	const Uint8* key = SDL_GetKeyboardState(NULL);

	//create new enemies
	if ((rand() % enemyOdds) == 0)
	{
		addEnemy();
	}

	//create new bullets
	if (reloading == false) //if the player is not reloading they are allowed to shoot
	{
		if (key[SDL_SCANCODE_SPACE] == SDL_PRESSED)
		{
			addBullet();
		}
	}
	reloading = key[SDL_SCANCODE_SPACE] == SDL_PRESSED;

	//update each enemy's position
	for (int i = 0; i < MAX_ENEMIES; i++) 
	{
		enemies[i].update();
	}

	//update player's position
	player.update(); 

	//update each bullet's position
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		bullets[i].update();
	}

	//check all enemies if they collide with any bullets
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		for (int j = 0; j < MAX_BULLETS; j++)
		{
			if (enemies[i].alive == true && bullets[j].alive == true)
			{
				if (checkCollision(&enemies[i], &bullets[j]))
				{
					score += 1;
					enemies[i].alive = false;
					bullets[j].alive = false;
					//explosion
				}
			}
		}
	}

	//check all enemies if they collide with the player
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		if (enemies[i].alive == true)
		{
			if (checkCollision(&player, &enemies[i])) //if enemy collides with player, player looses a life and becomes invisible for x frames
			{
				playerLives -= 1;
				std::cout << "collision";
				enemies[i].alive = false;
				int channel = Mix_PlayChannel(-1, explosionSfx, 0);
				//set player to immune for 3 secs
			}
		}
	}
	if (playerLives <= 0) //if the player looses 3 lives, the game ends and the player loses
	{
		win = false;
		endScreen = true;
	}
	if (score == GOAL_SCORE) //if player kills 10 enemies and gets a score of 10, the game ends and the player wins
	{
		win = true;
		endScreen = true;
	}
	
	if (enemyOdds > 60) //make game progressively harder over time by increasing enemy spawn rate
	{
		enemyOdds = enemyOdds - 1;
	}
}

void Game::handleEvents() //allow closing SDL window to quit
{
	SDL_PollEvent(&event); 
	switch (event.type) //detects user input
	{
	case SDL_MOUSEBUTTONDOWN:
		menu = false;
		break;
	case SDL_QUIT:
		menu = false;
		endScreen = false;
		gameRunning = false;
	}

}

void Game::render() //clears the renderer and then renders again with the clear colour
{
	SDL_RenderClear(renderer); //clear renderer

	//display relevant screen
	if (menu == true)
	{
		displayMenu();
	}
	else if (endScreen == true)
	{
		displayEndScreen();
	}
	else
	{
		theTextureManager::Instance()->draw("background", 0, 0, 320, 256, 2, renderer);
		for (int i = 0; i < MAX_ENEMIES; i++)
		{
			if (enemies[i].alive)
			{
				enemies[i].draw(renderer);
			}
		}
		if (playerLives > 0)
		{
			player.draw(renderer);
		}
		for (int i = 0; i < MAX_BULLETS; i++)
		{
			if (bullets[i].alive)
			{
				bullets[i].draw(renderer);
			}
		}
		updateScore(renderer, "SCORE:", font, textDest, score); //update score
	}

	SDL_RenderPresent(renderer); //update the screen

}

void Game::clean() //clean up any memeory that was used
{
	std::cout << "cleaning game" << std::endl;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	Mix_CloseAudio();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void Game::displayMenu()
{
	theTextureManager::Instance()->draw("menu", 0, 0, 827, 668, 0.8, renderer);
}

void Game::displayEndScreen()
{
	if (win == true) //if the player won, display win screen
	{
		theTextureManager::Instance()->draw("win", 0, 0, 1226, 1008, 0.55, renderer);
	}
	else //if the player lost, display lose screen
	{
		theTextureManager::Instance()->draw("lose", 0, 0, 1227, 1008, 0.55, renderer);
	}

	SDL_RenderCopy(renderer, textTexture, NULL, &textDest);
}

void Game::addEnemy()
{
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		if (enemies[i].alive == false)
		{
			int randNum = -25 + rand() % 445; //generate random y coordinate
			enemies[i].position.y = randNum;
			enemies[i].position.x = 615;
			enemies[i].alive = true;
			return;
		}
	}
}

void Game::addBullet()
{
	for (int i = 0; i < MAX_BULLETS; i++) //check for free bullet slot
	{
		if (bullets[i].alive == false)
		{
			bullets[i].position.x = player.position.x + (player.position.w * player.scale + bullets[i].position.w * bullets[i].scale) / 2;
			bullets[i].position.y = player.position.y + (player.position.h * player.scale + bullets[i].position.h * bullets[i].scale) / 2;
			bullets[i].alive = true;
			int channel = Mix_PlayChannel(-1, shootSfx, 0);
			return;
		}
	}
}

bool Game::checkCollision(GameObject* go1, GameObject* go2)
{
	SDL_Rect rect1;
	SDL_Rect rect2;

	rect1.x = go1->getX(); rect1.y = go1->getY(); rect1.w = go1->getW(); rect1.h = go1->getH();
	rect2.x = go2->getX(); rect2.y = go2->getY(); rect2.w = go2->getW(); rect2.h = go2->getH();
	bool result = SDL_HasIntersection(&rect1, &rect2);

	return(result); //collision
}

void Game::updateScore(SDL_Renderer* renderer, std::string displayText, TTF_Font* font, SDL_Rect& textDest, int& score)
{
	displayText += std::to_string(score);
	//render text to a surface
	SDL_Colour colour = { 0, 0, 0 };
	textSurface = TTF_RenderText_Solid(font, displayText.c_str(), colour);
	textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_FreeSurface(textSurface);
	SDL_QueryTexture(textTexture, 0, 0, &textDest.w, &textDest.h);
	textDest.x = 500;
	textDest.y = 10;
	SDL_RenderCopy(renderer, textTexture, 0, &textDest);
	SDL_DestroyTexture(textTexture);
}