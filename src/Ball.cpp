#include "Ball.h" // for calling ball states
#include "GameEng.h" // for aball object
#include <cmath> // calculating the movements
#include <SFML/Main.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
// setting up the ball params and position for calling
void ball::createaball(sf::RenderWindow& win)
{
	sf::Vector2f winmousepos = sf::Vector2f(sf::Mouse::getPosition(win));
	aball.setRadius(6.f);
	aball.setFillColor(sf::Color::Green);
	aball.setOutlineColor(sf::Color::White);
	aball.setOrigin({ 9.f, 9.f });
	aball.setPosition(winmousepos);
}

// Alternate between ball states and calculating the ball throwing
void ball::ballevent(const sf::Event& ev, const sf::RenderWindow& win, sf::Vector2f playerpos)
{
	
	if (const auto* mpressed = ev.getIf<sf::Event::MouseButtonPressed>())
	{
		if (mpressed->button == sf::Mouse::Button::Left && state == ballstate::Held) // basically for hold throwing the ball
		{
			ischarge = true;
			chargetime = 0.f;
		}
	}
	if (const auto* mreleased = ev.getIf<sf::Event::MouseButtonReleased>()) // and when it's released we set it to the FLY state
	{
		if (mreleased->button == sf::Mouse::Button::Left && ischarge)
		{
			ischarge = false;
			float ratio = std::min(chargetime / maxcharge, 1.0f);
			float launchspeed = basespeed + ratio * (maxspeed - basespeed);
			sf::Vector2f mpos = win.mapPixelToCoords(sf::Mouse::getPosition(win));
			sf::Vector2f dir = mpos - this->aball.getPosition();
			float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
			if (length != 0.f)
			{
				dir /= length;
			}
			velocity = dir * launchspeed;
			state = ballstate::Fly;
		}
	}
}

void ball::update(float dt, sf::Vector2f playerpos) // here lies the description between all the states
{
	if (ischarge)
	{
		chargetime += dt;
		if (chargetime > maxcharge)
		{
			chargetime = maxcharge;
		}
	}
	switch (state)
	{
	case ballstate::Held: { 
		
		sf::Vector2f handcoords = { 42.f, 23.f }; // it was picked by experimenting on where would the ball end up cuz at first it was ending on the top left corner as "starter coord"
		this->aball.setPosition(playerpos + handcoords);
		break; } // basically when it's held we make the ball "glued" to the handcoords(or dogpos)
	case ballstate::Fly: {
		

		// here it's a lot of math which has been explained by AI but as the time went I tended to forget what was the logic behind it...
		aball.move(velocity * dt);
		velocity -= velocity * drageffect * dt;
		sf::Vector2f pos = aball.getPosition();
		float rad = aball.getRadius();
		float bouncefactor = 0.70f;
		const float winwidth = 800.f;
		const float winheight = 600.f;
		bool collided = false;
		if (pos.x - rad < 0.f && velocity.x < 0.f)
		{
			pos.x = rad;
			velocity.x = -velocity.x * bouncefactor;
			collided = true;
		}
		else if (pos.x + rad > winwidth && velocity.x > 0.f)
		{
			pos.x = winwidth - rad;
			velocity.x = -velocity.x * bouncefactor;
			collided = true;
		}
		if (pos.y - rad < 0.f && velocity.y < 0.f)
		{
			pos.y = rad;
			velocity.y = -velocity.y * bouncefactor;
			collided = true;
		}
		else if (pos.y + rad > winheight && velocity.y > 0.f)
		{
			pos.y = winheight - rad;
			velocity.y = -velocity.y * bouncefactor;
			collided = true;
		}
		if (collided)
		{
			aball.setPosition(pos);
		}
		
		if (std::hypot(velocity.x, velocity.y) < 15.f) {
			velocity = { 0.f, 0.f }; state = ballstate::Grounded; // when it finally lands we set it's state to grounded
		} 
		break;
	}
	case ballstate::Grounded: {
		sf::Vector2f diff = aball.getPosition() - playerpos; // calculating the difference between playerpos and ballpos
		float dist = std::hypot(diff.x, diff.y); 
		// Pickup radius
		if (dist < 40.0f)
		{
			state = ballstate::Held; // Collision detected! Transitions back to Held
		}
	}
	break;
	}
};