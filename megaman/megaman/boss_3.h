#include "boss.h"

class SpikeShooter : public Shooter {
private:
	sf::Clock clk; // for delay

	float delay = 0.75;
	bool firstShot = false;

	std::vector<Vector2f> coords; //size = 9
public:
	SpikeShooter(TextureManager* textureManager, std::vector<Vector2f>& coords) :
		Shooter(textureManager, 7, 55, 50, 0, 0) {

		for (auto& x : coords) this->coords.push_back(x);

		this->damage = 30;

		this->LoadAnimationForBullet("Animation\\Map3\\Chill_Penguin\\ice_spike.png", "Animation\\Map3\\Chill_Penguin\\ice_spike.png",
			100.f, 0, 0, 0, 0, 7, 55);
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

		for (int i = 0; i < 3; ++i) this->Shooter::Shoot(dir); //three times
	}

	void UpdateMovingProjectiles(float delt, Vector2f pos) override {
		int n = bullets.size();

		int tmp = 0;
		short int randid = std::rand() % 3;

		for (int i = n - 1; i >= activeBullets; --i) {
			Vector2f p = coords[0];
			if(randid + 3 * tmp < (int)coords.size()) p = coords[randid + 3 * tmp];
			bullets[i]->ProjectileFly(delt, p); //this moves the projectile
			bullets[i]->Entity::UpdateEntity(delt); //this plays animation of bullets
			++tmp;
		}
	}
};

class ChillPenguin : public Boss {
private:
	//attack 1

	std::vector<Vector2f> coords = { {5950.f,10.f},  {5980.f,10.f}, {6010.f,10.f},
									{6040.f,10.f}, {6070.f,10.f} , {6100.f,10.f} ,
									{6130.f,10.f} , {6160.f,10.f} , {6190.f,10.f} };
	IceShotgun* ice_shotgun;
	SpikeShooter* spike_shooter;


	//attack 2

	float timer2 = 0;
	const float timerPrepare2 = 1200.f;
	

	//attack 3

	float timer3 = 0;
	const float timerPrepare3 = 800.f;
public:
	ChillPenguin(TextureManager* textureManager, float x, float y, Vector2f bounded) :
		Boss(textureManager, x, y, 24, 37, 300, bounded, 2000) 
	{

		InnitBossAttributes(x, y);

		InnitActionDuration();

		InnitBossCommands();

		InnitBossWeapon();

		InnitAnimation();
	}

	~ChillPenguin() {
		delete ice_shotgun;
		delete spike_shooter;
	}

	void Update(Character* character, float delt) override {
		if (!this->IsDead()) {
			this->Boss::UpdateEntity(delt);
			this->UpdateEnemyBehaviour(character, delt);
			this->UpdateEnemyProjectiles(delt);
		}
		else {
			this->Boss::UpdateDeath(delt);
		}
	}

	void Render(RenderWindow* l_window) override {
		//l_window->draw(this->frame);
		if (!this->IsDead() || finishDeathAnimation == false) {
			l_window->draw(this->sprite);
			if (!this->IsDead()) {
				spike_shooter->RenderProjectiles(l_window);
				ice_shotgun->RenderProjectiles(l_window);
			}
		}
	}

	//-----------SHOOTING-----------//

	void HandleProjectileCollision(Obstacle* obs, Entity* en) override {
		spike_shooter->HandleProjectileCollision(obs, en);
		ice_shotgun->HandleProjectileCollision(obs, en);
	}

	void HandleProjectileCollision(Entity* en) override {
		spike_shooter->HandleProjectileCollision(en);
		ice_shotgun->HandleProjectileCollision(en);
	}

	void HandleProjectileCollision(Obstacle* obs) override {
		spike_shooter->HandleProjectileCollision(obs);
		ice_shotgun->HandleProjectileCollision(obs);
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

		spike_shooter->UpdateMovingProjectiles(delt, { 0.f,0.f }); //pos doesnt matter in this weapon
		if(this->direction == Direction::Left) ice_shotgun->UpdateMovingProjectiles(delt, this->getUpLeftPosition());
		else ice_shotgun->UpdateMovingProjectiles(delt, this->getUpRightPosition());
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
		if (com == BOSSCOMMANDS::ATTACK1) { //drop ice spikes
			this->ExecuteAttack1(character, delt);
		}
		if (com == BOSSCOMMANDS::ATTACK2) { //dash
			this->ExecuteAttack2(character, delt);
		}
		if (com == BOSSCOMMANDS::ATTACK3) {
			this->ExecuteAttack3(character, delt);
		}
	}

	void ResetAfterAttack() override {
		timer = 0; timer2 = 0; timer3 = 0;
		actionTimer = 0;
		//idle after attack

		TurnOnFlag(0);
	}


	void ExecuteAttack1(Character* character, float delt) override { //spit oil
		auto dir = LocateCharacterDir(character);

		if (dir != direction) this->FaceDirection(dir);

		this->TurnOnFlag(1);

		spike_shooter->Shoot(this->direction);
	}

	void ExecuteAttack2(Character* character, float delt) override { //shoot fireball
		//face direction only once
		if (timer2 == 0) {
			auto dir = LocateCharacterDir(character);
			if (this->direction != dir) FaceDirection(dir);
		}

		//prepare + dash = actionDuration
		timer2 += 600 * delt;
		//prepare
		

		if (timer2 > timerPrepare2) {
			//play dash animation
			
			this->TurnOnFlag(2);

			//dashes

			if (this->direction == Direction::Left) {
				if (this->getUpLeftPosition().x - 5.f <= bounded.x) {
					FaceDirection(Direction::Right);
				}
				else MoveLeft(delt);
			}
			if (this->direction == Direction::Right) {
				if (this->getUpRightPosition().x + 5.f >= bounded.y) {
					FaceDirection(Direction::Left);
				}
				else MoveRight(delt);
			}
		}
		else this->TurnOnFlag(4);
	}

	void ExecuteAttack3(Character* character, float delt) override {
		//face direction only once
		if (timer3 == 0) {
			auto dir = LocateCharacterDir(character);
			if (this->direction != dir) FaceDirection(dir);
		}

		//prepare + jump = actionDuration
		timer3 += 600 * delt;
		//prepare


		if (timer3 > timerPrepare3) {
			
			//jump animation
			this->TurnOnFlag(3);

			//shoot
			if(timer3 > timerPrepare3 + 200.f) this->ice_shotgun->Shoot(this->direction);

			if (this->getLeftMostX() - 5.f < bounded.x || this->getRightMostX() + 5.f > bounded.y) {
				if (this->isJumping == true) FinishAttack(ATTACK3);

				if (this->getLeftMostX() - 5.f < bounded.x) {
					this->setPosition({ bounded.x + 10.f, this->getPosition().y });
				}
				else if (this->getRightMostX() + 5.f > bounded.y) {
					this->setPosition({ bounded.y - this->getFrameSize().y - 10.f, this->getPosition().y });
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
		else this->TurnOnFlag(5); //prepare animation
	}

	void ExecuteAttack4(Character* character, float delt) override {}

	void InnitBossAttributes(float x, float y) override {
		this->gravity = 80.f;
		this->delay = 2000.f;
		this->velocityX = 200.f;
		this->health = 1500;
		this->jumpStrength = 100.f;
		this->collisiondamage = 30.f;
		this->deathAnimationMaxTimer = 4800.f;

		this->dilation = { 10.f,0.f };
		setPosition({ x,y });
	}

	void InnitBossWeapon() override {
		this->spike_shooter = new SpikeShooter(textureManager, coords);
		this->ice_shotgun = new IceShotgun(textureManager);
	}

	void InnitBossCommands() override {
		TurnOnFlag(0);

		commands.push_back(BOSSCOMMANDS::ATTACK1);
		commands.push_back(BOSSCOMMANDS::ATTACK2);
		commands.push_back(BOSSCOMMANDS::ATTACK3);
		commands.push_back(BOSSCOMMANDS::ATTACK2);
		commands.push_back(BOSSCOMMANDS::ATTACK2);
		commands.push_back(BOSSCOMMANDS::ATTACK1);
		commands.push_back(BOSSCOMMANDS::ATTACK3);
	}

	void InnitActionDuration() override {
		//command0 = idle doesnt need duration

		actionDuration[ATTACK1] = 2400.f;
		actionDuration[ATTACK2] = 3600.f;
		actionDuration[ATTACK3] = 3600.f;
	}

	void InnitAnimation() override {
		movingAnimation->AddAnimation("Command0_left", "Animation\\Map3\\Chill_Penguin\\idle_left.png"
			, 200.f, 0, 0, 0, 0, 44, 37);
		movingAnimation->AddAnimation("Command0_right", "Animation\\Map3\\Chill_Penguin\\idle_right.png"
			, 200.f, 0, 0, 0, 0, 44, 37);


		movingAnimation->AddAnimation("Command1_left", "Animation\\Map3\\Chill_Penguin\\roar_left.png"
			,200.f, 0, 0, 5, 0, 44, 37);
		movingAnimation->AddAnimation("Command1_right", "Animation\\Map3\\Chill_Penguin\\roar_right.png"
			, 200.f, 0, 0, 5, 0, 44, 37);

		//attack 2 but prepare (use flag 4)

		movingAnimation->AddAnimation("Command4_left", "Animation\\Map3\\Chill_Penguin\\prepare_dash_left.png"
			, 200.f, 0, 0, 5, 0, 44, 37);
		movingAnimation->AddAnimation("Command4_right", "Animation\\Map3\\Chill_Penguin\\prepare_dash_right.png"
			, 200.f, 0, 0, 5, 0, 44, 37);

		//attack 2
		movingAnimation->AddAnimation("Command2_left", "Animation\\Map3\\Chill_Penguin\\dash_left.png"
			, 200.f, 0, 0, 0, 0, 44, 37);
		movingAnimation->AddAnimation("Command2_right", "Animation\\Map3\\Chill_Penguin\\dash_right.png"
			, 200.f, 0, 0, 0, 0, 44, 37);

		//prepare jump
		movingAnimation->AddAnimation("Command5_left", "Animation\\Map3\\Chill_Penguin\\prepare_jump_left.png"
			, 200.f, 0, 0, 3, 0, 44, 37);
		movingAnimation->AddAnimation("Command5_right", "Animation\\Map3\\Chill_Penguin\\prepare_jump_right.png"
			, 200.f, 0, 0, 3, 0, 44, 37);

		//attack 3
		movingAnimation->AddAnimation("Command3_left", "Animation\\Map3\\Chill_Penguin\\shoot_left.png"
			, 200.f, 0, 0, 0, 0, 44, 37);
		movingAnimation->AddAnimation("Command3_right", "Animation\\Map3\\Chill_Penguin\\shoot_right.png"
			, 200.f, 0, 0, 0, 0, 44, 37);

		//die
		movingAnimation->AddAnimation("Command6_left", "Animation\\Map3\\Chill_Penguin\\die_left.png", 150.f, 0, 0, 2, 0, 42, 46);
		movingAnimation->AddAnimation("Command6_right", "Animation\\Map3\\Chill_Penguin\\die_right.png", 150.f, 0, 0, 2, 0, 42, 46);
	}
};
