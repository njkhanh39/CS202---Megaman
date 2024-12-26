#pragma once
#include "attackenemies.h"

enum BOSSCOMMANDS {
	IDLE = 0,ATTACK1 = 1, ATTACK2 = 2, ATTACK3 = 3, ATTACK4 = 4, JUMP = 5, DIE = 6
};

class Boss : public Enemy {
protected:
	std::vector<BOSSCOMMANDS> commands; 

	//---Map-related--//


	//boss's region
	Vector2f bounded = { 0.f, 0.f };



	//die
	float deathAnimationMaxTimer = 400.f;
	float death_timer = 0.f;
	bool finishDeathAnimation = false;

	//attack / executions
	float delay;//there should be delays between commands
	float timer = 0;

	std::map<BOSSCOMMANDS, float> actionDuration; //the time of each action
	float actionTimer = 0;

	int cur = 0; //current command


	//command flags

	bool flags[7] = { false };
public:
	Boss(TextureManager* textureManager, float x, float y, float sizex, float sizey, float _viewRange, Vector2f bounded
		, float delay): 
		Enemy(textureManager,x,y,sizex,sizey, _viewRange), delay(delay), bounded(bounded){



	}

	//void Update(Character* character, float delt) override {}

	//virtual void Render(RenderWindow* l_window) override {}

	//-----------SHOOTING-----------//

	//void HandleProjectileCollision(Obstacle* obs, Entity* en) override {}

	//void HandleProjectileCollision(Entity* en) override {}

	//void HandleProjectileCollision(Obstacle* obs) override {}

	bool FinishedDeathAnimation() {
		return this->finishDeathAnimation;
	}
protected:
	

	void TurnOnFlag(short int i) {
		if (!(0 <= i && i <= 6)) return;

		for (auto& x : flags) x = false;
		this->flags[i] = true;
	}

	void FinishAttack(BOSSCOMMANDS cmd) {
		this->actionTimer = actionDuration[cmd];
	}

	void UpdateDeath(float delt) { //play this only once
		if (this->finishDeathAnimation) return;

		death_timer += 600 * delt;

		if (death_timer <= deathAnimationMaxTimer) {
			if(direction == Direction::Left) movingAnimation->Play("Command6_left", delt);
			else movingAnimation->Play("Command6_right", delt);
		}
		else {
			death_timer = 0;
			this->finishDeathAnimation = true;
		}
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

		for (int i = 0; i < 7; ++i) {
			if (flags[i]) {
				movingAnimation->Play("Command" + std::to_string(i) + dr, delt);
				break;
			}
		}
	}

	virtual void ExecuteCommand(Character* character, float delt, BOSSCOMMANDS com) = 0;


	virtual void ResetAfterAttack() = 0; //after attacking, some values might been change (ex: velocity x
												// is larger since boss uses dash, needs to set back to normal)

	virtual void ExecuteAttack1(Character* character, float delt) = 0;

	virtual void ExecuteAttack2(Character* character, float delt) = 0;

	virtual void ExecuteAttack3(Character* character, float delt) = 0;

	virtual void ExecuteAttack4(Character* character, float delt) = 0;	
	

	virtual void InnitBossAttributes(float x, float y) = 0;

	virtual void InnitBossCommands() = 0;

	virtual void InnitBossWeapon() = 0;
	
	virtual void InnitActionDuration() = 0;

	//void innit animation
};