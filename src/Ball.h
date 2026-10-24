#pragma once
#include <SFML/Graphics.hpp>
class ball // here I've decided to set no sprites cuz omfg it's just a ball
{
public:
	enum class ballstate { Held, Fly, Grounded }; // setting up the states and base shape
	ballstate state{ ballstate::Held };
	sf::CircleShape aball;
private:

	// for ball movement calculations
	sf::Vector2f velocity{ 0.f,0.f };
	bool ischarge = false;
	float chargetime{ 0.f };
	const float maxcharge{ 1.5f };
	const float basespeed{ 300.f };
	const float maxspeed{ 900.f };
	const float drageffect{ 2.5f };

public:
	// functions for: creating a ball, drawing, setting the position, setting the state
	void createaball(sf::RenderWindow& win);
	void drawball(sf::RenderWindow& win) const {
		win.draw(aball);
	}
	void setpos(sf::Vector2f newpos)
	{
		this->aball.setPosition(newpos);
	}
	void setstate(ballstate s)
	{
		this->state = s;
	}


	void ballevent(const sf::Event& ev, const sf::RenderWindow& win, sf::Vector2f sparxpos); // updating the ball's state and 
	sf::FloatRect getglobalbounds() const { return this->aball.getGlobalBounds(); } // checking for collision
	void update(float dt, sf::Vector2f playerpos); // for states of the ball and calculations with the movements
	sf::Vector2f getposition() const { return aball.getPosition(); } // calling for the coordinates of where does the ball lands
	ballstate getState() const { return state; } // to get current state
};