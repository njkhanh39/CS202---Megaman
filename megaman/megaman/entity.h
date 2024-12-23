#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "obstacle.h"
#include "animation.h"
#include "enumdirection.h"
#include <algorithm>
#include "Manager/SoundManager.h"

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
	AnimationComponent* idleAnimation;


	//---------Physic variables----------------------

	float gravity = 680.0f, slowGravity = 30.0f, jumpStrength = 250.0f;
	float velocityY = 0.0f, velocityX = 50;

	int health = 50;

	bool invisible = false;  // after taking a hit, entity may become invisible for a short period
	float invisibleTimer = 0.f;
	float invisibleMaxTimer = 100.f; //for character, this = 100.f


	//the distance between sprite and frame
	Vector2f dilation = { 0.f,0.f };
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
	velocityY(other.velocityY), textureManager(other.textureManager), health(other.health), invisible(other.invisible),
	invisibleTimer(other.invisibleTimer), invisibleMaxTimer(other.invisibleMaxTimer), dilation(other.dilation){

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
		idleAnimation = new AnimationComponent(*other.idleAnimation);

		//Aftert that, we change animation sprite*

		movingAnimation->SetAllAnimationsSprite(&sprite);
		idleAnimation->SetAllAnimationsSprite(&sprite);
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

	void PushedUpward(float delt);

	virtual void Shoot(float delt);

	void TurnLeft();

	void TurnRight();

	void FaceDirection(Direction dir);

	//maybe different for each
	virtual void Die() {
		std::cout << "Entity dies!\n";
		this->~Entity();
	}

	//------------Collision checks----------------------

	bool canMoveRight(Obstacle* obs);

	bool canMoveLeft(Obstacle* obs);

	bool canKeepFalling(Obstacle* obs);

	bool isHeadBlocked(Obstacle* obs);

	bool canMoveRight(Entity* en);

	bool canMoveLeft(Entity* en);

	bool canKeepFalling(Entity* en);

	bool isHeadBlocked(Entity* en);

	//-------------------------------------------------

	//updates
	virtual void UpdateEntity(float delt); //u can customize this

	bool isInvisible() {
		return this->invisible;
	}

	//helper update
	bool IsDead();

	virtual void TakeDamage(int damage);


	//getters
	virtual int getHealth() {
		return this->health;
	}

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

	FloatRect getFloatRect() {
		FloatRect res(frame.getPosition(), frame.getSize());
		return res;
	}

	RectangleShape getFrame() {
		return this->frame;
	}

	//setters

	void setIdleLeftTexture(const std::string& file) {
		textureManager->BorrowTexture(file, texture_idle_left);
	}

	void setIdleRightTexture(const std::string& file) {
		textureManager->BorrowTexture(file, texture_idle_right);
	}

	void setSpriteScale(float s1, float s2);

	void setSpritePosition(Vector2f pos);

	void setSize(Vector2f size);

	void setVelocityY(float _y);

	void setVelocityX(float _x);

	void setGravity(float _g);

	void setPosition(Vector2f pos);
	//helpers


};