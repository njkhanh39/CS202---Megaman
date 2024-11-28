#pragma once
#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <SFML/Graphics.hpp>
#include "Manager/TextureManager.h"
class Map {
private:
	sf::Texture* mapTexture;
	sf::Sprite* mapSprite;
	
	TextureManager* manager;
public:
	Map(TextureManager* manager, const std::string& filemap): manager(manager) {
		
		this->manager->BorrowTexture(filemap, mapTexture);

		
		this->mapSprite = new sf::Sprite();
		if (this->mapTexture) {
			this->mapSprite->setTexture(*mapTexture);
		}
	}

	~Map() {
		//dont delete texture because we borrow it
		delete mapSprite;
	}


	void Render(RenderWindow* target) {
		if(mapSprite) target->draw(*mapSprite);
	}
};