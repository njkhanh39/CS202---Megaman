#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "obstacle.h"
#include "enumdirection.h"

using namespace sf;

class Entity {
protected:
	RectangleShape frame;
	Sprite sprite;

	Texture* texture;
	
	

	float gravity = 980.0f, slowGravity = 10.0f, jumpStrength = 600.0f;
	float velocityY = 0.0f, velocityX = 400;
public:


	Direction direction;

	//moving permission flags
	bool left = true, right = true, fall = true;


	//current action flags

	bool isLeft = false;
	bool isRight = false;
	bool isShooting = false;
	bool isJumping = false;
	bool isGrabbing = false;

	Entity();

	Entity(float x, float y);

	~Entity();



	virtual void Render(RenderWindow* l_window);


	//movements

	void MoveLeft(float delt);

	void MoveRight(float delt);

	void Jump(float delt);

	virtual void Shoot();


	//collision checks

	bool canMoveRight(Obstacle* obs);

	bool canMoveLeft(Obstacle* obs);

	virtual bool canKeepFalling(Obstacle* obs);

	bool isHeadBlocked(Obstacle* obs);



	//getters


	Vector2f getUpLeftPosition();

	Vector2f getUpRightPosition();

	Vector2f getDownLeftPosition();

	Vector2f getDownRightPosition();

	float getRightMostX();

	float getLeftMostX();

	float getUpMostY();

	float getDownMostY();

	//setters

	void setSpritePosition(Vector2f pos) {
		sprite.setPosition(pos);
	}

	void setSize(Vector2f size) {
		frame.setSize(size);
	}

	void setVelocityY(float _y) {
		velocityY = _y;
	}
};