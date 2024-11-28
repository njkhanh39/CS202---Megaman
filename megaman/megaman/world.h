#pragma once

#include "obstacle.h"
#include "character.h"
#include "enemy.h"
#include "map.h"
#include <typeinfo>

class World {
public:
	TextureManager* textureManager;
	RectangleShape* background;

	//-----Game stuffs------//

	Map* gameMap;

	ShooterEnemy* enemy[30] = {nullptr};
	Obstacle* platform[30] = {nullptr};

	int numEnemy = 0, numObs = 0;

	World(TextureManager* textureManager);

	~World();

	void Render(RenderWindow* l_window);

	
	//Character + all entities in world :>
	void HandleAllEntitiesCollisions(Character* character) {

		//-----------Projectiles------------------

		//char + enes
		HandleAllProjectileCollisions(character);

		//------------Non-projectiles-------------

		HandleOneEntityCollision(character);

		for (int i = 0; i < 30; ++i) {
			if (enemy[i]) {
				HandleOneEntityCollision(enemy[i]);
			}
		}
	}
	

	void UpdateAllEnemies(Character* character, float delt) {
		for (int i = 0; i < 30; ++i) {
			if (enemy[i] && !enemy[i]->IsDead()) {
				enemy[i]->Update(character, delt);
			}
		}
	}

private:

	//helpers

	void HandleAllProjectileCollisions(Character* character) {

		//-----------Character----------------

		//for loops through all obstacles

		if (!character->IsDead()) {

			for (int i = 0; i < 30; ++i) {
				if (platform[i]) {
					character->HandleProjectileCollision(platform[i]);
				}
			}

			//for loops through all entities but itself

			for (int i = 0; i < 30; ++i) {
				if (enemy[i] && !enemy[i]->IsDead()) {
					character->HandleProjectileCollision(enemy[i]);
				}
			}
		}
		//-----------Enemy----------------

		//for loops through all obstacles **WE CAN OPTIMIZE THIS OFC**
		for (int i = 0; i < 30; ++i) {
			if (!enemy[i]) continue;

			for (int j = 0; j < 30; ++j) {
				if (!platform[j]) continue;

				enemy[i]->HandleProjectileCollision(platform[j]);
			}
		}

		//consider character only, we dont want enemies to hit one another
		if (!character->IsDead()) {
			for (int i = 0; i < 30; ++i) {
				if (enemy[i]) enemy[i]->HandleProjectileCollision(character);
			}
		}
	}

	void HandleOneEntityCollision(Entity* en) {
		if (en->IsDead()) return;
		//obstacles: wall, platform,.... 

		//check obstacle left-touch = character-right touch

		bool yesLeft = true, yesRight = true, yesFall = true;

		bool headBlock = false;

		for (int i = 0; i < 30; ++i) {
			//dont forget this
			if (!platform[i]) continue;


			if (!en->canMoveRight(platform[i])) yesRight = false;
			if (!en->canMoveLeft(platform[i])) yesLeft = false;
			if (!en->canKeepFalling(platform[i])) yesFall = false;

			//slightly different, hehe
			if (en->isHeadBlocked(platform[i])) headBlock = true;
		}

		en->right = yesRight; //can move right
		en->left = yesLeft; //can move left
		if (yesFall) {
			en->fall = true; //can fall
		}
		else {
			en->fall = false;
			//allow jump again! Resetting!
			en->isJumping = false;
		}

		//head blocked
		if (headBlock) en->setVelocityY(10); //gets head blocked


		//wall grab - jump - only for char
		if (dynamic_cast<Character*>(en)) {
			if ((!en->left || !en->right) && en->fall) {
				en->isJumping = false;
			}
		}
	}

	//create new enemies

	void CreateShooterEnemy(float x, float y, bool canMove, float movingRange
	, float viewRange) {
		if (numEnemy >= 30) return;
		enemy[numEnemy] = new ShooterEnemy(textureManager, x, y, canMove, movingRange, viewRange);
		++numEnemy;
	}

	void CreatePlatform(float x, float y, float xSize, float ySize) {
		if (numObs >= 30) return;

		platform[numObs] = new Obstacle(textureManager, Vector2f(x,y), Vector2f(xSize, ySize), true);
		++numObs;
	}
};