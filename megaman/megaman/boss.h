#pragma once
#include "attackenemies.h"

enum BOSSCOMMANDS {
	IDLE = 0,ATTACK1 = 1, ATTACK2 = 2, ATTACK3 = 3, ATTACK4 = 4, JUMP = 5
};

class Boss : public Enemy {
protected:
	std::vector<BOSSCOMMANDS> commands; 

	//---Map-related--//


	//boss's region
	Vector2f bounded = { 0.f, 0.f };

	//---Timer---//

	float delay;//there should be delays between commands
	float timer = 0;

	std::map<BOSSCOMMANDS, float> actionDuration; //the time of each action
	float actionTimer = 0;

	int cur = 0; //current command


	//command flags

	bool flags[6] = { false };
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
protected:
	

	void TurnOnFlag(short int i) {
		if (!(0 <= i && i <= 5)) return;

		for (auto& x : flags) x = false;
		this->flags[i] = true;
	}

	void FinishAttack(BOSSCOMMANDS cmd) {
		this->actionTimer = actionDuration[cmd];
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