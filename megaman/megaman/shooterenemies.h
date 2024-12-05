#ifndef SHOOTERENEMIES_H
#define SHOOTERENEMIES_H

#include "enemy.h"

class Shooter1 : public Shooter {// robot's electric shock
public:
	Shooter1(TextureManager* textureManager) :
		Shooter(textureManager, 17.f, 17.f, 0, 100.f, 0.f) {

		//damage

		this->damage = 1;

		//animation

		this->LoadAnimationForBullet("Animation\\Map1\\ShooterEnemy1\\electricshock.png", "Animation\\Map1\\ShooterEnemy1\\electricshock.png"
			, 70.f, 0, 0, 2, 0, 17, 17); //doesnt need to scale
	}

};

class ShooterEnemy1: public ShooterEnemy { //that big robot with electric shock
private:
	float timer = 0.f; //prepare shooting

	float timerPrepare = 1400.f;
	
	float timerFinish = 2800.f;
	bool alreadyShoot = false;
public: //view range = 150.f
	ShooterEnemy1(TextureManager* textureManager, float x, float y): 
	ShooterEnemy(textureManager, x, y, 50, 50, 200.f){
		InnitDelaySpeed();

		//overrides
		InnitShooterType();
		InnitAnimation();
		
		this->dilation = { 0.f, 20.f };

		//this takes the dilation into consideration
		this->setPosition({ x,y });

	}

	void Update(Character* character, float delt) override {
		this->Entity::UpdateEntity(delt);
		UpdateEnemyBehaviour(character, delt);
		UpdateEnemyProjectiles(delt);
	}

	void Render(RenderWindow* l_window) {
		//l_window->draw(frame);
		l_window->draw(sprite);
		weapon->RenderProjectiles(l_window);
	}
private:
	void InnitDelaySpeed() {
		this->timerPrepare = 10.f;
	}

	void InnitShooterType() override {
		this->weapon = new Shooter1(this->textureManager);
	}


	//From entity
	void Shoot(float delt) override {
		isShooting = true;
		this->weapon->Shoot(this->direction);
	}

	void UpdateEnemyProjectiles(float delt) override {
		Vector2f pos = getPosition();
		Vector2f size = getFrameSize();

		weapon->UpdateMovingProjectiles(delt, { pos.x + size.x/2, pos.y + (size.y)/2});
	}

	void AttackCharacter(Character* character, float delt) override {// it shoots two missiles at once
		if (!CharacterInRange(character)) {
			movingAnimation->Reset();
			isShooting = false;
			alreadyShoot = false;
			timer = 0;
			return;
		}

		auto dir = LocateCharacterDir(character);
		

		if (dir == Direction::Left && this->direction != Direction::Left) TurnLeft();
		if (dir == Direction::Right && this->direction != Direction::Right) TurnRight();


		timer += 600 * delt;
		
		if (timer <= timerPrepare) {
			isShooting = true; //this invokes the animation
		}
		


		//1 frame = 200.f, 1400.f = 7 frames = half way through
		if (timer + 1000.f >= timerFinish) {
			if (!alreadyShoot) {
				this->Shoot(timer);
				alreadyShoot = true;
			}

			if (timer >= timerFinish) {
				isShooting = false;
				alreadyShoot = false;
				timer = 0;
			}
		}
	
	}
	void UpdateEnemyBehaviour(Character* character, float delt) override {
		if (character->IsDead()) return;
		AttackCharacter(character, delt);
	}

	void InnitAnimation() override{
		this->textureManager->BorrowTexture("Animation\\Map1\\ShooterEnemy1\\Robot_Idle.png",
			this->texture_idle_left);
		this->movingAnimation->AddAnimation("ShootLeft", "Animation\\Map1\\ShooterEnemy1\\Robot_Attack.png"
			, 200.f, 0, 0, 13, 0, 50, 70);
	}
};

class ShooterEnemy2 : public ShooterEnemy {// bee that drop bombs

};

#endif 