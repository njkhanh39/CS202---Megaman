#pragma once
#include "boss.h"
#include "shooter.h"

class OilDropper : public SpecialShooter {
private:
	sf::Clock clk; // for delay

	float delay = 1.f;
	bool firstShot = false;

	const int v1 = 70.f;
	const int v2 = 120.f;
	const int v3 = 130.f;
public:
	OilDropper(TextureManager* textureManager) :
		SpecialShooter(textureManager, 15.f, 14.f, 100, 200.f, -80.f) {

		//damage

		this->damage = 15;


		//animation

		this->LoadAnimationForBullet("Animation\\Map2\\Flame_Mammoth\\oil_drop.png",
			"Animation\\Map2\\Flame_Mammoth\\oil_drop.png"
			, 70.f, 0, 0, 0, 0, 15, 14); //doesnt need to scale
	}

	void Shoot(Direction dir) override {
		//shoots three times
		if (clk.getElapsedTime().asSeconds() < delay && firstShot == true) {
			return;
		}
		else if (firstShot == false) {
			this->firstShot = true;
		}
		clk.restart();

		this->SetVelocityX_One_ActiveBullet(v1);
		this->SetVelocityY_One_ActiveBullet(-60);
		this->Shooter::Shoot(dir);

		this->SetVelocityX_One_ActiveBullet(v2);
		this->SetVelocityY_One_ActiveBullet(-90);
		this->Shooter::Shoot(dir);

		this->SetVelocityX_One_ActiveBullet(v3);
		this->SetVelocityY_One_ActiveBullet(-140);
		this->Shooter::Shoot(dir);


	}


};

class FlameThrower : public SpecialShooter {
private:
	sf::Clock clk; // for delay

	float delay = 0.75;
	bool firstShot = false;
public:
	FlameThrower(TextureManager* textureManager) :
		SpecialShooter(textureManager, 16.f, 16.f, 0, 150.f, 0.f) {

		//damage

		this->damage = 30;


		//animation

		this->LoadAnimationForBullet("Animation\\Own_By_Many\\fire_left.png",
			"Animation\\Own_By_Many\\fire_right.png"
			, 100.f, 0, 0, 2, 0, 16, 12); //doesnt need to scale
	}

	void Shoot(Direction dir) override {

		//----We add this----//

		if (clk.getElapsedTime().asSeconds() < delay && firstShot == true) {
			return;
		}
		else if (firstShot == false) {
			this->firstShot = true;
		}

		clk.restart();

		//------------------//

		this->Shooter::Shoot(dir);
	}

	void SetClockDelay(float tmp) {
		this->delay = tmp;
	}
};

class FlameMammoth : public Boss {
private:

	const Vector2f normal_dilation = { 20.f, 0.f };
	const Vector2f shoot_right_dilation = { 0.f,0.f };
	const Vector2f shoot_left_dilation = { 40.f, 0.f };
	const Vector2f die_dilation = { 25.f,15.f };

	OilDropper* oil_dropper;
	FlameThrower* flame_thrower;

public:
	FlameMammoth(TextureManager* textureManager, float x, float y, Vector2f bounded) :
		Boss(textureManager, x, y, 41, 65, 300, bounded, 2000) //no scaling
	{

		InnitBossAttributes(x, y);

		InnitActionDuration();

		InnitBossCommands();

		InnitBossWeapon();

		InnitAnimation();
	}


	~FlameMammoth() {
		delete oil_dropper;
		delete flame_thrower;
	}

	void Update(Character* character, float delt) override {
		if (!this->IsDead()) {
			this->Boss::UpdateEntity(delt);
			this->UpdateEnemyBehaviour(character, delt);
			this->UpdateEnemyProjectiles(delt);
		}
		else {
			if (this->dilation != die_dilation) {
				this->dilation = die_dilation;
				setPosition(getPosition());
			}
			this->Boss::UpdateDeath(delt);
		}
	}

	void Render(RenderWindow* l_window) override {
		//l_window->draw(this->frame);
		if (!this->IsDead() || finishDeathAnimation == false) {
			l_window->draw(this->sprite);
			if (!this->IsDead()) {
				oil_dropper->RenderProjectiles(l_window);
				flame_thrower->RenderProjectiles(l_window);
			}
		}
	}

	//-----------SHOOTING-----------//

	void HandleProjectileCollision(Obstacle* obs, Entity* en) override {
		oil_dropper->HandleProjectileCollision(obs, en);
		flame_thrower->HandleProjectileCollision(obs, en);
	}

	void HandleProjectileCollision(Entity* en) override {
		oil_dropper->HandleProjectileCollision(en);
		flame_thrower->HandleProjectileCollision(en);
	}

	void HandleProjectileCollision(Obstacle* obs) override {
		oil_dropper->HandleProjectileCollision(obs);
		flame_thrower->HandleProjectileCollision(obs);
	}

private:

	void UpdateEnemyBehaviour(Character* character, float delt) override {
		if (flags[0]) {
			if (direction != LocateCharacterDir(character)) {
				this->FaceDirection(LocateCharacterDir(character));
			}
		}

		AttackCharacter(character, delt);
	}

	void UpdateEnemyProjectiles(float delt) {
		Vector2f pos = getPosition();
		Vector2f size = getFrameSize();

		if (direction == Direction::Right) { 
			oil_dropper->UpdateMovingProjectiles(delt, { pos.x + size.x, pos.y }); 
			flame_thrower->UpdateMovingProjectiles(delt, { pos.x + size.x, pos.y + size.y / 3 });
		}
		else {
			oil_dropper->UpdateMovingProjectiles(delt, { pos.x, pos.y });
			flame_thrower->UpdateMovingProjectiles(delt, { pos.x, pos.y + size.y / 3 });
		}
	}

	void AttackCharacter(Character* character, float delt) override {
		timer += 600 * delt;

		if (timer >= delay) {

			BOSSCOMMANDS curcmd = commands[cur];

			actionTimer += 600 * delt;

			this->ExecuteCommand(character, delt, curcmd);


			if (actionTimer >= actionDuration[curcmd]) {
				//reset
				std::cout << "reset!\n";
				this->ResetAfterAttack();
				++cur;
				if (cur >= (int)commands.size()) cur = 0;
			}
		}
	}


	void ExecuteCommand(Character* character, float delt, BOSSCOMMANDS com) override {
		if (com == BOSSCOMMANDS::ATTACK1) { //spit oil
			this->ExecuteAttack1(character, delt);
		}
		if (com == BOSSCOMMANDS::ATTACK2) { //shoot fire ball
			this->ExecuteAttack2(character, delt);
		}
		if (com == BOSSCOMMANDS::ATTACK3) { 
			this->ExecuteAttack3(character, delt);
		}
	}

	void ResetAfterAttack() override {
		this->dilation = normal_dilation;
		timer = 0;
		actionTimer = 0;
		//idle after attack

		TurnOnFlag(0);
	}

	
	void ExecuteAttack1(Character* character, float delt) override { //spit oil
		auto dir = LocateCharacterDir(character);

		if (dir != direction) this->FaceDirection(dir);

		this->TurnOnFlag(1);

		//we only attack once!
		if(actionTimer >= actionDuration[ATTACK1]){
			this->oil_dropper->Shoot(dir);
		}
	}

	void ExecuteAttack2(Character* character, float delt) override { //shoot fireball
		auto dir = LocateCharacterDir(character);

		if (dir != direction) this->FaceDirection(dir);

	

		//set dilation

		if (dir == Direction::Left) {
			if (this->dilation != shoot_left_dilation) {
				this->dilation = shoot_left_dilation;
				setPosition(this->getUpLeftPosition());
			}
		}
		else {
			if (this->dilation != shoot_right_dilation) {
				this->dilation = shoot_right_dilation;
				setPosition(this->getUpLeftPosition());
			}
		}

		this->TurnOnFlag(2);

		this->flame_thrower->Shoot(dir);
		
	}

	void ExecuteAttack3(Character* character, float delt) override {
		//locate dir once only

		if (this->flags[3] == false) {
			auto dir = LocateCharacterDir(character);

			if (dir != direction) this->FaceDirection(dir);

			//too far left
			if (this->getLeftMostX() - 5.f < bounded.x) {
				this->FaceDirection(Direction::Right);
			}
			if (this->getRightMostX() + 5.f > bounded.y) {
				this->FaceDirection(Direction::Left);
			}
		}

		this->TurnOnFlag(3);

		if (actionTimer >= 600.f) { //jump
			
			if (this->getLeftMostX() - 5.f < bounded.x || this->getRightMostX() + 5.f > bounded.y) {
				if(this->isJumping == true) FinishAttack(ATTACK3);

				if (this->getLeftMostX() - 5.f < bounded.x) {
					this->setPosition({ bounded.x + 10.f, this->getPosition().y });
				}
				else if (this->getRightMostX() + 5.f > bounded.y) {
					this->setPosition({ bounded.y - this->getFrameSize().y - 10.f, this->getPosition().y});
				}
			}
			else {
				this->Jump(delt);


				if (this->direction == Direction::Left) {
					this->MoveLeft(delt);
				}
				else {
					this->MoveRight(delt);
				}
			}
		}
	}

	void ExecuteAttack4(Character* character, float delt) override {}

	void InnitBossAttributes(float x, float y) override {
		//-----------//

		this->gravity = 200.f;
		this->delay = 4000.f;
		this->velocityX = 120.f;
		this->health = 1000;
		this->jumpStrength = 140.f;
		this->collisiondamage = 30.f;
		this->deathAnimationMaxTimer = 4800.f;
		//----------//
		//vile's hitbox should be smaller than sprite (54,45)
		this->dilation = normal_dilation;
		this->Entity::setPosition({ x,y });

	}

	void InnitBossCommands() override {
		TurnOnFlag(0);

		commands.push_back(BOSSCOMMANDS::ATTACK1);
		commands.push_back(BOSSCOMMANDS::ATTACK2);
		commands.push_back(BOSSCOMMANDS::ATTACK3);

		/*for (int i = 0; i < 4; ++i) {
			commands.push_back(BOSSCOMMANDS::ATTACK1);
			commands.push_back(BOSSCOMMANDS::ATTACK2);
			commands.push_back(BOSSCOMMANDS::ATTACK3);
			commands.push_back(BOSSCOMMANDS::ATTACK2);
		}
		commands.push_back(BOSSCOMMANDS::ATTACK1);
		commands.push_back(BOSSCOMMANDS::ATTACK1);
		commands.push_back(BOSSCOMMANDS::ATTACK3);
		commands.push_back(BOSSCOMMANDS::ATTACK3);*/
	}

	void InnitBossWeapon() override {
		oil_dropper = new OilDropper(textureManager);
		flame_thrower = new FlameThrower(textureManager);
	}

	void InnitActionDuration() override {
		//command0 = idle doesnt need duration

		actionDuration[ATTACK1] = 1400.f;
		actionDuration[ATTACK2] = 2400.f;
		actionDuration[ATTACK3] = 1400.f;
	}


	void InnitAnimation() override {
		//idle
		movingAnimation->AddAnimation("Command0_left", "Animation\\Map2\\Flame_Mammoth\\idle_left.png", 100.f, 0, 0, 0, 0, 81, 65);
		movingAnimation->AddAnimation("Command0_right", "Animation\\Map2\\Flame_Mammoth\\idle_right.png", 100.f, 0, 0, 0, 0, 81, 65);

		//attack1 - spit oil
		movingAnimation->AddAnimation("Command1_left", "Animation\\Map2\\Flame_Mammoth\\spit_left.png",
			200.f, 0, 0, 6, 0, 81, 65);
		movingAnimation->AddAnimation("Command1_right", "Animation\\Map2\\Flame_Mammoth\\spit_right.png",
			200.f, 0, 0, 6, 0, 81, 65);
		//attack2 - shoot fireball

		movingAnimation->AddAnimation("Command2_left", "Animation\\Map2\\Flame_Mammoth\\shoot_left.png",
			200.f, 0, 0, 3, 0, 98, 65);
		movingAnimation->AddAnimation("Command2_right", "Animation\\Map2\\Flame_Mammoth\\shoot_right.png",
			200.f, 0, 0, 3, 0, 98, 65);

		//attack3 - jump
		movingAnimation->AddAnimation("Command3_left", "Animation\\Map2\\Flame_Mammoth\\jump_left.png",
			200.f, 0, 0, 6, 0, 81, 65);
		movingAnimation->AddAnimation("Command3_right", "Animation\\Map2\\Flame_Mammoth\\jump_right.png",
			200.f, 0, 0, 6, 0, 81, 65);

		//cmd 6 - die
		movingAnimation->AddAnimation("Command6_left", "Animation\\Map2\\Flame_Mammoth\\die_left.png", 150.f, 0, 0, 2, 0, 78, 79);
		movingAnimation->AddAnimation("Command6_right", "Animation\\Map2\\Flame_Mammoth\\die_right.png", 150.f, 0, 0, 2, 0, 78, 79);


	}
};