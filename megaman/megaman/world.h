#pragma once

#include "obstacle.h"
#include "character.h"
#include "attackenemies.h"
#include "map.h"
#include "enemyspawner.h"

#include <fstream>
#include <sstream>
#include <string.h>
#include <typeinfo>


class World {
public:
	const int MAXPLATFORMS = 80;
	const int MAXENEMIES = 80;


	TextureManager* textureManager;
	RectangleShape* background;

	//-----Game stuffs------//

	Map* gameMap;
	EnemySpawner* enemySpawner;

	std::vector<Enemy*> enemy;
	std::vector<Obstacle*> platform;

	const int boss_region_left = 6730;
	const int boss_region_right = 7080;

	bool isBossFight = false;
	bool lockBossRegion = false;

	//---------------------//


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

		for (int i = 0; i < (int)enemy.size(); ++i) {
			if (enemy[i]) {
				HandleOneEntityCollision(enemy[i]);
			}
		}

		//--------------CHAR VS ENEMY COLLISION------------//

		//they can pass through each other, but char loses health

		HandleCharacterVsEnemyCollisions(character);
	}
	

	void UpdateWorld(Character* character, float delt) {
		for (int i = 0; i < (int)enemy.size(); ++i) {
			if (enemy[i] && !enemy[i]->IsDead()) {
				enemy[i]->Update(character, delt);
			}
		}
		this->enemySpawner->UpdateSpawn(character);

		//---BOSS----//

		if (this->isBossFight && lockBossRegion==false) {
			LockBossZone();
		}
	}

	Vector2f GetBossRegion() {
		return Vector2f(boss_region_left, boss_region_right);
	}

	Boss* GetBossPointer() {
		if (enemy.empty()) return nullptr;

		if (dynamic_cast<Boss*>(enemy.back())) {
			return dynamic_cast<Boss*>(enemy.back());
		}

		return nullptr;
	}

private:

	//helpers

	void HandleCharacterVsEnemyCollisions(Character* character) {
		if (character->IsDead()) return;

		for (int i = 0; i < (int)enemy.size(); ++i) {
			if (!enemy[i] || enemy[i]->IsDead()) continue;

			if (!character->canMoveLeft(enemy[i])) 
			{ //still let it move through, but take damage
				if (!character->isInvisible()) {
					character->TakeDamage(enemy[i]->GetCollisionDamage());
					if(character->right) character->MoveRight(0.1f);
				}
			}
			else if (!character->canMoveRight(enemy[i])) {
				if (!character->isInvisible()) {
					character->TakeDamage(enemy[i]->GetCollisionDamage());
					if (character->left) character->MoveLeft(0.1f);
				}
			}
			else if (character->isHeadBlocked(enemy[i])) {
				if (!character->isInvisible()) {
					character->TakeDamage(enemy[i]->GetCollisionDamage());
					character->MoveLeft(0.1f);
				}
			}
			else if (!character->Entity::canKeepFalling(enemy[i])) {	
				if (!character->isInvisible()) {
					character->TakeDamage(enemy[i]->GetCollisionDamage());
					character->MoveLeft(0.1f);
				}
			}
		}
	}

	void HandleAllProjectileCollisions(Character* character) {

		//-----------Character----------------

		//for loops through all obstacles

		if (!character->IsDead()) {

			for (int i = 0; i < (int)platform.size(); ++i) {
				if (platform[i]) {
					character->HandleProjectileCollision(platform[i]);
				}
			}

			//for loops through all entities but itself

			for (int i = 0; i < (int)enemy.size(); ++i) {
				if (enemy[i] && !enemy[i]->IsDead()) {
					character->HandleProjectileCollision(enemy[i]);
				}
			}
		}
		//-----------Enemy----------------

		//for loops through all obstacles **WE CAN OPTIMIZE THIS OFC**

		//Enemy shooting platform
		for (int i = 0; i < (int)enemy.size(); ++i) {
			if (!enemy[i]) continue;

			for (int j = 0; j < (int)platform.size(); ++j) {
				if (!platform[j]) continue;

				enemy[i]->HandleProjectileCollision(platform[j]);
			}
		}

		//Enemy shooting character
		if (!character->IsDead()) {
			for (int i = 0; i < (int)enemy.size(); ++i) {
				if (enemy[i] && !enemy[i]->IsDead()) enemy[i]->HandleProjectileCollision(character);
			}
		}
	}

	void HandleOneEntityCollision(Entity* en) {
		if (en->IsDead()) return;
		//obstacles: wall, platform,.... 

		//check obstacle left-touch = character-right touch

		int leftidx = -1, rightidx = -1;

		bool yesLeft = true, yesRight = true, yesFall = true;

		bool headBlock = false;

		for (int i = 0; i < (int)platform.size(); ++i) {
			//dont forget this
			if (!platform[i]) continue;


			if (!en->canMoveRight(platform[i])) {
				yesRight = false;
				rightidx = i;
				en->TakeDamage(platform[i]->getDamage());
			}
			if (!en->canMoveLeft(platform[i])) {
				yesLeft = false;
				leftidx = i;
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

			en->isGrabbing = false;
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
			if (leftidx != -1 && en->fall && platform[leftidx]->CanBeClimbed()) {
				en->isGrabbing = true;
				en->isJumping = false;
			}
			else if (rightidx != -1 && en->fall && platform[rightidx]->CanBeClimbed()) {
				en->isGrabbing = true;
				en->isJumping = false;
			}
		}
	}

	//-----TEST--------//


	void CreatePlatform(float x, float y, float xSize, float ySize, int damage, bool invisible, bool canClimb) {
		platform.push_back(nullptr);
		platform.back() = new Obstacle(textureManager, Vector2f(x, y), Vector2f(xSize, ySize), invisible, damage, canClimb);
	}

	void CreatePlatform(float x, float y, const std::string& file) {
		platform.push_back(nullptr);
		platform.back() = new Obstacle(textureManager, Vector2f(x, y), file);
	}

	void CreatePlatform(float x, float y, int damage, const std::string& file, bool canClimb) {
		platform.push_back(nullptr);
		platform.back() = new Obstacle(textureManager, Vector2f(x, y), file, damage, canClimb);
	}

	void CreateWorld(const std::string& dir, const std::string& mapfile, const std::string& obs1file, const std::string& obs2file) {
		//------MAP-------//
		this->gameMap = new Map(textureManager, dir+mapfile);
		//------INVISIBLE OBS----------//
		this->LoadInvisibleObstacles(dir+obs1file);

		//------ADDITIONAL / SPECIAL OBS-----
		this->LoadSpecialObstacles(dir, obs2file);
	}

	void LockBossZone() {
		CreatePlatform(boss_region_left, 0, 1, 400, 0, 1, 0);
		CreatePlatform(boss_region_right, 0, 1, 400, 0, 1, 0);
		lockBossRegion = true;
	}

	

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

		std::string line;
		std::getline(fin, line);

		while (std::getline(fin, line)) {
			std::stringstream sstr(line);
			std::vector<int> coords;

			std::string field;
			while (std::getline(sstr, field, ',')) {
				coords.push_back(std::stoi(field));
			}

			if ((int)coords.size() == 6) {
				this->CreatePlatform(coords[0], coords[1], coords[2], coords[3], coords[4], true, coords[5]);
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
				this->CreatePlatform(coords[0], coords[1],coords[3], dir + std::to_string(coords[2]) + ".png", coords[4]);
			}
		}


		//--------
		fin.close();
	}

};