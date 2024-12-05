#pragma once

#include "obstacle.h"
#include "character.h"
#include "attackenemies.h"
#include "map.h"
#include <fstream>
#include <sstream>
#include <string.h>
#include <typeinfo>


class World {
public:
	const int MAXPLATFORMS = 30;
	const int MAXENEMIES = 30;


	TextureManager* textureManager;
	RectangleShape* background;

	//-----Game stuffs------//

	Map* gameMap;

	Enemy* enemy[30] = {nullptr};


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

		for (int i = 0; i < numEnemy; ++i) {
			if (enemy[i]) {
				HandleOneEntityCollision(enemy[i]);
			}
		}

		//--------------CHAR VS ENEMY COLLISION------------//

		//they can pass through each other, but char loses health

		HandleCharacterVsEnemyCollisions(character);
	}
	

	void UpdateAllEnemies(Character* character, float delt) {
		for (int i = 0; i < numEnemy; ++i) {
			if (enemy[i] && !enemy[i]->IsDead()) {
				enemy[i]->Update(character, delt);
			}
		}
	}

private:

	//helpers

	void HandleCharacterVsEnemyCollisions(Character* character) {
		if (character->IsDead()) return;

		for (int i = 0; i < numEnemy; ++i) {
			if (!enemy[i] || enemy[i]->IsDead()) continue;

			if (!character->canMoveLeft(enemy[i])) 
			{ //still let it move through, but take damage
				if (!character->isInvisible()) {
					character->TakeDamage(enemy[i]->GetCollisionDamage());
					character->MoveRight(0.075f);
				}
			}
			else if (!character->canMoveRight(enemy[i])) {
				if (!character->isInvisible()) {
					character->TakeDamage(enemy[i]->GetCollisionDamage());
					character->MoveLeft(0.075f);
				}
			}
			else if (character->isHeadBlocked(enemy[i])) {
				if(!character->isInvisible()) character->TakeDamage(enemy[i]->GetCollisionDamage());
			}
			else if (!character->Entity::canKeepFalling(enemy[i])) {	
				if (!character->isInvisible()) {
					character->TakeDamage(enemy[i]->GetCollisionDamage());
					character->PushedUpward(5.0f);
				}
			}
		}
	}

	void HandleAllProjectileCollisions(Character* character) {

		//-----------Character----------------

		//for loops through all obstacles

		if (!character->IsDead()) {

			for (int i = 0; i < numObs; ++i) {
				if (platform[i]) {
					character->HandleProjectileCollision(platform[i]);
				}
			}

			//for loops through all entities but itself

			for (int i = 0; i < numEnemy; ++i) {
				if (enemy[i] && !enemy[i]->IsDead()) {
					character->HandleProjectileCollision(enemy[i]);
				}
			}
		}
		//-----------Enemy----------------

		//for loops through all obstacles **WE CAN OPTIMIZE THIS OFC**

		//Enemy shooting platform
		for (int i = 0; i < numEnemy; ++i) {
			if (!enemy[i]) continue;

			for (int j = 0; j < numObs; ++j) {
				if (!platform[j]) continue;

				enemy[i]->HandleProjectileCollision(platform[j]);
			}
		}

		//Enemy shooting character
		if (!character->IsDead()) {
			for (int i = 0; i < numEnemy; ++i) {
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

		for (int i = 0; i < numObs; ++i) {
			//dont forget this
			if (!platform[i]) continue;


			if (!en->canMoveRight(platform[i])) {
				yesRight = false;
				en->TakeDamage(platform[i]->getDamage());
			}
			if (!en->canMoveLeft(platform[i])) {
				yesLeft = false;
				en->TakeDamage(platform[i]->getDamage());
			}
			if (!en->canKeepFalling(platform[i])) {
				yesFall = false;
				en->TakeDamage(platform[i]->getDamage());
			}

			//slightly different, hehe
			if (en->isHeadBlocked(platform[i])) {
				headBlock = true;
				en->TakeDamage(platform[i]->getDamage());
			}
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

	//-----TEST--------//

	void CreateShooterEnemy1(float x, float y) {
		if (numEnemy >= MAXENEMIES) return;
		enemy[numEnemy] = new ShooterEnemy1(textureManager, x, y);
		++numEnemy;
	}

	void CreateAttackEnemy1(float x, float y, Direction dir) {
		if (numEnemy >= MAXENEMIES) return;
		enemy[numEnemy] = new AttackEnemy1(textureManager, x, y, dir);
		++numEnemy;
	}

	//create new enemies

	void CreateShooterEnemy(float x, float y,
	 float viewRange) {
		if (numEnemy >= MAXENEMIES) return;
		//enemy[numEnemy] = new ShooterEnemy(textureManager, x, y, viewRange);
		++numEnemy;
	}

	void CreateShooterEnemy(float x, float y, float sizex, float sizey, float sizebulletx, float sizebullety
		, float viewRange) {
		if (numEnemy >= MAXENEMIES) return;
		//enemy[numEnemy] = new ShooterEnemy(textureManager, x, y,sizex, sizey, sizebulletx, sizebullety, viewRange);
		++numEnemy;

		//std::cout << "Enemy health: " << enemy[numEnemy - 1]->getHealth() << '\n';
	}

	void CreatePlatform(float x, float y, float xSize, float ySize, int damage, bool invisible) {
		if (numObs >= MAXPLATFORMS) return;

		platform[numObs] = new Obstacle(textureManager, Vector2f(x,y), Vector2f(xSize, ySize), invisible, damage);
		++numObs;
	}

	void CreatePlatform(float x, float y, const std::string& file) {
		if (numObs >= MAXPLATFORMS) return;

		platform[numObs] = new Obstacle(textureManager, Vector2f(x, y), file);
		++numObs;
	}

	void CreatePlatform(float x, float y, int damage, const std::string& file) {
		if (numObs >= MAXPLATFORMS) return;

		platform[numObs] = new Obstacle(textureManager, Vector2f(x, y), file, damage);
		++numObs;
	}

	void CreateWorld(const std::string& dir, const std::string& mapfile, const std::string& obs1file, const std::string& obs2file) {
		//------MAP-------//
		this->gameMap = new Map(textureManager, dir+mapfile);
		//------INVISIBLE OBS----------//
		this->LoadInvisibleObstacles(dir+obs1file);

		//------ADDITIONAL / SPECIAL OBS-----
		this->LoadSpecialObstacles(dir, obs2file);
	}

private:

	//helpers

	void LoadInvisibleObstacles(const std::string& file) {
		std::ifstream fin;

		fin.open(file);

		if (!fin.is_open()) {
			std::cout << "Error opening invisible obstacles from " << file << '\n';
			return;
		}

		//--------

		int n = -1;

		fin >> n;
		if (n+this->numObs > MAXPLATFORMS) {
			std::cout << "Too many platforms!\n";
			fin.close();
			return;
		}

		std::string line;
		std::getline(fin, line);

		while (std::getline(fin, line)) {
			std::stringstream sstr(line);
			std::vector<int> coords;

			std::string field;
			while (std::getline(sstr, field, ',')) {
				coords.push_back(std::stoi(field));
			}

			if ((int)coords.size() == 5) {
				this->CreatePlatform(coords[0], coords[1], coords[2], coords[3], coords[4], true);
			}
		}


		//--------
		fin.close();
	}


	void LoadSpecialObstacles(const std::string& dir, const std::string& obs2) {
		std::ifstream fin;

		fin.open(dir+obs2);

		if (!fin.is_open()) {
			std::cout << "Error opening special obstacles from " << dir + obs2 << '\n';
			return;
		}

		//--------

		int n = -1;

		fin >> n;
		if (n + this->numObs> MAXPLATFORMS) {
			std::cout << "Too many platforms!\n";
			fin.close();
			return;
		}

		std::string line;
		std::getline(fin, line);

		while (std::getline(fin, line)) {
			std::stringstream sstr(line);
			std::vector<int> coords;

			std::string field;
			while (std::getline(sstr, field, ',')) {
				coords.push_back(std::stoi(field));
			}

			if ((int)coords.size() == 4) {
				this->CreatePlatform(coords[0], coords[1],coords[3], dir + std::to_string(coords[2]) + ".png");
			}
		}


		//--------
		fin.close();
	}
};