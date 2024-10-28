#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "obstacle.h"
#include "animation.h"
#include "enumdirection.h"

using namespace sf;

class Entity {
protected:
	RectangleShape frame;
	Sprite sprite;


	//---------Textures----------
	Texture* texture_idle_left, * texture_idle_right, * texture_jump_left, * texture_jump_right;

	Texture* texture_shoot_left, * texture_shoot_right, * texture_shoot_jump_left, * texture_shoot_jump_right;

	Texture* texture_movement_right, * texture_movement_left, * texture_movement_shoot_right
		, * texture_movement_shoot_left;

	//---------Animation----------
	
	AnimationComponent* movingAnimation;

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


	void CreateAnimationComponent();

	virtual void Render(RenderWindow* l_window);


	//movements

	void MoveLeft(float delt);

	void MoveRight(float delt);

	void Jump(float delt);

	virtual void Shoot();

	void TurnLeft() {
		direction = Direction::Left;
		sprite.setTexture(*texture_idle_left, true);
	}

	void TurnRight() {
		direction = Direction::Right;
		sprite.setTexture(*texture_idle_right, true);
	}


	//collision checks

	bool canMoveRight(Obstacle* obs);

	bool canMoveLeft(Obstacle* obs);

	virtual bool canKeepFalling(Obstacle* obs);

	bool isHeadBlocked(Obstacle* obs);


	//updates
	void UpdateCharacter(float delt);


	//getters
	Vector2f getPosition() {
		return getUpLeftPosition();
	}

	Vector2f getCenterPosition();

	Vector2f getUpLeftPosition();

	Vector2f getUpRightPosition();

	Vector2f getDownLeftPosition();

	Vector2f getDownRightPosition();

	float getRightMostX();

	float getLeftMostX();

	float getUpMostY();

	float getDownMostY();

	Vector2f getFrameSize() {
		return frame.getSize();
	}

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

	virtual void setPosition(Vector2f pos) {
		frame.setPosition(pos);
		sprite.setPosition(pos);
	}

	//OOP stuffs


};