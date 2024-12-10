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

	const float dashVelocity = 300.f;
	const float normalVelocityX = 120.f;

	//---Weapon
	ElectricCannon* cannon;
public:
	Vile(TextureManager* textureManager, float x, float y, std::vector<BOSSCOMMANDS>& coms, float delay):
		Boss(textureManager, x, y, 20,40, 300, coms, delay) //no scaling
	{
		//vile's hitbox should be smaller than sprite (54,45)

		
		//-----------//


		this->health = 1000;
		this->velocityX = 120.f;
		//----------//

		//Innit the duration for each attack


		actionDuration[ATTACK1] = 100.f; //for example


		//


		this->dilation = normal_dilation;
		this->Entity::setPosition({ x,y });

		cannon = new ElectricCannon(textureManager);

		//vile can use the cannon faster

		cannon->SetClockDelay(0.5f);
	}

	~Vile() {
		delete cannon;
	}

	void Update(Character* character, float delt) override {
		this->AttackCharacter(character, delt); //bro has no life, all he does is attack X
	}

	virtual void Render(RenderWindow* l_window) override {
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

		for (int i = 0; i < 4; ++i) {
			if (flags[i]) {
				movingAnimation->Play("Command" + std::to_string(i + 1) + dr, delt);
				break;
			}
		}
	}

	virtual void AttackCharacter(Character* character, float delt) {
		timer += 600 * delt;

		if (timer >= delay) {

			BOSSCOMMANDS curcmd = commands[cur];

			actionTimer += 600 * delt;

			//continuous behavior (constantly doing)

			//this->Shoot(delt); //its ok, we have the cannon's clock. Or at least u can set duration = 0, then we done

			this->ExecuteCommand(character, delt, curcmd);


			if (actionTimer >= actionDuration[curcmd]) {
				//reset

				timer = 0;
				actionTimer = 0;
				this->ResetAfterAttack();
				++cur;
				if (cur > (int)commands.size()) cur = 0;
			}
		}
	}

	//virtual void UpdateEnemyBehaviour(Character* character, float delt) {}

	void ExecuteCommand(Character* character, float delt, BOSSCOMMANDS com) override {
		if (com == BOSSCOMMANDS::ATTACK1) {
			this->ExecuteAttack1(character, delt);
		}
	}

	void ResetAfterAttack() override {
		//after executing attack1
		this->velocityX = this->normalVelocityX;
	}

	void ExecuteAttack1(Character* character, float delt) override {//Dash attack
		auto dir = LocateCharacterDir(character);

		if (dir != direction) this->FaceDirection(dir);

		this->TurnOnFlag(0);

		this->velocityX = dashVelocity;

		if (dir == Direction::Left && this->getLeftMostX() - 5.f > bounded.x) { //meet wall = stawp
			this->Entity::MoveLeft(delt);
		}
		else if (dir == Direction::Right && this->getRightMostX() + 5.f < bounded.y) {
			this->Entity::MoveRight(delt);
		}
	}

	void ExecuteAttack2(Character* character, float delt) override {//Static fire
		auto dir = LocateCharacterDir(character);

		if (dir != direction) this->FaceDirection(dir);

		this->TurnOnFlag(1);

		this->Shoot(delt);
	}
	

	virtual void InnitAnimation() {
		movingAnimation->AddAnimation("Command1_left", "Animation\\Map1\\Vile\\DashLeft.png", 70.f, 0, 0, 3, 0, 54, 45);
	
	}
};