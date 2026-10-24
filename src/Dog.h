#pragma once
#include <SFML/Graphics.hpp>
#include "Ball.h"
#include <iostream>
#include <fstream>
class dog // COPYPASTED FROM PLAYERMODEL CLASS
{
private:
	sf::Sprite dogsprite;
public:
	sf::Texture dogtext;
	enum class dogstate { Idle, Catch, Bring };
	dogstate state{ dogstate::Idle };
	dogstate getstate() const { return this->state; }
	sf::Vector2f getPosition() const { return this->dogsprite.getPosition(); }
	dog(sf::Vector2f startpos) : dogsprite(dogtext)
	{ // safecheck for fileloading
		if (!dogtext.loadFromFile("Textures/DogIdle1.png")) {
			std::cerr << "Error, can't load the file";
			std::ofstream wfile("LogReport.txt", std::ios::app);
			wfile << "Dog sprite : failure\n";
			wfile.close();
		}
		else
		{
			std::ofstream wfile("LogReport.txt", std::ios::app);
			wfile << "Dog sprite : sucess\n";
			wfile.close();
		}
		dogsprite.setTexture(dogtext, true);
		dogsprite.setScale({ 2.f, 2.f });
		dogsprite.setPosition(startpos);
	}

	// drawing dog and handling it's states
	void dogupdate(float dt, sf::Vector2f playerpos, ball::ballstate bstate, sf::Vector2f ballposition); 

	void dogdraw(sf::RenderWindow& win) const 
	{
		win.draw(dogsprite);
	}
};