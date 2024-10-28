#pragma once

#include "obstacle.h"
#include "character.h"

class World {
public:
	Obstacle *wall;
	Obstacle* platform[1];

	World();

	~World();

	void Render(RenderWindow* l_window);

	void HandleProjectileCollision(Character* character) {
		character->HandleProjectileCollision(wall);
		character->HandleProjectileCollision(platform[0]);
	}

	//called during loop

	void HandleCharacterCollision(Character* character) {
		
		//obstacles: wall, platform,.... 

		//check obstacle left-touch = character-right touch

		bool yesLeft = true, yesRight = true, yesFall = true;

		bool headBlock = false;

		for (int i = 0; i < 1; ++i) {
			if (!character->canMoveRight(platform[i])) yesRight = false;
			if (!character->canMoveLeft(platform[i])) yesLeft = false;
			if (!character->canKeepFalling(platform[i])) yesFall = false;
			if (character->isHeadBlocked(platform[i])) headBlock = true;
		}

		if (yesRight && character->canMoveRight(wall)) {
			character->right = true;
		}
		else character->right = false;
		

		if (yesLeft && character->canMoveLeft(wall)) {
			character->left = true;
		}
		else character->left = false;

		//fall and jump
		if (yesFall && character->canKeepFalling(wall)) {
			character->fall = true;
		}
		else {
			character->fall = false;
			//allow jump again! Resetting!
			character->isJumping = false;
		}
		
		//wall grab - jump

		if ((!character->left || !character->right) && character->fall) {
			character->isJumping = false;
		}

		//head blocked

		if (headBlock || character->isHeadBlocked(wall)) {
			character->setVelocityY(10);
		}
		
	}

private:
	
};