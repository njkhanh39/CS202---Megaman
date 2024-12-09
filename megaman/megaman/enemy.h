#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"
#include "shooter.h"
#include "character.h"

using namespace sf;
//interface
class Enemy: public Entity {
protected:
	float viewRange;

	bool active = false;

	//damage when char touches

	int collisiondamage = 5;

	// time stuff

	float enemyTimer = 20.f;
	float timer = 0.f;
public:

	Enemy(TextureManager* textureManager, float x, float y, float sizex, float sizey, float _viewRange): 
		Entity(textureManager, x,y, sizex, sizey), viewRange(_viewRange) {

	}

	~Enemy() override {
		std::cout << "Destructor of Enemy\n";
	}

	virtual void Update(Character* character, float delt) = 0;

	virtual void Render(RenderWindow* l_window) = 0;



	//-----------SHOOTER ENEMY-----------//

	virtual void HandleProjectileCollision(Obstacle* obs, Entity* en) = 0;

	virtual void HandleProjectileCollision(Entity* en) = 0;

	virtual void HandleProjectileCollision(Obstacle* obs) = 0;


	//just a helper
	int GetCollisionDamage() {
		return collisiondamage;
	}
protected:
	virtual void AttackCharacter(Character* character, float delt) = 0;

	virtual void UpdateEnemyBehaviour(Character* character, float delt) = 0;

	virtual void InnitAnimation() = 0;



	
	//helpers to locate the character and attack

	bool CharacterInRange(Character* character) {
		auto v = character->getCenterPosition();
		auto u = getCenterPosition();

		float dist = (u.x - v.x) * (u.x - v.x) + (u.y - v.y) * (u.y - v.y);

		if (dist <= viewRange * viewRange) {
			return true;
		}
		else return false;
	}

	Direction LocateCharacterDir(Character* character) {
		auto vec = character->getCenterPosition();
		auto us = getCenterPosition();

		if (vec.x < us.x) return Direction::Left;
		else return Direction::Right;
	}

	
};

class ShooterEnemy : public Enemy {
protected:
	Shooter* weapon;
public:

	ShooterEnemy(TextureManager* textureManager, float x, float y, float sizex, float sizey, float _viewRange) :
		Enemy(textureManager, x, y,sizex,sizey, _viewRange), weapon(nullptr) {

		
	}

	~ShooterEnemy() override {
		std::cout << "Destructor of the ShooterEnemy.\n";
		delete weapon;
	}



	////Rendering
	//void Render(RenderWindow* l_window) override {
	//	
	//}
	/*l_window->draw(frame);
		l_window->draw(sprite);
		weapon->RenderProjectiles(l_window);*/

	//handling collision

	void HandleProjectileCollision(Obstacle* obs, Entity* en) override {
		weapon->HandleProjectileCollision(obs, en);
	}

	void HandleProjectileCollision(Entity* en) override {
		weapon->HandleProjectileCollision(en);
	}

	void HandleProjectileCollision(Obstacle* obs) override {
		weapon->HandleProjectileCollision(obs);
	}

	

	////updates

	//void Update(Character* character, float delt) override {
	//	this->Entity::UpdateEntity(delt);
	//	UpdateEnemyBehaviour(character, delt);
	//	UpdateEnemyProjectiles(delt);
	//}
	

protected:
	virtual void InnitShooterType() = 0;

	
	virtual void UpdateEnemyProjectiles(float delt) = 0;
};

class MovingEnemy : public Enemy { //interface
private:
	Vector2f startPoint;
	float distance;
public:

	MovingEnemy(TextureManager* textureManager, float x, float y, float sizex, float sizey,
		float _viewRange, Direction direction, Vector2f startPoint, float distance)
		:Enemy(textureManager, x, y, sizex, sizey, _viewRange), startPoint(startPoint), distance(distance)
	{

	}


	void Render(RenderWindow* l_window) {
		l_window->draw(frame);
		l_window->draw(sprite);
	}

protected:
	//virtual for polymorphism

	//this new method which is common for most, we may improvise for harder enemies
	virtual void UpdateEnemyBehaviour(Character* character, float delt) {
		auto v = this->getPosition();
		if (direction == Direction::Left) {
			if (v.x >= startPoint.x - distance) {
				this->MoveLeft(delt);
			}
			else FaceDirection(Direction::Right);
		}
		if (direction == Direction::Right) {
			if (v.x <= startPoint.x) {
				this->MoveRight(delt);
			}
			else FaceDirection(Direction::Left);
		}
	}
};

#endif


