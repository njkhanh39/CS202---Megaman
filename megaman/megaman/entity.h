#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "obstacle.h"
#include "animation.h"
#include "enumdirection.h"
#include <algorithm>
//#include "Manager/TextureManager.h"

using namespace sf;

class Entity {
protected:
	TextureManager* textureManager; //dont delete

	//--------Basic shapes--------

	RectangleShape frame;
	Sprite sprite;


	//---------Textures---------- //These will be borrowed from texture manager

	Texture* texture_idle_left, * texture_idle_right, * texture_jump_left, * texture_jump_right;

	Texture* texture_shoot_left, * texture_shoot_right, * texture_shoot_jump_left, * texture_shoot_jump_right;


	//---------Animation----------
	
	AnimationComponent* movingAnimation;


	//---------Physic variables----------------------

	float gravity = 980.0f, slowGravity = 10.0f, jumpStrength = 300.0f;
	float velocityY = 0.0f, velocityX = 120;

	int health = 50;
public:
	//direction
	Direction direction;

	//moving permission flags
	bool left = true, right = true, fall = true;


	//current action flags

	bool isLeft = false;
	bool isRight = false;
	bool isShooting = false;
	bool isJumping = false;
	bool isGrabbing = false;

	//---X only----
	bool isChargeShooting = false;

	//--------Constructors and Destructor-------------

	Entity(TextureManager* textureManager);

	Entity(TextureManager* textureManager, float x, float y);

	Entity(TextureManager* textureManager, float x, float y, float width, float height);

	////Perfect deep copy
	////NOTE2: UPDATE COPY CONSTRUCTOR WHENEVER YOU ADD NEW ATTRIBUTES TO CLASS
	Entity(const Entity& other): frame(other.frame), sprite(other.sprite), gravity(other.gravity), 
	slowGravity(other.slowGravity), jumpStrength(other.jumpStrength), velocityX(other.velocityX),
	velocityY(other.velocityY), textureManager(other.textureManager), health(other.health){

		direction = other.direction;
		left = other.left;
		right = other.right;
		fall = other.fall;

		isLeft = other.isLeft;
		isRight = other.isRight;
		isShooting = other.isShooting;
		isJumping = other.isJumping;
		isGrabbing = other.isGrabbing;


		texture_idle_left = other.texture_idle_left;
		texture_idle_right = other.texture_idle_right;
		texture_jump_left = other.texture_jump_left;
		texture_jump_right = other.texture_jump_right;

		texture_shoot_left = other.texture_shoot_left;
		texture_shoot_right = other.texture_shoot_right;
		texture_shoot_jump_left = other.texture_shoot_jump_left;
		texture_shoot_jump_right = other.texture_shoot_jump_right;


		//Invoke "shallow copy on sprite, deep copy on others"
		movingAnimation = new AnimationComponent(*other.movingAnimation);

		//Aftert that, we change animation sprite*

		movingAnimation->SetAllAnimationsSprite(&sprite);
		//==> Deep copy now works perfectly
	}

	virtual ~Entity(); 

	//void CreateTextures();

	void InnitNullTextures();
	
	void CreateAnimationComponent();

	//-------------Render-----------------------------

	virtual void Render(RenderWindow* l_window);


	//-------------Actions---------------------------

	void MoveLeft(float delt);

	void MoveRight(float delt);

	void Jump(float delt);

	virtual void Shoot();

	void TurnLeft();

	void TurnRight();

	void FaceDirection(Direction dir);

	//maybe different for each
	virtual void Die() {
		this->~Entity();
	}

	//------------Collision checks----------------------

	bool canMoveRight(Obstacle* obs);

	bool canMoveLeft(Obstacle* obs);

	virtual bool canKeepFalling(Obstacle* obs);

	bool isHeadBlocked(Obstacle* obs);

	//-------------------------------------------------

	//updates
	void UpdateMovements(float delt);

	

	//helper update
	bool IsDead();

	void TakeDamage(int damage);


	//getters
	Vector2f getPosition();

	Vector2f getCenterPosition();

	Vector2f getUpLeftPosition();

	Vector2f getUpRightPosition();

	Vector2f getDownLeftPosition();

	Vector2f getDownRightPosition();

	float getRightMostX();

	float getLeftMostX();

	float getUpMostY();

	float getDownMostY();

	Vector2f getFrameSize();

	//setters

	void setSpriteScale(float s1, float s2);

	void setSpritePosition(Vector2f pos);

	void setSize(Vector2f size);

	void setVelocityY(float _y);

	void setVelocityX(float _x);

	void setGravity(float _g);

	virtual void setPosition(Vector2f pos);



	//helpers


};