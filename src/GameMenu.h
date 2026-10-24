#pragma once
#include "Ball.h"
#include "Dog.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <optional>
#include "GameLogic.h"

class GameMenu
{
public:
	sf::Text newgame; 
	sf::Text exitgame;
	sf::Text settings;
	sf::Text undobutton;
	sf::Text targetBalls;
	sf::Text win;
	sf::Text volumetracker; // Creating the textbuttons and trackers for the menu
	sf::Text nextskin;
	sf::Text prevskin;


	sf::Sprite titlesprite;
	sf::Texture titletexture;
	// audio and the base volume
	std::optional<sf::Sound> hoversound; // sound effect for hovering over the buttons
	float currentvolume = 100.f;
	enum class MenuState { mainmenu, optionsmenu, leavemenu }; // adding additional menus for buttons pressed
	MenuState currentstate = MenuState::mainmenu;
	GameMenu(); // base constructor
	void hovermenu(const sf::Vector2f& mpose); // for hovering over buttons in the main menu
	void updatemenu(sf::RenderWindow& win); // for animations or sprites inside of that stuff

	void draw(sf::RenderWindow& win) //drawing function
	{
		if (this->currentstate == MenuState::mainmenu)
		{
			win.setView(win.getDefaultView());
			win.draw(titlesprite);
			win.draw(newgame);
			win.draw(exitgame);
			win.draw(settings);
			win.draw(dogsprite);
			
		}
		else if (this->currentstate == MenuState::optionsmenu)
		{
			win.draw(volumetracker);
			win.draw(undobutton);
			win.draw(targetBalls);
			win.draw(nextskin);
			win.draw(prevskin);
			
		}
	}

	// showcasing the window
	void renderwinhud(sf::RenderWindow& win)
	{
		win.draw(this->win);
	}

	// Processing of the interactions with the mouse for the main menu UI
	// bool here is to trigger the gaming state
	bool events(sf::RenderWindow& win, const sf::Event& eve)
	{
		sf::Vector2f winmousepos = sf::Vector2f(sf::Mouse::getPosition(win)); // since we have to put it to the 2f anyway it's nothing
		if (exitgame.getGlobalBounds().contains(winmousepos))
		{
			exitgame.setFillColor(sf::Color::Red);
			exitgame.setOutlineColor(sf::Color::White);
		}
		else
		{
			exitgame.setFillColor(sf::Color::White);
			exitgame.setOutlineColor(sf::Color::Black);
		}
		if (newgame.getGlobalBounds().contains(winmousepos))
		{
			newgame.setFillColor(sf::Color::Red);
			newgame.setOutlineColor(sf::Color::White);
		}
		else
		{
			newgame.setFillColor(sf::Color::White);
			newgame.setOutlineColor(sf::Color::Black);
		}
		if (eve.is<sf::Event::MouseButtonPressed>()) // since eve.type is no longer a thing right logic is eve.is<T>
		{
			if (exitgame.getGlobalBounds().contains(winmousepos)) // if pressed exit game - close the window, worstcase we can do the soft check with if the user rly wants to quit
			{
				win.close();
			}
			if (newgame.getGlobalBounds().contains(winmousepos)) // logic for the newgame button
			{
				return true;
			}
		}
		return false;
	}

	void updateHud(int current, int target)
	{
		this->hudtext.setString("current fetches: " + std::to_string(current) + " / " + std::to_string(target));
	}

	void renderHud(sf::RenderWindow& win, const GameLogic& logic)
	{
		
		win.draw(this->hudtext);
	}

	void updateTargetTracker(int target)
	{
		this->targetBalls.setString("Target Fetches: " + std::to_string(static_cast<int>(target)));
	}

private:

	sf::Font font;
	
	// stuff for the menu
	sf::Text hudtext; // for showcasing the stuff during the gameplay
	
	// for the dog animation
	sf::Texture dogtexture;
	sf::Sprite dogsprite;
	int currentframe = 0; // starter frame for the anim
	int totalframes = 3; // the menu gif is 3 frames so... yeah
	sf::Clock animclock; // the clock for the animation
	float animdur = 0.15f; // total duration of menu gif
	
	sf::SoundBuffer buf; // sound buffer
	
	bool isHovered = false; // boolean to prevent from overusing the sound
};