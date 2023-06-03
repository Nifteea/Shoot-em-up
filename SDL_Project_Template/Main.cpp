#include <iostream>
#include "Game.h"

int main(int argc, char* argv[])
{
	if (theGame::Instance() != 0)
	{
		theGame::Instance()->gameLoop();
	}
	else
	{
		std::cout << "error creating game " << std::endl;
	}
	return 0;
}