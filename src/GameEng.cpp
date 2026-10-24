#include "Ball.h"
#include "Dog.h"
#include "GameEng.h"
#include "GameMenu.h"
#include "SkinSelector.h"
#include <windows.h>
// describing the initialize stuff from the gameeng

void gameeng::initplayer()
{
	this->player = std::make_unique<playermodel>(sf::Vector2f(100.f, 300.f));
}
void gameeng::initdog()
{
	this->doga = std::make_unique<dog>(sf::Vector2f(150.f, 300.f));
}
// describing the dog and player b4hand


void gameeng::initint() // setting the window to nullptr b4 giving it the params
{
	this->window = nullptr;
}

void gameeng::initw() // setting the window params and framerate
{
	this->window = new sf::RenderWindow(sf::VideoMode({ 800, 600 }), "NewStartGame", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(60);
}

const bool gameeng::getWindowIsOpen() const // safe bool for keeping the window is open and nothing is wrong with it
{
	return this->window != nullptr && this->window->isOpen();
}

gameeng::gameeng() // constructor for every initialisation
{
	this->initint();
	this->initw();
	this->initplayer();
	this->initdog();
	this->menu.updateTargetTracker(this->glogic.getDesBalls()); // this one was added later when was working on in-game HUD
	this->previewlist.checkskins();
}

gameeng::~gameeng() // destructor for the window pointer
{
	delete this->window;
}

void gameeng::pollEvents() // here we hold all the events inside the window
{
	while (const std::optional<sf::Event> ev = this->window->pollEvent())
	{
		if (ev->is<sf::Event::Closed>()) // on closing event we also close the window
		{
			this->window->close();
		}
		if (const auto* keyPressed = ev->getIf<sf::Event::KeyPressed>()) // here we put that if someone'd press Esc button the game would close
		{
			if (keyPressed->code == sf::Keyboard::Key::Escape)
			{
				this->window->close();
			}
        }
		if (isplay) // isplay is a boolean to alternate between game state and any other state
		{
			if (auto keypressed = ev->getIf<sf::Event::KeyPressed>()) // ingame I've made the reset trigger just in case
			{
				if (keypressed->code == sf::Keyboard::Key::R)
				{
					this->glogic.reset();
					this->menu.updateHud(this->glogic.getCurrentCatches(), this->glogic.getDesBalls());
					
				}
			}
			if (this->player) // if player exists(not nullptr) we get the ballevent going
			{
				this->gameball.ballevent(*ev, *this->window, this->player->getposition());
			}
		}
		// going for the menu mode
		if (!this->isplay)
		{
			if(this->menu.events(*this->window, *ev))
			{
				this->isplay = true; // flip the state of the gameengine
			}
			if (const auto* mPressed = ev->getIf<sf::Event::MouseButtonPressed>()) // here once we get the lmb event we check the ifs for the different menu states
			{
				if (mPressed->button == sf::Mouse::Button::Left)
				{
					sf::Vector2f mousepos = this->window->mapPixelToCoords(mPressed->position); // the variable to get the exact coords of the coursor

					if (this->menu.currentstate == GameMenu::MenuState::mainmenu) // if in the main menu we triggered any buttons
					{
						if (this->menu.newgame.getGlobalBounds().contains(mousepos))
						{
							this->gameball.createaball(*this->window);
							this->isplay = true;
						}
						else if (this->menu.settings.getGlobalBounds().contains(mousepos))
						{
							this->menu.currentstate = GameMenu::MenuState::optionsmenu; // setting the state to the options menu
						}
					}
				}
			}
			if (this->menu.currentstate == GameMenu::MenuState::optionsmenu) // in the options all we do is set the back to the main menu button 
			{
				
				sf::Vector2f mousepos = sf::Vector2f(sf::Mouse::getPosition(*this->window));
				if (ev->getIf<sf::Event::MouseButtonPressed>())
				{
					if (this->menu.undobutton.getGlobalBounds().contains(mousepos))
					{
						this->menu.currentstate = GameMenu::MenuState::mainmenu;
					}
					if (this->menu.nextskin.getGlobalBounds().contains(mousepos))
					{
						this->previewlist.nextskin();
						this->updateskin();
					}
					if (this->menu.prevskin.getGlobalBounds().contains(mousepos))
					{
						this->previewlist.prevskin();
						this->updateskin();
					}
				}
				// adjusting the volume
				bool volumechanged = false;
				if (auto keypressed = ev->getIf<sf::Event::KeyPressed>())
				{
					if (keypressed->code == sf::Keyboard::Key::Subtract || keypressed->code == sf::Keyboard::Key::Hyphen )
					{
						if (this->menu.currentvolume > 0.f)
						{
							this->menu.currentvolume -= 1.f;
							volumechanged = true;
						}
					}
					if (keypressed->code == sf::Keyboard::Key::Add || keypressed->code == sf::Keyboard::Key::Equal) // same but in opposite way + =
					{
						if (this->menu.currentvolume < 100.f)
						{
							this->menu.currentvolume += 1.f;
							volumechanged = true;
						}
					}
					if (volumechanged) // displaying the value of the current volume on screen
					{
						if (this->menu.hoversound.has_value())
						{
							this->menu.hoversound->setVolume(this->menu.currentvolume);
						}
						this->menu.volumetracker.setString("Volume: " + std::to_string(static_cast<int>(this->menu.currentvolume)) + "%");
					}
					if (keypressed->code == sf::Keyboard::Key::Left) // here we either decrease the target fetches or increase(yes, it has it's limits(256))
					{
						this->glogic.decrementTargetCatches();
						this->menu.updateTargetTracker(this->glogic.getDesBalls());
					}
					if (keypressed->code == sf::Keyboard::Key::Right)
					{
						this->glogic.incrementTargetCatches();
						this->menu.updateTargetTracker(this->glogic.getDesBalls());
					}
					if (keypressed->code == sf::Keyboard::Key::A)
					{
						this->previewlist.nextskin();
						this->updateskin();
					}
					if (keypressed->code == sf::Keyboard::Key::D)
					{
						this->previewlist.prevskin();
						this->updateskin();
					}
				}	
			}
		}
	}
}
void gameeng::update(float dt) // updates either game state or menu logic based on the delta time
{
	this->pollEvents(); // calling the window events(like resetting the goal with R)
	if (isplay) // if in the game state
	{
		if (this->player) // safe if for checking if player holds value
		{
			this->player->playerupdate(dt, this->window->getSize()); // calling the player controls
			this->gameball.update(dt, this->player->getposition()); // calling the ball logic
			bool wasbringin = (this->doga->getstate() == dog::dogstate::Bring); // the safe-bool for the dog bringin the ball
			this->doga->dogupdate(dt, this->player->getposition(), this->gameball.getState(), this->gameball.getposition()); // calling the dog AI
			if (this->doga->getstate() == dog::dogstate::Bring) // in case if dog is in bring mode the ball is attached to it
			{
				this->gameball.setpos(this->doga->getPosition());
			}

			if (wasbringin && this->doga->getstate() == dog::dogstate::Idle) // the safe if to make the increment of the score and resetting the ball easier
			{
				this->gameball.setstate(ball::ballstate::Held);
				this->gameball.setpos(this->player->getposition());
				this->glogic.plusScore();
			}
			this->menu.updateHud(this->glogic.getCurrentCatches(), this->glogic.getDesBalls()); // perma updating the HUD
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Subtract))
			{
				this->menu.currentvolume -= 25.f * dt;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Add))
			{
				this->menu.currentvolume += 25.f * dt;
			}
			this->menu.currentvolume = std::clamp(this->menu.currentvolume, 0.f, 100.f);
			sf::Listener::setGlobalVolume(this->menu.currentvolume);
		}
	}
	else // if ur in the menu(obv)
	{
		this->menu.updatemenu(*this->window);
	}
}

void gameeng::render() // Clears the current frame and updates it based on the boolean(isplay) state
{
	this->window->clear(sf::Color::Black);
	if (!isplay) // if not in the game mod draw the menu box
	{
		menu.draw(*this->window);
	}
	else
	{
		// here we draw the objects which are supposed to be visible on the screen
		this->menu.renderHud(*this->window, this->glogic);
		this->player->drawplayer(*this->window);
		this->gameball.drawball(*this->window);
		this->doga->dogdraw(*this->window);
		if (this->glogic.returnState()) // in case if the win condition was met
		{
			this->menu.renderwinhud(*this->window);
		}
		
	}
	if (this->menu.currentstate == GameMenu::MenuState::optionsmenu)
	{
		this->window->draw(this->previewlist.previewsprite);
	}
	this->window->display();
}
void gameeng::chktext()
{
	std::vector<textchest> gametextures = {
		{this->player->texture, "Textures/Sparxie.png"}, {doga->dogtext, "Textures/DogIdle1.png"}
	};

	isfileloaded(gametextures);

}



void playermodel::applyskin(const sf::Texture& tmp)
{
	this->texture = tmp;
	this->sprite.setTexture(this->texture, true);
}

void gameeng::updateskin()
{
	const sf::Texture* currtxt = this->previewlist.getskinindex();
	if (currtxt != nullptr)
	{
		this->player->applyskin(*currtxt);
	}
}

// prime execution loop
void gameeng::run() 
{
	while (window->isOpen())
	{
		sf::Clock cl; // we set clock for the calculations
		while (this->getWindowIsOpen())
		{
			float dt = cl.restart().asSeconds();
			this->update(dt);
			this->render();	
		}
	}
}