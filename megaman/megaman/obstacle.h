#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Manager/TextureManager.h"

using namespace sf;

class Obstacle {
private:
	//position of texture's boundary
	Vector2f position;

	//size of texture's boundary
	RectangleShape* frame; //boundary for the texture


	TextureManager* textureManager; //dont delete
	Texture* texture;
	Sprite* sprite;

	bool invisible = false;
	int damage = 0;
public:

	Obstacle(TextureManager* textureManager, Vector2f pos, Vector2f siz);

	Obstacle(TextureManager* textureManager, Vector2f pos, Vector2f siz, bool invisible);

	Obstacle(TextureManager* textureManager, Vector2f pos, Vector2f siz, bool invisible, int damage);

	Obstacle(TextureManager* textureManager, Vector2f pos, const std::string& file, int damage);

	Obstacle(TextureManager* textureManager, Vector2f pos, const std::string& file);
	~Obstacle();


	int getDamage() {
		return damage;
	}

	//getters

	Vector2f getPosition();

	Vector2f getSize();

	Vector2f getUpLeftPosition();

	Vector2f getUpRightPosition();

	Vector2f getDownLeftPosition();

	Vector2f getDownRightPosition();

	float getRightMostX();

	float getLeftMostX();

	float getUpMostY();

	float getDownMostY();

	//setters

	void setPosition(Vector2f pos);

	//can only setSize of sprite is not loaded
	void setSize(Vector2f siz);
	//set color incase we dont load sprite
	void setColor(Color col);

	//others
	
	void Render(RenderWindow* l_window);
};