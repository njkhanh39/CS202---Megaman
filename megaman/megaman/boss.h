#pragma once
#include "attackenemies.h"

enum BOSSCOMMANDS {
	ATTACK1 = 1, ATTACK2 = 2, ATTACK3 = 3, ATTACK4 = 4
};

class Boss : public Enemy {
protected:
	std::vector<BOSSCOMMANDS> commands; 

	//---Map-related--//

	Vector2f bounded = { 0.f, 0.f };

	//---Timer---//

	const float delay;//there should be delays between commands
	float timer = 0;

	std::map<BOSSCOMMANDS, float> actionDuration; //the time of each action
	float actionTimer = 0;

	int cur = 0; //current command


	//command flags

	bool flags[4] = { false };
public:
	Boss(TextureManager* textureManager, float x, float y, float sizex, float sizey, float _viewRange
		, std::vector<BOSSCOMMANDS>& coms, float delay): 
		Enemy(textureManager,x,y,sizex,sizey, _viewRange), delay(delay){

		for (auto& c : coms) {
			this->commands.push_back(c);
		}

	}

	//void Update(Character* character, float delt) override {}

	//virtual void Render(RenderWindow* l_window) override {}

	//-----------SHOOTING-----------//

	//void HandleProjectileCollision(Obstacle* obs, Entity* en) override {}

	//void HandleProjectileCollision(Entity* en) override {}

	//void HandleProjectileCollision(Obstacle* obs) override {}
protected:

	//virtual void AttackCharacter(Character* character, float delt) {
	//	timer += 600 * delt;

	//	if (timer >= delay) {

	//		BOSSCOMMANDS curcmd = commands[cur];

	//		actionTimer += 600 * delt;

	//		if (actionTimer >= actionDuration[curcmd]) {

	//			//Action



	//			//reset
	//			timer = 0;
	//			actionTimer = 0;
	//		}
	//	}
	//}

	//virtual void UpdateEnemyBehaviour(Character* character, float delt) {}

	//virtual void InnitAnimation() {}

	////------------------

	void TurnOnFlag(short int i) {
		if (!(0 <= i && i <= 3)) return;

		for (auto& x : flags) x = false;
		this->flags[i] = true;
	}

	virtual void ExecuteCommand(Character* character, float delt, BOSSCOMMANDS com) = 0;


	virtual void ResetAfterAttack() = 0; //after attacking, some values might been change (ex: velocity x
												// is larger since boss uses dash, needs to set back to normal)

	virtual void ExecuteAttack1(Character* character, float delt) = 0;

	virtual void ExecuteAttack2(Character* character, float delt) = 0;

	virtual void ExecuteAttack3(Character* character, float delt) = 0;

	virtual void ExecuteAttack4(Character* character, float delt) = 0;
};