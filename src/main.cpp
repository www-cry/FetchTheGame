#include "GameEng.h"
#include <iostream>


int main()
{
	gameeng game; // object for gameeng class
	int testcount = tracker(); // tracking the amount of the times the program was launched
	std::cout << "Counter is at: " << testcount; // printing the amount of times the game was launched;
	game.run(); // executing the game loop
	return 0;
}