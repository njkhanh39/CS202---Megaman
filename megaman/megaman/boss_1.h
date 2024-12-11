#pragma once
#include "boss.h"
#include "shooter.h"

class ElectricCannon : public Shooter {
	sf::Clock clk; // for delay

	float delay = 1.f;
	bool firstShot = false;
public:
	ElectricCannon(TextureManager* textureManager) :
		Shooter(textureManager, 16.f, 16.f, 0, 150.f, 0.f) {

		//damage

		this->damage = 60;
		

		//animation

		this->LoadAnimationForBullet("Animation\\Own_By_Many\\electric_left.png",
			"Animation\\Own_By_Many\\electric_right.png"
			, 70.f, 0, 0, 3, 0, 16, 16); //doesnt need to scale
	}

	void Shoot(Direction dir) override {

		//----We add this----//

		if (clk.getElapsedTime().asSeconds() < delay && firstShot==true) {
			return;
		}
		else if (firstShot == false) {
			this->firstShot = true;
		}

		clk.restart();

		//------------------//

		if (activeBullets == 0) {
			//temporarily, we only give 10 bullets
			return;
		}


		int n = bullets.size();

		bullets[activeBullets - 1]->FaceDirection(dir);
		--activeBullets;

		//load
		Load(dir);

		std::cout << "Number of bullets in vector: " << (int)bullets.size() << '\n';
	}

	void SetClockDelay(float tmp) {
		this->delay = tmp;
	}
};

class Vile: public Boss {
private:
	const Vector2f normal_framesize = { 20.f, 40.f };
	const Vector2f normal_dilation = { 17.f, 5.f };

	const Vector2f dash_framesize = { 54.f,30.f }; //shorter, but wide
	const Vector2f dash_dilation = { 0.f, 15.f };


	//----------------------

	const float dashVelocity = 240.f;
	const float normalVelocityX = 180.f;


	//--------ATTACK3'S------------

	float attack3_timer = 0;

	//----------------------


	//---Weapon--//
	ElectricCannon* cannon;
public:
	Vile(TextureManager* textureManager, float x, float y, Vector2f bounded, float delay):
		Boss(textureManager, x, y, 20,40, 300,bounded, delay) //no scaling
	{
				
		InnitBossAttributes(x, y);

		InnitActionDuration();

		InnitBossCommands();

		InnitBossWeapon();
		
		InnitAnimation();
	}

	~Vile() {
		delete cannon;
	}

	void Update(Character* character, float delt) override {
		this->UpdateEntity(delt);
		this->UpdateEnemyBehaviour(character, delt);
		this->UpdateEnemyProjectiles(delt);
	}

	void Render(RenderWindow* l_window) override {
		//l_window->draw(this->frame);
		l_window->draw(this->sprite);
		cannon->RenderProjectiles(l_window);
	}

	//-----------SHOOTING-----------//

	void HandleProjectileCollision(Obstacle* obs, Entity* en) override {
		cannon->HandleProjectileCollision(obs, en);
	}

	void HandleProjectileCollision(Entity* en) override {
		cannon->HandleProjectileCollision(en);
	}

	void HandleProjectileCollision(Obstacle* obs) override {
		cannon->HandleProjectileCollision(obs);
	}
protected:

	//Inherits from entity

	void Shoot(float delt) override {
		//dont care about boolean?....
		this->cannon->Shoot(direction);
	}

	void UpdateEntity(float delt) override {

		//Death
		if (this->IsDead()) return;

		//invisibility 

		if (this->invisible) { //true = is being hurt
			this->invisibleTimer += 100 * delt;

			if (this->invisibleTimer >= this->invisibleMaxTimer) { // we are done with invisibility!
				this->invisible = false;
				this->invisibleTimer = 0;
			}
		}


		//---Movement----

		if (isJumping || fall) {
			//Entity fall



			velocityY += gravity * delt;
			isGrabbing = false;


			//when reaches a limit, fall & isJumping will be turned to false
			//by our canKeepFalling function


			//move our 2 attributes

			sprite.move(0, velocityY * delt);
			frame.setPosition({ frame.getPosition().x, frame.getPosition().y + velocityY * delt });
		}
		else velocityY = 0.0f; //not falling & not jumping = 0 velocityY ?? :D

		//-----ANIMATION UPDATE---------

		//from first if to last if, it follows the animation priority

		//if (invisible) { //play the hurt animation
		//	movingAnimation->Play("HurtAnimation", delt / 2); //make animation slower by dividing
		//}
		
		std::string dr = "_left";
		if (this->direction == Direction::Right) dr = "_right";

		for (int i = 0; i < 6; ++i) {
			if (flags[i]) {
				movingAnimation->Play("Command" + std::to_string(i) + dr, delt);
				break;
			}
		}
	}

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

		if(direction == Direction::Right) cannon->UpdateMovingProjectiles(delt, { pos.x + size.x, pos.y });
		else cannon->UpdateMovingProjectiles(delt, { pos.x, pos.y });
	}

	void AttackCharacter(Character* character, float delt) override {
		timer += 600 * delt;

		if (timer >= delay) {

			BOSSCOMMANDS curcmd = commands[cur];

			actionTimer += 600 * delt;

			//continuous behavior (constantly doing)

			//this->Shoot(delt); //its ok, we have the cannon's clock. Or at least u can set duration = 0, then we done

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
		if (com == BOSSCOMMANDS::ATTACK1) {
			this->ExecuteAttack1(character, delt);
		}
		if (com == BOSSCOMMANDS::ATTACK2) {
			this->ExecuteAttack2(character, delt);
		}
		if (com == BOSSCOMMANDS::ATTACK3) {
			this->ExecuteAttack3(character, delt);
		}
	}

	void ResetAfterAttack() override {
		timer = 0;
		actionTimer = 0;
		//after executing attack1
		this->velocityX = this->normalVelocityX;

		//idle after attack

		TurnOnFlag(0);
	}

	void ExecuteAttack1(Character* character, float delt) override {//Dash attack

		if (!flags[1]) {
			FaceDirection(LocateCharacterDir(character));
		}
		
		this->TurnOnFlag(1);
		this->velocityX = this->dashVelocity;

		if (direction == Direction::Left && this->getLeftMostX() - 5.f > bounded.x) {
			this->Entity::MoveLeft(delt);
		}
		else if (direction == Direction::Right && this->getRightMostX() + 5.f < bounded.y) {
			this->Entity::MoveRight(delt);
		}
		else {
			FinishAttack(ATTACK1);
		}

	}

	void ExecuteAttack2(Character* character, float delt) override {//Static fire
		auto dir = LocateCharacterDir(character);

		if (dir != direction) this->FaceDirection(dir);

		this->TurnOnFlag(2);

		this->Shoot(delt);
	}

	void ExecuteAttack3(Character* character, float delt) override {
		attack3_timer += 600 * delt;

		// 0  -->  1/3 = dash;  1/3 -->  2/3 = jump; 2/3 -> 1/1 duration = dash
	
		if (attack3_timer <= actionDuration[ATTACK3]/3) {
			//dashing

			this->TurnOnFlag(1); //dashing animation

			if (direction == Direction::Left) {
				MoveLeft(delt);
			}
			else MoveRight(delt);
		}
		else if(attack3_timer <= 2*actionDuration[ATTACK3]/3) {
			//Jumping

			this->TurnOnFlag(5); //jumping animation

			this->Jump(delt);
		}
		else if (attack3_timer <= actionDuration[ATTACK3]) {
			//Dashing

			this->TurnOnFlag(1); //dashing animation

			if (direction == Direction::Left) {
				MoveLeft(delt);
			}
			else MoveRight(delt);
		}
		else {
			//reset
			attack3_timer = 0;
		}
	}

	void ExecuteAttack4(Character* character, float delt) override {}

	void InnitBossAttributes(float x, float y) override {
		//-----------//

		this->delay = 2000.f;
		this->health = 1000;
		this->velocityX = 80.f;
		this->collisiondamage = 30.f;
		//----------//
		//vile's hitbox should be smaller than sprite (54,45)
		this->dilation = normal_dilation;
		this->Entity::setPosition({ x,y });
		
	}

	void InnitBossCommands() override {
		TurnOnFlag(0);
		commands.push_back(BOSSCOMMANDS::ATTACK1);
		commands.push_back(BOSSCOMMANDS::ATTACK2);
	}

	void InnitBossWeapon() override{
		cannon = new ElectricCannon(textureManager);

		//vile can use the cannon faster

		cannon->SetClockDelay(0.6f);
	}

	void InnitActionDuration() override  {
		//command0 = idle doesnt need duration

		actionDuration[ATTACK1] = 4000.f;
		actionDuration[ATTACK2] = 4000.f;
		actionDuration[ATTACK3] = 5100.f;
	}
	

	void InnitAnimation() override {
		//idle
		movingAnimation->AddAnimation("Command0_left", "Animation\\Map1\\Vile\\Idle_Left.png", 100.f, 0, 0, 0,0, 54, 45);
		movingAnimation->AddAnimation("Command0_right", "Animation\\Map1\\Vile\\Idle_Right.png", 100.f, 0, 0, 0, 0, 54, 45);

		//attack1 - dash
		movingAnimation->AddAnimation("Command1_left", "Animation\\Map1\\Vile\\DashLeft.png", 100.f, 0, 0, 3, 0, 54, 45);
		movingAnimation->AddAnimation("Command1_right", "Animation\\Map1\\Vile\\DashRight.png", 100.f, 0, 0, 3, 0, 54, 45);


		//attack2 - shoot
		movingAnimation->AddAnimation("Command2_left", "Animation\\Map1\\Vile\\ShootLeft.png", 100.f, 0, 0, 2, 0, 54, 45);
		movingAnimation->AddAnimation("Command2_right", "Animation\\Map1\\Vile\\ShootRight.png", 100.f, 0, 0, 2, 0, 54, 45);
	}
};