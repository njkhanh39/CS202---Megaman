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

	void Shoot(Direction dir) override {
		//sound
		SoundManager::GetInstance().LoadSound("electric_bolt", "Audio\\SFX\\55 - MMX - Electric Bolt.wav");
		SoundManager::GetInstance().PlaySound("electric_bolt", false);

		this->Shooter::Shoot(dir);
	}
};

class ShooterEnemy1: public ShooterEnemy { //that big robot with electric shock
private: //no scaling
	float timer = 0.f; //prepare shooting

	float timerPrepare2 = 1400.f;
	
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
		this->timerPrepare2 = 1400.f;
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
		
		if (timer <= timerPrepare2) {
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
		Shooter(textureManager, 18.f, 9.f, 20.f, 0.f, 0.f) { //has gravity, innit Xvelo = 0

		//damage

		this->damage = 30;

		//animation

		this->LoadRightTexture("Animation\\Map1\\ShooterEnemy2\\missile_right.png");
		this->LoadLeftTexture("Animation\\Map1\\ShooterEnemy2\\missile_left.png");

		this->LoadAnimationForBullet("Animation\\Map1\\ShooterEnemy2\\missile_move_left.png", "Animation\\Map1\\ShooterEnemy2\\missile_move_right.png"
			, 70.f, 0, 0, 2, 0, 24, 12); //needs scaling

		this->ScaleProjectileAnimation(0.75, 0.75);

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
		ShooterEnemy(textureManager, x, y, 20.f, 20.f, 125.f), startPoint(Vector2f(x,y)), distance(100.f) {
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

class Shooter4 : public Shooter { //pickaxe
public:
	Shooter4(TextureManager* textureManager):
	Shooter(textureManager, 24, 24, 80, 90, -100){

		this->damage = 20;

		this->LoadAnimationForBullet("Animation\\Map2\\ShooterEnemy4\\pickaxe_left.png", "Animation\\Map2\\ShooterEnemy4\\pickaxe_right.png",
			100.f, 0, 0, 2, 0, 24, 24);
	}

	
};

class ShooterEnemy4 : public ShooterEnemy { // miner robot
private:
	float timer = 0.f;
	float timerFinish = 2100.f;
public:
	ShooterEnemy4(TextureManager* textureManager, float x, float y, Direction dir): 
	ShooterEnemy(textureManager, x, y, 20, 34, 200){ //sprite is 50 x 39

		InnitShooterType();
		InnitAnimation();

		this->health = 100;

		this->dilation = { 15.f, 5.f };

		//this takes the dilation into consideration
		this->setPosition({ x,y });

		this->FaceDirection(dir);
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

	void InnitShooterType() override {
		this->weapon = new Shooter4(this->textureManager);
	}

	//From entity
	void Shoot(float delt) override {
		isShooting = true;
		this->weapon->Shoot(this->direction);
	}

	void UpdateEnemyProjectiles(float delt) override {
		Vector2f pos = getPosition();
		Vector2f size = getFrameSize();

		weapon->UpdateMovingProjectiles(delt, { pos.x + size.x / 2, pos.y });
	}

	void AttackCharacter(Character* character, float delt) override {
		if (!this->CharacterInRange(character)) {
			isShooting = false;
			timer = 0;
			movingAnimation->Reset();
		}

		if (direction != LocateCharacterDir(character)) this->FaceDirection(LocateCharacterDir(character));

		//let it play animation
		this->isShooting = true;

		timer += 600 * delt;

		if (timer >= timerFinish) {

			this->Shoot(delt);

			timer = 0;
			isShooting = false;
		}
	}

	void UpdateEnemyBehaviour(Character* character, float delt) override { 
		AttackCharacter(character, delt);
	}

	void InnitAnimation() override {
		this->textureManager->BorrowTexture("Animation\\Map2\\ShooterEnemy4\\idle_left.png",
			this->texture_idle_left);
		this->textureManager->BorrowTexture("Animation\\Map2\\ShooterEnemy4\\idle_right.png",
			this->texture_idle_right);

		this->movingAnimation->AddAnimation("ShootLeft", "Animation\\Map2\\ShooterEnemy4\\throw_left.png"
			, 300.f, 0, 0, 6, 0, 50, 39);

		this->movingAnimation->AddAnimation("ShootRight", "Animation\\Map2\\ShooterEnemy4\\throw_right.png"
			, 300.f, 0, 0, 6, 0, 50, 39);
	}
};

class Shooter5 : public SpecialShooter { //fireball shooter
public:
	Shooter5(TextureManager* textureManager) :
		SpecialShooter(textureManager, 18, 17, 70, 130, 0) {

		this->damage = 15;

		this->LoadAnimationForBullet("Animation\\Map2\\ShooterEnemy5\\fireball.png",
			"Animation\\Map2\\ShooterEnemy5\\fireball.png", 100, 0, 0, 3, 0, 18, 17);
	}
};

class ShooterEnemy5 : public ShooterEnemy { // the flamethrower hanging on the pipe
private:
	float timer = 0;
	float timerFinish = 2500.f;

	std::vector<float> bullet_range;
	int cur_range = 0;
public:
	ShooterEnemy5(TextureManager* textureManager, float x, float y, Direction dir) :
		ShooterEnemy(textureManager, x, y, 31, 24, 200) {

		InnitShooterType();
		InnitAnimation();

		this->gravity = 0.f;
		this->health = 100;

		FaceDirection(dir);
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
	void InnitShooterType() override {
		this->weapon = new Shooter5(this->textureManager);
		bullet_range = { 100,150,200 };
	}

	void UpdateEnemyProjectiles(float delt) override {
		Vector2f pos = getPosition();
		Vector2f size = getFrameSize();

		if (this->direction == Direction::Left) {
			weapon->UpdateMovingProjectiles(delt, { pos.x, pos.y });
		}
		else weapon->UpdateMovingProjectiles(delt, { pos.x + size.x, pos.y });
	}

	void AttackCharacter(Character* character, float delt) override {
		if (!CharacterInRange(character)) {
			timer = 0;
			return;
		}
		timer += 600 * delt;

		if (timer >= timerFinish) {

			if (cur_range >= (int)bullet_range.size()) {
				cur_range = 0;
			}

			this->weapon->SetVelocityX(bullet_range[cur_range]);
			this->weapon->Shoot(this->direction);

			timer = 0;
			++cur_range;
		}
	}

	void UpdateEnemyBehaviour(Character* character, float delt) override {
		AttackCharacter(character, delt);
	}

	void InnitAnimation() override {
		this->textureManager->BorrowTexture("Animation\\Map2\\ShooterEnemy5\\idle_left.png",
			texture_idle_left);
		this->textureManager->BorrowTexture("Animation\\Map2\\ShooterEnemy5\\idle_right.png",
			texture_idle_right);
	}
};

class Shooter6 : public Shooter { //snowball shooter
public:
	Shooter6(TextureManager* textureManager) :
		Shooter(textureManager, 13, 13, 40, 80, 0) {

		this->damage = 20;

		this->LoadAnimationForBullet("Animation\\Map3\\ShooterEnemy6\\snowball.png",
			"Animation\\Map3\\ShooterEnemy6\\snowball.png", 100, 0, 0, 0, 0, 13, 13);
	}
};

class ShooterEnemy6 : public ShooterEnemy {//snow cannon
private:
	float timer = 0;
	float timerFinish = 2000.f;

	std::vector<float> bullet_range;
	int cur_range = 0;
public:
	ShooterEnemy6(TextureManager* textureManager, float x, float y, Direction dir) :
		ShooterEnemy(textureManager, x, y, 44, 28, 300) {

		InnitShooterType();
		InnitAnimation();

		this->gravity = 0.f;
		this->health = 100;

		FaceDirection(dir);
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
	void InnitShooterType() override {
		this->weapon = new Shooter6(this->textureManager);
		bullet_range = { 100,150,200 };
	}

	void UpdateEnemyProjectiles(float delt) override {
		Vector2f pos = getPosition();
		Vector2f size = getFrameSize();

		if (this->direction == Direction::Left) {
			weapon->UpdateMovingProjectiles(delt, { pos.x, pos.y });
		}
		else weapon->UpdateMovingProjectiles(delt, { pos.x + size.x, pos.y });
	}

	void AttackCharacter(Character* character, float delt) override {
		if (!CharacterInRange(character)) {
			timer = 0;
			return;
		}
		timer += 600 * delt;

		if (timer >= timerFinish) {

			if (cur_range >= (int)bullet_range.size()) {
				cur_range = 0;
			}

			this->weapon->SetVelocityX(bullet_range[cur_range]);
			this->weapon->Shoot(this->direction);

			timer = 0;
			++cur_range;
		}
	}

	void UpdateEnemyBehaviour(Character* character, float delt) override {
		AttackCharacter(character, delt);
	}

	void InnitAnimation() override {
		this->textureManager->BorrowTexture("Animation\\Map3\\ShooterEnemy6\\idle_left.png",
			texture_idle_left);
		this->textureManager->BorrowTexture("Animation\\Map3\\ShooterEnemy6\\idle_right.png",
			texture_idle_right);
	}
};

class Shooter7 : public Shooter {//saw blade
public:
	Shooter7(TextureManager* textureManager) :
		Shooter(textureManager, 16, 16, 10, 70, 0) {

		this->damage = 20;

		this->LoadAnimationForBullet("Animation\\Map3\\ShooterEnemy7\\saw_blade.png", "Animation\\Map3\\ShooterEnemy7\\saw_blade.png",
			100.f, 0, 0, 2, 0, 16, 16);
	}
};

class ShooterEnemy7 : public ShooterEnemy {// ostrich with blade
private:
	float timer = 0;
	float timerFinishShoot = 4600.f;
	float timerDelay = 2800.f;
	bool alreadyShoot = false;

	const Vector2f shoot_dilation = { 5.f,0 };
	const Vector2f regular_dilation = { 0,0 };
public:
	ShooterEnemy7(TextureManager* textureManager, float x, float y, Direction dir) :
		ShooterEnemy(textureManager, x, y, 30, 73, 150.f) {

		//overrides
		InnitShooterType();
		InnitAnimation();

		this->health = 100;
		this->FaceDirection(dir);
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
	void InnitShooterType() override {
		this->weapon = new Shooter7(this->textureManager);
	}


	//From entity
	void Shoot(float delt) override {
		isShooting = true;
		this->weapon->Shoot(this->direction);
	}

	void UpdateEnemyProjectiles(float delt) override {
		Vector2f pos = getPosition();
		Vector2f size = getFrameSize();

		if(this->direction == Direction::Left) weapon->UpdateMovingProjectiles(delt, { pos.x, pos.y});
		else weapon->UpdateMovingProjectiles(delt, { pos.x + size.x, pos.y });
	}

	void AttackCharacter(Character* character, float delt) override {
		if (!CharacterInRange(character)) {

			if(this->direction == Direction::Left) movingAnimation->Play("Idle_Left", delt);
			else movingAnimation->Play("Idle_Right", delt);

			this->dilation = regular_dilation;

			setPosition(getUpLeftPosition());

			isShooting = false;

			alreadyShoot = false;

			timer = 0;
			return;
		}

		auto dir = LocateCharacterDir(character);


		if (dir == Direction::Left && this->direction != Direction::Left) TurnLeft();
		if (dir == Direction::Right && this->direction != Direction::Right) TurnRight();


		timer += 600 * delt;

		if (timer >= timerDelay) {

			if (!this->alreadyShoot) {
				this->Shoot(delt);
				this->alreadyShoot = true;

				//set dilation for animation

				this->dilation = shoot_dilation;
				setPosition(getUpLeftPosition());
			}

			if (timer >= timerFinishShoot) {
				

				this->dilation = regular_dilation;
				setPosition(getUpLeftPosition());

				this->isShooting = false;
				timer = 0;
				alreadyShoot = false;
			}
		}
		else {
			if (this->direction == Direction::Left) movingAnimation->Play("Idle_Left", delt);
			else movingAnimation->Play("Idle_Right", delt);
		}
		
	}
	void UpdateEnemyBehaviour(Character* character, float delt) override {
		AttackCharacter(character, delt);
	}

	void InnitAnimation() override {

		//idle
		this->movingAnimation->AddAnimation("Idle_Left", "Animation\\Map3\\ShooterEnemy7\\idle_left.png"
			,400.f, 0, 0, 6, 0, 30, 73);
		this->movingAnimation->AddAnimation("Idle_Right", "Animation\\Map3\\ShooterEnemy7\\idle_right.png"
			, 400.f, 0, 0, 6, 0, 30, 73);
		
		this->movingAnimation->AddAnimation("ShootLeft", "Animation\\Map3\\ShooterEnemy7\\shoot_left.png"
			, 200.f, 0, 0, 8, 0, 40, 73);
		this->movingAnimation->AddAnimation("ShootRight", "Animation\\Map3\\ShooterEnemy7\\shoot_right.png"
			, 200.f, 0, 0, 8, 0, 40, 73);
	}
};

class Shooter8 : public Shooter { //snow catapult
public:
	Shooter8(TextureManager* textureManager) :
		Shooter(textureManager, 13, 13, 40, 80, -50) {

		this->damage = 20;

		this->LoadAnimationForBullet("Animation\\Map3\\ShooterEnemy6\\snowball.png",
			"Animation\\Map3\\ShooterEnemy6\\snowball.png", 100, 0, 0, 0, 0, 13, 13);
	}
};

class ShooterEnemy8 : public ShooterEnemy { //snow thrower
private:
	float timer = 0;
	float timerFinish = 2800.f;
	float timerDelay = 1400.f;
	const Vector2f left_dilation = { 0,13.f };
	const Vector2f right_dilation = { 17.f, 13.f };
public:
	ShooterEnemy8(TextureManager* textureManager, float x, float y, Direction dir) :
		ShooterEnemy(textureManager, x, y, 20, 41, 200) {

		//overrides
		InnitShooterType();
		InnitAnimation();

		this->health = 100;
		this->FaceDirection(dir);

		//dilation
		if (this->direction == Direction::Left) {
			this->dilation = left_dilation;
		}
		else this->dilation = right_dilation;

		//to activate dilation
		setPosition({ x,y });
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
	void InnitShooterType() override {
		this->weapon = new Shooter8(this->textureManager);
	}

	void UpdateEnemyProjectiles(float delt) override {
		Vector2f pos = getPosition();
		Vector2f size = getFrameSize();

		if (this->direction == Direction::Left) weapon->UpdateMovingProjectiles(delt, { pos.x, pos.y });
		else weapon->UpdateMovingProjectiles(delt, { pos.x + size.x, pos.y });
	}

	void AttackCharacter(Character* character, float delt) override {
		if (!CharacterInRange(character)) {
			isShooting = false;
			this->movingAnimation->Reset();
			timer = 0;
		}

		auto dir = LocateCharacterDir(character);

		if (this->direction != dir) this->FaceDirection(dir);

		timer += 600 * delt;
		

		if (timer >= timerDelay) {

			isShooting = true; //for animation playing
			if (timer >= timerFinish) {

				//attack
				this->weapon->Shoot(this->direction);

				//reset
				timer = 0;
				isShooting = false;
			}
		}
	}
	void UpdateEnemyBehaviour(Character* character, float delt) override {
		AttackCharacter(character, delt);
	}

	void InnitAnimation() override {

		//idle
		this->textureManager->BorrowTexture("Animation\\Map3\\ShooterEnemy8\\idle_left.png",
			texture_idle_left);

		this->textureManager->BorrowTexture("Animation\\Map3\\ShooterEnemy8\\idle_right.png",
			texture_idle_right);

		this->movingAnimation->AddAnimation("ShootLeft", "Animation\\Map3\\ShooterEnemy8\\shoot_left.png"
			, 200.f, 0, 0, 6, 0, 37, 54);
		this->movingAnimation->AddAnimation("ShootRight", "Animation\\Map3\\ShooterEnemy8\\shoot_right.png"
			, 200.f, 0, 0, 6, 0, 37, 54);
	}
};


#endif 