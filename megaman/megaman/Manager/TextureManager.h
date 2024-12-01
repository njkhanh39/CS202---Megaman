#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <SFML/Graphics.hpp>

class TextureManager {
private:
	std::map<std::string, sf::Texture*> textureMap;
	std::map<std::string, int> countMap; //count number of use
public:
	TextureManager() { 
		InnitCharacter(); 
		InnitMap();
	}

	~TextureManager() {
		for (auto& x : textureMap) {
			delete x.second;
		}
	}

	void InnitCharacter() { //we actuall dont even need all these
		this->AddTexture("Animation\\X\\X_IdleLeft.png");
		this->AddTexture("Animation\\X\\X_IdleRight.png");
		this->AddTexture("Animation\\X\\X_JumpLeft.png");
		this->AddTexture("Animation\\X\\X_JumpRight.png");

		this->AddTexture("Animation\\X\\X_ShootLeft.png");
		this->AddTexture("Animation\\X\\X_ShootRight.png");
		this->AddTexture("Animation\\X\\X_ShootJumpLeft.png");
		this->AddTexture("Animation\\X\\X_ShootJumpRight.png");
	}

	void InnitMap() { //dont even need
		//this->AddTexture("Animation\\highway.png");
	}

	void AddTexture(const std::string& name) {
		if (!textureMap[name]) {
			textureMap[name] = new sf::Texture();
			textureMap[name]->loadFromFile(name);
		}
	}

	void BorrowTexture(const std::string& name, sf::Texture*& borrowPtr) {
		if (!textureMap[name]) this->AddTexture(name);

		//Borrow
		borrowPtr = textureMap[name];

		//Somebody uses this
		++countMap[name];
	}

	int UsedTimes(const std::string& name) {
		return countMap[name];
	}

	void ShowAllUses() {
		for (auto& x : textureMap) {
			std::cout << "File: " << x.first << '\n';
			std::cout << "Used: " << countMap[x.first] << " times \n\n";
		}
	}

	//getters

	sf::Vector2u getTextureSize(const std::string& name) {
		if (!textureMap[name]) return { 0,0 };

		return textureMap[name]->getSize();
	}
};

#endif


