#pragma once
#include <SFML/System.hpp>
#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>
#include <vector> // best to store textures in the vectors
#include <optional>
class SkinSelector
{
public:
	
	std::vector<sf::Texture> listoftextures;
	sf::Texture previewskin;
	sf::Sprite previewsprite;

	size_t currentskinindex = 0;
	
	SkinSelector() : previewskin(), previewsprite(previewskin)
	{
	};

	void checkskins()
	{
		listoftextures.clear();
		listoftextures.reserve(3);
		previewsprite.setPosition({ 385.f, 150.f });
		previewsprite.setScale({2.5f,2.5f});
		sf::Texture tmp;
		if (tmp.loadFromFile("Textures/male.png"))
		{
			listoftextures.push_back(tmp);
			
		}
		if (tmp.loadFromFile("Textures/girl.png"))
		{
			listoftextures.push_back(tmp);
			
		}
		if (tmp.loadFromFile("Textures/Sparxie.png"))
		{
			listoftextures.push_back(tmp);
			
		}
		if (!listoftextures.empty())
		{
			currentskinindex = 0;
			previewsprite.setTexture(listoftextures[currentskinindex], true);
		}
	}
	void nextskin()
	{
		if (listoftextures.empty()) { return; }
		
			currentskinindex = (currentskinindex + 1) % listoftextures.size();
			previewsprite.setTexture(listoftextures[currentskinindex], true);
		
	}
	void prevskin()
	{
		if (listoftextures.empty()) { return; }
		
			if(currentskinindex == 0)
			{
				currentskinindex = listoftextures.size() - 1;
			}
			else
			{
				currentskinindex--;
			}
			previewsprite.setTexture(listoftextures[currentskinindex], true);	
	}
	const sf::Texture* getskinindex() const
	{
		if (listoftextures.empty()) return nullptr;
		return &listoftextures[currentskinindex];
	}
};