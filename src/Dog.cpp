#include "Dog.h" // for using the dog states and it's variables
#include "Ball.h" // for tracking the ballstates
#include "GameEng.h" // for tracking the positions
#include <cmath> // for calculating the direction and distance
#include <SFML/Main.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
void dog::dogupdate(float dt, sf::Vector2f playerpos, ball::ballstate bstate, sf::Vector2f ballposition)
{
	float dogspeed = 150.f; // the base speed of the dog
	switch (state)
	{
	case dogstate::Idle:
	{
		// In IDLE state we casually wait for the ball to be in the FLY state caused by us throwing the ball
		if (bstate == ball::ballstate::Fly)
		{
			this->state = dogstate::Catch;
		}
		break;
	}
	case dogstate::Catch:
	{
		if (bstate == ball::ballstate::Fly || bstate == ball::ballstate::Grounded) //during the flying or grounded state the dog gets the ballpos and moves to it the further the faster
		{
			sf::Vector2f dir = ballposition - this->dogsprite.getPosition(); // getting the direction for the dog
			this->dogsprite.move(dir * dt); // direction * time
			sf::Vector2f diff = ballposition - this->dogsprite.getPosition(); // difference in coords
			float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y); // x*x + y*y

			// this block was added later to try to force the dog to jump
			float ballabovehead = this->dogsprite.getPosition().y - ballposition.y; // same as diff but only for y coords
			if (ballabovehead > 0.f && ballabovehead <= 20.f)
			{
				this->dogsprite.move({0.f, -20.f}); // since the 0 y is happened to be at the top left by setting it to -20 makes it go UP
			}
			// current issue: if I am to throw a ball higher than ball position but away from the dog the "jump" is still getting triggered
			// possible fix: add another if cause checking if the X axis is different by slight amount (5.f for example?)


			if (distance < 20.f && bstate == ball::ballstate::Grounded) // if the ball is grounded and dog is near set the state of the dog to bring back the ball
			{
				this->state = dogstate::Bring;
			}
		}
		break;
	}
	case dogstate::Bring:
	{
		sf::Vector2f dir = playerpos - this->dogsprite.getPosition(); 
		this->dogsprite.move(dir * dt);
		sf::Vector2f diff = playerpos - this->dogsprite.getPosition(); // been explained in the catch case
		float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y); 
		if (distance < 25.f) // if the distance is close we reset the state to IDLE until the player throws again
		{
			this->state = dogstate::Idle;
		}
		break;
	}
	}
}