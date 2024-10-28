#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;

class Obstacle {
private:
	//position of texture's boundary
	Vector2f position;

	//size of texture's boundary
	Vector2f size;
	RectangleShape* frame; //boundary for the texture
	Texture* texture;
	Sprite* sprite;
public:
	Obstacle();
	Obstacle(Vector2f pos, Vector2f siz);
	~Obstacle();

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

	void loadImage(const std::string& file);
	//others
	
	void Render(RenderWindow* l_window);
};