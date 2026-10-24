#pragma once
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include <fstream>
#include <cmath>
#include "Dog.h"
#include "Ball.h"
#include "SkinSelector.h"
#include "GameMenu.h"
#include "GameLogic.h"

class playermodel
{
private:
	sf::Sprite sprite;
	std::string path = "Textures/Sparxie.png";
	float ms = 300.f; // base speed of the player
	// initialising the texture and sprite varibales
public:
	sf::Texture texture;
	sf::Vector2f getposition() const { return this->sprite.getPosition(); }
	sf::FloatRect getglobalbounds() const { return this->sprite.getGlobalBounds(); }
	// getting the collision and coordinates of playerposition
	playermodel(sf::Vector2f startpos) : sprite(texture)
	{
		if (!texture.loadFromFile(path)) { // safecheck
			std::cerr << "Error, can't load the file";
			std::ofstream wfile("LogReport.txt", std::ios::app);
			wfile << "Player sprite : failure\n";
			wfile.close();
		}
		else
		{
			std::ofstream wfile("LogReport.txt", std::ios::app);
			wfile << "Player sprite : sucess\n";
			wfile.close();
		}
		sprite.setTexture(texture, true);
		sprite.setScale({ 1.5f, 1.5f });
		sprite.setPosition(startpos);
	}
	// in update we handle the movement controls via WASD or arrow keys + use SHIFT to sprint
	void playerupdate(float dt,const sf::Vector2u& windowSize)
	{
		sf::Vector2f movement(0.f, 0.f);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)))
		{
			movement.y -= 1.f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)))
		{
			movement.y += 1.f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)))
		{
			movement.x -= 1.f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)))
		{
			movement.x += 1.f;
		}
		// for diagonal movement
		if (movement.x != 0.f && movement.y != 0.f)
		{
			movement *= 0.7071f;
		}
		float currentspeed = this->ms;
		// adding shifting
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift))
		{
			movement *= 1.75f;
		}
		this->sprite.move(movement * currentspeed * dt);
		// bounds
		sf::FloatRect bounds = this->sprite.getGlobalBounds();
		sf::Vector2f coords = this->sprite.getPosition();

		if (coords.x < 0.f) { coords.x = 0.f; }
		if (coords.y < 0.f) { coords.y = 0.f; }
		if (coords.x + bounds.size.x > static_cast<float>(windowSize.x)) { coords.x = static_cast<float>(windowSize.x) - bounds.size.x; }
		if (coords.y + bounds.size.y > static_cast<float>(windowSize.y)) { coords.y = static_cast<float>(windowSize.y) - bounds.size.y; }
		this->sprite.setPosition(coords);
	};
		
	void drawplayer(sf::RenderWindow& win) const {
		win.draw(sprite);
	}

	void applyskin(const sf::Texture& tmp); // for applying skin selected in the options menu
};

	
// gameeng class acts as game engine, it's task is to initialise most of the stuff beforehand in a way that later
// it's gonna be used alot to call out for almost anything in source1
class gameeng
{
private:
	void initw(); // init a window
	void initint(); // init the variables
	void initplayer(); // init the player
	void initdog(); // init the dog since it also has the sprites apparently
	ball gameball; // creating an object for the ball
	std::unique_ptr<dog> doga; // same as the smart pointer for the player model as it's expected to add several sprites both to the player and the dog(god help me)
	// Here will be also an object for the dog //
	sf::RenderWindow* window; // creating a renderwindow pointer?
	GameMenu menu; // basically an object of GameMenu so we can use the gamemenu functions in the game engine
	GameLogic glogic; // for calling the reset function for now
	std::unique_ptr<playermodel> player; // we create a smart pointer to the player so it's easier to work with it
	bool isplay = false; // simple boolean for game mode and menu mode
	SkinSelector previewlist; // list of the skins for optionsmenu
public:
	void run(); // for starting the game
	gameeng(); // base constructod
	virtual ~gameeng(); // base destructor

	const bool getWindowIsOpen() const; // boolean for window being open
	void update(float dt); // updating the screen
	void render(); // adding objects and etc
	void pollEvents(); // for the window events
	void chktext(); // for checking loaded textures
	void updateskin(); // for updating skin
};

inline int tracker() // tracking the amount of times the game was opened and in plans to make a checklist of what's been loaded and not
{
	std::string filename = "LogReport.txt";
	int counter = 0;
	std::ifstream rFile("LaunchCount.txt");
	if (rFile.is_open())
	{
		rFile >> counter;
		rFile.close();
	}
	counter++;
	std::ofstream wFile("LaunchCount.txt")	;
	if (!wFile)
	{
		std::cerr << "FILE FAILED TO LAUNCH!\n";
	}
	if (wFile.is_open())
	{
		wFile << counter;
		wFile.close();
	}
	
	std::ofstream cFile(filename, std::ios::app);

	if (cFile.is_open())
	{
		cFile << "\n == Game session #" << counter << " has been started == \n";
		cFile.close();
	}
	else
	{
		std::cerr << "COULDN'T OPEN THE FILE\n";
	}

	return counter;
}

struct textchest
{
	sf::Texture& texture;
	std::string path;
};

inline bool isfileloaded(std::vector<textchest>& textures, const std::string& logfile = "LogReport.txt")
{
	std::ofstream checker(logfile, std::ios::app);
	checker << "\n == Checking == \n";
	bool everythingloaded = true;

	for (auto& a : textures)
	{
		if (a.texture.loadFromFile(a.path))
		{
			if (checker.is_open())
			{
				checker << "\n == File " << a.path << " loaded\n";
			}
			else
			{
				std::cerr << "Can't open the file\n";
			}
		}
		else
		{
			if (checker.is_open())
			{
				checker << "\n == File " << a.path << " was not loaded\n";
				everythingloaded = false;
			}
			else
			{
				std::cerr << "Can't open the file\n";
			}
		}
	}
	return everythingloaded;
}