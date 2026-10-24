#pragma once
#include <algorithm>
#include <string>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>
class GameLogic
{
 private:
	int maxBalls;
	int desballs;
	int currentCatches;
	bool isWon;
	bool alrwon = false; // safebool from permaplaying the victory sound when alr won
	sf::SoundBuffer bufwin;
	std::optional<sf::Sound> winsound;
 public:
	 
	 int getCurrentCatches() const{
		 return currentCatches;
	 }
	 int getDesBalls() const {
		 return desballs;
	 }
	 bool returnState() const {
		 return isWon;
	 }
	 
	
	 void incrementTargetCatches()
	 {
		 if (this->desballs < this->maxBalls)
		 {
			 this->desballs++;
		 }
	 }
	 void decrementTargetCatches()
	 {
		 if (this->desballs > 1)
		 {
			 this->desballs--;
		 }
	 }
	GameLogic()
	{
		this->desballs = 10;
		this->maxBalls = 256;
		this->currentCatches = 0;
		this->isWon = false;
		if (!this->bufwin.loadFromFile("Sounds/Vicsound.wav"))
		{
			std::cerr << "Can't open the audio file!\n";
		}
		this->winsound.emplace(this->bufwin);
	}

	void reset()
	{
				this->currentCatches = 0;
				this->isWon = false;
				this->alrwon = false; // not to forget alrwon is a third-leg boolean to prevent from spamming victory sound when the user refuses to reset the game after winning
	}


	void plusScore()
	{
		currentCatches++;
		if (this->currentCatches >= this->desballs)
		{
			
			this->currentCatches = this->desballs;
			this->isWon = true;
			if(alrwon == false)
			if (this->winsound.has_value())
			{
				this->winsound->play();
				this->alrwon = true;
			}
		}
	}

};