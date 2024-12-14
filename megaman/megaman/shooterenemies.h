#ifndef SHOOTERENEMIES_H
#define SHOOTERENEMIES_H

#include "enemy.h"

class Shooter1 : public Shooter {// robot's electric shock
public:
	Shooter1(TextureManager* textureManager) :
		Shooter(textureManager, 17.f, 17.f, 0, 100.f, 0.f) {

		//damage

		this->damage = 30;

		//animation

		this->LoadAnimationForBullet("Animation\\Map1\\ShooterEnemy1\\electricshock.png", "Animation\\Map1\\ShooterEnemy1\\electricshock.png"
			, 70.f, 0, 0, 2, 0, 17, 17); //doesnt need to scale
	}

};

class ShooterEnemy1: public ShooterEnemy { //that big robot with electric shock
private: //no scaling
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
		
		this->health = 200; 

		this->dilation = { 0.f, 20.f };

		//this takes the dilation into consideration
		this->setPosition({ x,y });

	}

	void Update(Character* character, float delt) override {
		if (this->IsDead()) return;
		this->Entity::UpdateEntity(delt);
		UpdateEnemyBehaviour(character, delt);
		UpdateEnemyProjectiles(delt);
		
	}

	void Render(RenderWindow* l_window) override {
		//l_window->draw(frame);
		if (this->IsDead()) return;
		l_window->draw(sprite);
		weapon->RenderProjectiles(l_window);
		
	}
private:
	void InnitDelaySpeed() {
		this->timerPrepare = 1400.f;
		this->timerFinish = 2800.f;
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

	void AttackCharacter(Character* character, float delt) override {
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
		AttackCharacter(character, delt);
	}

	void InnitAnimation() override{
		this->textureManager->BorrowTexture("Animation\\Map1\\ShooterEnemy1\\Robot_Idle.png",
			this->texture_idle_left);
		this->movingAnimation->AddAnimation("ShootLeft", "Animation\\Map1\\ShooterEnemy1\\Robot_Attack.png"
			, 200.f, 0, 0, 13, 0, 50, 70);
	}
};


class Shooter2 : public Shooter { //missile drops from sky then fly
public:
	Shooter2(TextureManager* textureManager) : //mabee scaled 0.5
		Shooter(textureManager, 12.f, 6.f, 20.f, 0.f, 0.f) { //has gravity, innit Xvelo = 0

		//damage

		this->damage = 30;

		//animation

		this->LoadRightTexture("Animation\\Map1\\ShooterEnemy2\\missile_right.png");
		this->LoadLeftTexture("Animation\\Map1\\ShooterEnemy2\\missile_left.png");

		this->LoadAnimationForBullet("Animation\\Map1\\ShooterEnemy2\\missile_move_left.png", "Animation\\Map1\\ShooterEnemy2\\missile_move_right.png"
			, 70.f, 0, 0, 2, 0, 24, 12); //needs scaling

		this->ScaleProjectileAnimation(0.5, 0.5);

	}

	void UpdateMovingProjectiles(float delt, Vector2f pos) override {
		int n = bullets.size();
		for (int i = n - 1; i >= activeBullets; --i) {
			bullets[i]->ProjectileFly(delt, pos); //this moves the projectile
			bullets[i]->Entity::UpdateEntity(delt); //this plays animation of bullets

			//calls this afterwards
			if (bullets[i]->getCurrentTime() >= 0.6) {
				//let it fly horizontally
				bullets[i]->setVelocityX(100.f);
				bullets[i]->setGravity(0.f);
				bullets[i]->setVelocityY(0.f);
			}
		}
	}
};

class ShooterEnemy2 : public ShooterEnemy { //this wasp behaves both like shooter and moving. Scaled 0.75. IT FLIES
private:   
	Vector2f startPoint;
	float distance;
	float timer = 0.f; 
	float timerFinish = 1200.f;
public:

	ShooterEnemy2(TextureManager* textureManager, float x, float y, Direction dir) :
		ShooterEnemy(textureManager, x, y, 20.f, 20.f, 100.f), startPoint(Vector2f(x,y)), distance(100.f) {
		InnitDelaySpeed();

		//overrides
		InnitShooterType();
		InnitAnimation();

		this->FaceDirection(dir);
		this->gravity = 0.f; //this mf doesnt fall
		this->direction = dir;

		//scale 0.75

		this->dilation = { 5,5 };
		this->setPosition({ x,y });

		this->setSpriteScale(0.75, 0.75);
	}

	ShooterEnemy2(TextureManager* textureManager, float x, float y):
		ShooterEnemy(textureManager, x, y, 20.f, 20.f, 100.f), startPoint(Vector2f(x, y)), distance(100.f) {
		InnitDelaySpeed();

		//overrides
		InnitShooterType();
		InnitAnimation();

		this->FaceDirection(Direction::Left);
		this->gravity = 0.f; //this mf doesnt fall
		this->direction = Direction::Left;

		//scale 0.75

		this->dilation = { 5,5 };
		this->setPosition({ x,y });

		this->setSpriteScale(0.75, 0.75);
	}

	void Update(Character* character, float delt) override {
		if (this->IsDead()) return;
		this->Entity::UpdateEntity(delt);
		UpdateEnemyBehaviour(character, delt);
		UpdateEnemyProjectiles(delt);
	}

	void Render(RenderWindow* l_window) {
		if (this->IsDead()) return;
		//l_window->draw(frame);
		l_window->draw(sprite);
		weapon->RenderProjectiles(l_window);
	}

private:

	void InnitDelaySpeed() {
		this->timerFinish = 900.f;
	}


	void InnitShooterType() override {
		this->weapon = new Shooter2(this->textureManager);
	}

	//From entity
	void Shoot(float delt) override {
		isShooting = true;
		this->weapon->Shoot(this->direction);
	}

	void UpdateEnemyProjectiles(float delt) override {
		Vector2f pos = getPosition();
		Vector2f size = getFrameSize();

		weapon->UpdateMovingProjectiles(delt, { pos.x + size.x / 2, pos.y + (size.y) / 2 });
	}

	void AttackCharacter(Character* character, float delt) override {
		if (!this->CharacterInRange(character)) {
			isShooting = false;
			timer = 0;
			movingAnimation->Reset();
		}

		if(direction != LocateCharacterDir(character)) this->FaceDirection(LocateCharacterDir(character));

		//let it play animation
		this->isShooting = true;

		timer += 600 * delt;

		if (timer >= timerFinish) {

			this->Shoot(delt);

			timer = 0;
			isShooting = false;
		}
	}

	void UpdateEnemyBehaviour(Character* character, float delt) override { //similar to moving
		auto v = this->getPosition();

		if (this->CharacterInRange(character)) {
			AttackCharacter(character, delt);
		}
		else {
			isShooting = false;
			if (direction == Direction::Left) {
				if (v.x >= startPoint.x - distance) {
					this->MoveLeft(delt);
				}
				else {
					FaceDirection(Direction::Right);
				}
			}
			if (direction == Direction::Right) {
				if (v.x <= startPoint.x) {
					this->MoveRight(delt);
				}
				else {
					FaceDirection(Direction::Left);
				}
			}
		}
	}

	void InnitAnimation() override {
		this->textureManager->BorrowTexture("Animation\\Map1\\ShooterEnemy2\\wasp_idle_left.png",
			this->texture_idle_left);
		this->textureManager->BorrowTexture("Animation\\Map1\\ShooterEnemy2\\wasp_idle_right.png",
			this->texture_idle_right);

		this->movingAnimation->AddAnimation("MovingShootLeft", "Animation\\Map1\\ShooterEnemy2\\wasp_attack_left.png"
			, 133.f, 0, 0, 8, 0, 40, 40);
		this->movingAnimation->AddAnimation("MovingShootRight", "Animation\\Map1\\ShooterEnemy2\\wasp_attack_right.png"
			, 133.f, 0, 0, 8, 0, 40, 40);
		this->movingAnimation->AddAnimation("MovingLeft", "Animation\\Map1\\ShooterEnemy2\\wasp_left.png"
			, 100.f, 0, 0, 2, 0, 40, 40);
		this->movingAnimation->AddAnimation("MovingRight", "Animation\\Map1\\ShooterEnemy2\\wasp_right.png"
			, 100.f, 0, 0, 2, 0, 40, 40);
	}
};

class Shooter3 : public Shooter { //handicapped robot's laser
public:
	Shooter3(TextureManager* textureManager) :
		Shooter(textureManager, 19.f, 5.f, 0, 100.f, 0.f) {

		//damage

		this->damage = 20;

		//animation

		this->LoadAnimationForBullet("Animation\\Map2\\ShooterEnemy3\\laser_beam.png", "Animation\\Map2\\ShooterEnemy3\\laser_beam.png"
			, 70.f, 0, 0, 0, 0, 19, 5); //doesnt need to scale
	}
};

class ShooterEnemy3 : public ShooterEnemy {//handicapped robot
private:
	float timer = 0.f;
	float timerFinish = 2600.f;
	const Direction fixedMovingDir;
public:
	ShooterEnemy3(TextureManager* textureManager, float x, float y, Direction dir) :
		ShooterEnemy(textureManager, x, y, 30, 31, 200), fixedMovingDir(dir) {


		this->health = 60;
		this->velocityX = 20;

		InnitDelaySpeed();
		InnitShooterType();
		InnitAnimation();

		//this->setSpriteScale(0.5, 0.5);
	}

	void Update(Character* character, float delt) override {
		if (this->IsDead()) return;
		this->Entity::UpdateEntity(delt);
		UpdateEnemyBehaviour(character, delt);
		UpdateEnemyProjectiles(delt);

	}

	void Render(RenderWindow* l_window) override {
		//l_window->draw(frame);
		if (this->IsDead()) return;
		l_window->draw(sprite);
		weapon->RenderProjectiles(l_window);

	}

private:

	void InnitDelaySpeed() {
		this->timerFinish = 2000.f;
	}

	void InnitShooterType() override {
		this->weapon = new Shooter3(this->textureManager);
	}

	void UpdateEnemyProjectiles(float delt) override {
		Vector2f pos = getPosition();
		Vector2f size = getFrameSize();

		weapon->UpdateMovingProjectiles(delt, { pos.x + size.x / 2, pos.y });
	}

	void AttackCharacter(Character* character, float delt) override {
		if (!CharacterInRange(character)) return;
		timer += 600 * delt;

		if (timer >= timerFinish) {

			//shoot
			weapon->Shoot(Direction::Left);

			timer = 0;
		}
	}

	void UpdateEnemyBehaviour(Character* character, float delt) override {
		if (fixedMovingDir == Direction::Right) MoveRight(delt);
		else MoveLeft(delt);

		AttackCharacter(character, delt);
	}

	void InnitAnimation() override {
		this->movingAnimation->AddAnimation("MovingRight", "Animation\\Map2\\ShooterEnemy3\\Crawling.png"
			, 300.f, 0, 0, 5, 0, 50, 31);

		this->movingAnimation->AddAnimation("MovingLeft", "Animation\\Map2\\ShooterEnemy3\\Crawling.png"
			, 300.f, 0, 0, 5, 0, 50, 31);
	}
};


#endif 