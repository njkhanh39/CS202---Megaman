#pragma once

#include "obstacle.h"
#include "character.h"
#include "enemy.h"
#include <typeinfo>

class World {
public:
	ShooterEnemy* enemy;
	Obstacle *wall;
	Obstacle* platform[1];

	World();

	~World();

	void Render(RenderWindow* l_window);

	void HandleAllProjectileCollisions(Character* character) {

		//-----------Character----------------

		//for loops through all obstacles
		character->HandleProjectileCollision(wall);
		character->HandleProjectileCollision(platform[0]);

		//for loops through all entities but itself
		character->HandleProjectileCollision(enemy);

		//-----------Enemy----------------

		//for loops through all obstacles
		enemy->HandleProjectileCollision(wall);
		enemy->HandleProjectileCollision(platform[0]);

		//for loops through all entities but itself
		enemy->HandleProjectileCollision(character);
	}


	void HandleEntityCollision(Entity* en) {
		
		//obstacles: wall, platform,.... 

		//check obstacle left-touch = character-right touch

		bool yesLeft = true, yesRight = true, yesFall = true;

		bool headBlock = false;

		for (int i = 0; i < 1; ++i) {
			if (!en->canMoveRight(platform[i])) yesRight = false;
			if (!en->canMoveLeft(platform[i])) yesLeft = false;
			if (!en->canKeepFalling(platform[i])) yesFall = false;
			if (en->isHeadBlocked(platform[i])) headBlock = true;
		}

		if (yesRight && en->canMoveRight(wall)) {
			en->right = true;
		}
		else en->right = false;
		

		if (yesLeft && en->canMoveLeft(wall)) {
			en->left = true;
		}
		else en->left = false;

		//fall and jump
		if (yesFall && en->canKeepFalling(wall)) {
			en->fall = true;
		}
		else {
			en->fall = false;
			//allow jump again! Resetting!
			en->isJumping = false;
		}
		
		//wall grab - jump - only for char

		if (dynamic_cast<Character*>(en)) {

			if ((!en->left || !en->right) && en->fall) {
				en->isJumping = false;
			}
		}
		
		//head blocked

		if (headBlock || en->isHeadBlocked(wall)) {
			en->setVelocityY(10);
		}
		
	}

	void UpdateAllEnemies(Character* character, float delt) {
		enemy->UpdateCharacter(delt);

		enemy->UpdateEnemyBehaviour(character, delt);
		enemy->UpdateEnemyProjectiles(delt);
	}

private:
	
};