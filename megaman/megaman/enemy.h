#pragma once
#include "entity.h"
#include "shooter.h"
#include "character.h"

using namespace sf;

class Enemy: public Entity {
protected:
	float viewRange;
	float movingRange;

	bool active = false;

	// time stuff

	float enemyTimer = 3.f;
	float timer = 0.f;
public:
	bool canMove;

	Enemy(float x, float y, bool _canMove, float _movingRange, float _viewRange): Entity(x,y) {
		canMove = _canMove;
		movingRange = _movingRange;
		viewRange = _viewRange;
	}

	Enemy(bool _canMove, float _movingRange, float _viewRange) : Entity() {
		canMove = _canMove;
		movingRange = _movingRange;
		viewRange = _viewRange;
	}

	~Enemy() override {
		std::cout << "Destructor of Enemy\n";
	}


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
private:
	Shooter* weapon;
public:
	ShooterEnemy(float x, float y, bool _canMove, float _movingRange, float _viewRange): Enemy(x,y,_canMove, _movingRange, _viewRange) {
		weapon = new Shooter();

		setSize({ 50.f, 100.f });
	}

	~ShooterEnemy() override {
		std::cout << "Destructor of the ShooterEnemy.\n";
		delete weapon;
	}

	//Rendering
	void Render(RenderWindow* l_window) {
		l_window->draw(frame);
		l_window->draw(sprite);
		weapon->RenderProjectiles(l_window);
	}


	//handling collision

	void HandleProjectileCollision(Obstacle* obs, Entity* en) {
		weapon->HandleProjectileCollision(obs, en);
	}

	void HandleProjectileCollision(Entity* en) {
		weapon->HandleProjectileCollision(en);
	}

	void HandleProjectileCollision(Obstacle* obs) {
		weapon->HandleProjectileCollision(obs);
	}

	//Attacking character

	void AttackCharacter(Character* character, float delt) {
		auto dir = LocateCharacterDir(character);

		timer += delt;

		if (dir == Direction::Left) TurnLeft();
		if (dir == Direction::Right) TurnRight();

		//std::cout << timer << '\n';

		if (timer >= enemyTimer) {
			timer = 0;
			//attack

			weapon->Shoot(dir);
		}
	}

	//updates

	void Update(Character* character, float delt) {
		UpdateMovements(delt);
		UpdateEnemyBehaviour(character, delt);
		UpdateEnemyProjectiles(delt);
	}


private:

	void UpdateEnemyProjectiles(float delt) {
		Vector2f pos = getPosition();
		Vector2f size = getFrameSize();

		weapon->UpdateMovingProjectiles(delt, { pos.x + size.x, pos.y});
	}

	void UpdateEnemyBehaviour(Character* character, float delt) {
		if (!CharacterInRange(character)) return; //idle

		AttackCharacter(character, delt);
	}
};


