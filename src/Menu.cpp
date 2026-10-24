#include "GameMenu.h"
#include <string>

// initialising the parameters of the buttons and the texts
GameMenu::GameMenu() : font("fontforgame.ttf"), newgame(font), exitgame(font), settings(font), dogsprite(dogtexture), undobutton(font), volumetracker(font), hudtext(font), targetBalls(font), win(font), nextskin(font), prevskin(font), titlesprite(titletexture)  // creating the buttons and dog anim
{
	newgame.setString("New Game");
	newgame.setCharacterSize(16);
	newgame.setStyle(sf::Text::Regular);
	newgame.setPosition(sf::Vector2f(70.f, 200.f));

	exitgame.setString("Exit game");
	exitgame.setCharacterSize(16);
	exitgame.setStyle(sf::Text::Regular);
	exitgame.setPosition(sf::Vector2f(70.f, 300.f));

	settings.setString("Options");
	settings.setCharacterSize(16);
	settings.setStyle(sf::Text::Regular);
	settings.setPosition(sf::Vector2f(70.f, 250.f));

	undobutton.setString("Back");
	undobutton.setCharacterSize(24);
	undobutton.setStyle(sf::Text::Regular);
	undobutton.setPosition(sf::Vector2f(70.f, 500.f));

	volumetracker.setString("Volume :" + std::to_string(static_cast<int>(this->currentvolume)) + "%");
	volumetracker.setCharacterSize(16);
	volumetracker.setStyle(sf::Text::Regular);
	volumetracker.setPosition(sf::Vector2f(70.f, 400.f));

	targetBalls.setCharacterSize(16);
	targetBalls.setFillColor(sf::Color::White);
	targetBalls.setPosition(sf::Vector2f(70.f, 300.f));

	hudtext.setCharacterSize(24);
	hudtext.setFillColor(sf::Color::White);
	hudtext.setPosition(sf::Vector2f(20.f, 20.f));

	win.setCharacterSize(24);
	win.setFillColor(sf::Color::White);
	win.setPosition(sf::Vector2f(20.f, 50.f));
	win.setString("You win! \n R for restart!");

	nextskin.setCharacterSize(16);
	nextskin.setFillColor(sf::Color::White);
	nextskin.setScale({ 2.f, 2.f });
	nextskin.setPosition(sf::Vector2f(520.f, 200.f));
	nextskin.setString(">");

	prevskin.setCharacterSize(16);
	prevskin.setFillColor(sf::Color::White);
	prevskin.setScale({ 2.f, 2.f });
	prevskin.setPosition(sf::Vector2f(320.f, 200.f));
	prevskin.setString("<");

	if (titletexture.loadFromFile("Textures/Title.png"))
	{
		
		titlesprite.setTexture(titletexture,true);
		titlesprite.setPosition({0.f,2.f});
		sf::Vector2u txtsize;
		txtsize = titletexture.getSize();
		if (txtsize.x > 0 && txtsize.y > 0)
		{
			titlesprite.setScale({ 800.f / txtsize.x, 150.f / txtsize.y });
		}
	}

	if (dogtexture.loadFromFile("DogMenu.png")) // here we load the chopped animation of the dog for the menu
	{
		dogsprite.setTextureRect(sf::IntRect({ 0,0 }, { 256, 256 }));
		dogsprite.setScale({ 0.25f, 0.25f });
		dogsprite.setPosition({ 700.f, 400.f });
	}

	if (!buf.loadFromFile("ButtonSound.wav")) // safecheck
	{
		std::cerr << "FAILED TO LOAD THE FILE";
	}
	else
	{
		this->hoversound.emplace(this->buf); // bind the buffer sound
		this->hoversound->setVolume(this->currentvolume);
	}
}

// In here we set the logic of the menu like filling the other color or playing a hover sound

void GameMenu::hovermenu(const sf::Vector2f& mousepos) 
{
	if (this->currentstate == MenuState::mainmenu)
	{
		bool hoverovernewgame = this->newgame.getGlobalBounds().contains(mousepos);
		bool hoveroverexit = this->exitgame.getGlobalBounds().contains(mousepos);
		bool hoveroversettings = this->settings.getGlobalBounds().contains(mousepos);

		if (hoverovernewgame)
		{
			this->newgame.setFillColor(sf::Color::Red);
			this->newgame.setOutlineColor(sf::Color::White);
		}
		else
		{
			this->newgame.setFillColor(sf::Color::White);
			this->newgame.setOutlineColor(sf::Color::Black);
		}
		if (hoveroverexit)
		{
			this->exitgame.setFillColor(sf::Color::Red);
			this->exitgame.setOutlineColor(sf::Color::White);
		}
		else
		{
			this->exitgame.setFillColor(sf::Color::White);
			this->exitgame.setOutlineColor(sf::Color::Black);
		}
		if (hoveroversettings)
		{
			this->settings.setFillColor(sf::Color::Red);
			this->settings.setOutlineColor(sf::Color::White);
		}
		else
		{
			this->settings.setFillColor(sf::Color::White);
			this->settings.setOutlineColor(sf::Color::Black);
		}

		if (hoveroverexit || hoverovernewgame || hoveroversettings)
		{
			if (!this->isHovered)
			{
				if (this->hoversound.has_value())
				{
					this->hoversound->play();
				}
				this->isHovered = true;
			}
			
		}
		else
		{
			this->isHovered = false;
			this->undobutton.setFillColor(sf::Color::White);
			this->undobutton.setOutlineColor(sf::Color::Black);
			
		}
		
	}
	if (currentstate == MenuState::optionsmenu)
	{
		bool hoveroverundo = this->undobutton.getGlobalBounds().contains(mousepos);
		bool hoverovernextskin = this->nextskin.getGlobalBounds().contains(mousepos);
		bool hoveroverprevskin = this->prevskin.getGlobalBounds().contains(mousepos);
		if (hoveroverundo)
		{
			this->undobutton.setFillColor(sf::Color::Red);
			this->undobutton.setOutlineColor(sf::Color::White);
		}
		else
		{
			this->undobutton.setFillColor(sf::Color::White);
			this->undobutton.setOutlineColor(sf::Color::Black);
		}
		if (hoverovernextskin)
		{
			this->nextskin.setFillColor(sf::Color::Red);
			this->nextskin.setOutlineColor(sf::Color::White);
		}
		else
		{
			this->nextskin.setFillColor(sf::Color::White);
			this->nextskin.setOutlineColor(sf::Color::Black);
		}
		if (hoveroverprevskin)
		{
			this->prevskin.setFillColor(sf::Color::Red);
			this->prevskin.setOutlineColor(sf::Color::White);
		}
		else
		{
			this->prevskin.setFillColor(sf::Color::White);
			this->prevskin.setOutlineColor(sf::Color::Black);
		}
		if (hoveroverundo || hoverovernextskin || hoveroverprevskin)
		{
			if (!this->isHovered)
			{
				if (this->hoversound.has_value())
				{
					this->hoversound->play();
				}
				this->isHovered = true;
			}
		}
		else
		{
			this->isHovered = false;
		}
	}
}
// The animation of the dog in the corner
void GameMenu::updatemenu(sf::RenderWindow& win)
{
	if (animclock.getElapsedTime().asSeconds() >= animdur)
	{
		currentframe = (currentframe + 1) % totalframes;
		int leftof = currentframe * 256;
		dogsprite.setTextureRect(sf::IntRect({ leftof, 0 }, { 256, 256 }));
		animclock.restart();
	}

	sf::Vector2i pixelpos = sf::Mouse::getPosition(win);
	sf::Vector2f mousepos = win.mapPixelToCoords(pixelpos);
	
	this->hovermenu(mousepos);
	
}