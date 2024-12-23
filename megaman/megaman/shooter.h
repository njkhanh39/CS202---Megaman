#pragma once
#include "enumdirection.h"
#include "projectile.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <memory>
#include <stack>

class Shooter {
protected:
	int activeBullets;
	std::vector<Projectile*> bullets;
	bool yesAnimation = false;

	//-------------

	int damage = 5;

	TextureManager* textureManager;

	Projectile* sampleBullet = nullptr;
public:
	Shooter(TextureManager* textureManager);
	Shooter(TextureManager* textureManager, float bulletsizeX, float bulletsizeY);
	Shooter(TextureManager* textureManager, float bulletsizeX, float bulletsizeY, float gravity, float velox, float veloy);

	virtual ~Shooter();

	//render

	virtual void RenderProjectiles(RenderWindow* l_window);


	//shoot
	virtual void ChargeShoot(Direction dir);

	virtual void Shoot(Direction dir);

	virtual void Charge(){}

	virtual void UnCharge() {}

	virtual void UpdateMovingProjectiles(float delt, Vector2f pos);

	//call these in world

	//some projectiles can pierce through stuffs, so we need to make it virtual

	virtual void HandleProjectileCollision(Obstacle* obs, Entity* en);

	virtual void HandleProjectileCollision(Obstacle* obs);

	virtual void HandleProjectileCollision(Entity* en);

	//allow bullets animation


	void LoadLeftTexture(const std::string& file);

	void LoadRightTexture(const std::string& file);

	void LoadAnimationForBullet(const std::string& l_file, const std::string& r_file
		, float animationTimer, int start_frame_x, int start_frame_y,
		int frames_x, int frames_y, int _width, int _height);


	//scale animation

	virtual void ScaleProjectileAnimation(float f1, float f2);

	//setters

	void SetVelocityX(float v) {
		sampleBullet->setVelocityX(v);
		for (auto& x : bullets) {
			x->setVelocityX(v);
		}
	}

	void SetVelocityY(float v) {
		sampleBullet->setVelocityY(v);
		for (auto& x : bullets) {
			x->setVelocityY(v);
		}
	}

	void SetVelocityX_One_ActiveBullet(float v) {
		if (activeBullets == 0) {
			return;
		}
		bullets[activeBullets - 1]->setVelocityX(v);
	}

	void SetVelocityY_One_ActiveBullet(float v) {
		if (activeBullets == 0) {
			return;
		}
		bullets[activeBullets - 1]->setVelocityY(v);
	}
protected:
	//load

	virtual void Load(Direction dir);
};

//-------WEAPON--------

class XBuster : public Shooter {
public:
	class FullChargeBuster : public Shooter {
	public:
		FullChargeBuster(TextureManager* textureManager);

		~FullChargeBuster();

		//Full charge is piercing

		void HandleProjectileCollision(Obstacle* obs, Entity* en) override;

		void HandleProjectileCollision(Entity* en) override;
	};
	class SemiChargeBuster : public Shooter {
	public:
		SemiChargeBuster(TextureManager* textureManager);

		~SemiChargeBuster();
	};
	Clock clk; //for charging
	float maxTime = 4.f;
	float deltCharge = 0.f;

	FullChargeBuster* fullcharge;
	SemiChargeBuster* semicharge;
public:
	XBuster(TextureManager* textureManager);
	
	~XBuster() override;

	void Charge() override;

	void UnCharge() override;

	void ChargeShoot(Direction dir) override;

	//overrides

	void UpdateMovingProjectiles(float delt, Vector2f pos) override;

	void HandleProjectileCollision(Obstacle* obs, Entity* en) override;
	void HandleProjectileCollision(Obstacle* obs) override;

	void HandleProjectileCollision(Entity* en) override;

	void RenderProjectiles(RenderWindow* l_window) override;

	void ScaleProjectileAnimation(float f1, float f2) override;
};

class SpecialShooter : public Shooter {
public:
	SpecialShooter(TextureManager* textureManager, float bulletsizeX, float bulletsizeY, float gravity, float velox, float veloy) :
		Shooter(textureManager, bulletsizeX,  bulletsizeY,  gravity,  velox,  veloy) {
	}

	~SpecialShooter() {

	}
	void HandleProjectileCollision(Obstacle* obs, Entity* en) override {
		int n = bullets.size();
		for (int i = n - 1; i >= activeBullets; --i) {
			if (bullets[i]->IsHit(en)) {
				if(en && !en->isInvisible()) en->TakeDamage(this->damage);
				bullets[i]->setVelocityX(0);
			}
			if (bullets[i]->IsStopped()) {
				//pop that bullet out
				std::swap(bullets[i], bullets[n - 1]);

				//this bullet go out of scope, hence deleted
				delete bullets.back();
				bullets.pop_back();
				--n;
			}
			else {
				bool test = true;
				if (!bullets[i]->canMoveLeft(obs)) {
					test = false;
				}
				if (!bullets[i]->canKeepFalling(obs)) {
					test = false;
				}
				if (!bullets[i]->canMoveRight(obs)) {
					test = false;
				}

				if (!test) {
					bullets[i]->setVelocityX(0);
					bullets[i]->fall = false;
				}
			}
		}
	}

	void HandleProjectileCollision(Obstacle* obs) override {
		int n = bullets.size();
		for (int i = n - 1; i >= activeBullets; --i) {
			if (bullets[i]->IsStopped()) { //only when it ran out of time does we remove
				//pop that bullet out
				std::swap(bullets[i], bullets[n - 1]);

				//this bullet go out of scope, hence deleted
				delete bullets.back();
				bullets.pop_back();
				--n;
			}
			else {
				bool test = true;
				if (!bullets[i]->canMoveLeft(obs)) {
					test = false;
				}
				if (!bullets[i]->canKeepFalling(obs)) {
					test = false;
				}
				if (!bullets[i]->canMoveRight(obs)) {
					test = false;
				}

				if (!test) {
					bullets[i]->setVelocityX(0);
					bullets[i]->fall = false;
				}
			}
		}
	}

	void HandleProjectileCollision(Entity* en) override {
		int n = bullets.size();
		for (int i = n - 1; i >= activeBullets; --i) {
			if (bullets[i]->IsHit(en)) {
				if (en && !en->isInvisible()) en->TakeDamage(this->damage);
				bullets[i]->setVelocityX(0);
			}
			if (bullets[i]->IsStopped()) { //only when it ran out of time does we remove
				//pop that bullet out
				std::swap(bullets[i], bullets[n - 1]);

				//this bullet go out of scope, hence deleted
				delete bullets.back();
				bullets.pop_back();
				--n;
			}
		}
	}
};

class ElectricCannon : public Shooter {
private:
	sf::Clock clk; // for delay

	float delay = 1.f;
	bool firstShot = false;
public:
	ElectricCannon(TextureManager* textureManager) :
		Shooter(textureManager, 16.f, 16.f, 0, 150.f, 0.f) {

		//damage

		this->damage = 40;


		//animation

		this->LoadAnimationForBullet("Animation\\Own_By_Many\\electric_left.png",
			"Animation\\Own_By_Many\\electric_right.png"
			, 70.f, 0, 0, 3, 0, 16, 16); //doesnt need to scale
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

class FlameThrower : public SpecialShooter {
private:
	sf::Clock clk; // for delay

	float delay = 0.75;
	bool firstShot = false;
public:
	FlameThrower(TextureManager* textureManager) :
		SpecialShooter(textureManager, 16.f, 16.f, 0, 150.f, 0.f) {

		//damage

		this->damage = 15;


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

class IceShotgun : public Shooter {
private:
	sf::Clock clk; // for delay

	float delay = 0.75;
	bool firstShot = false;
public:
	IceShotgun(TextureManager* textureManager) :
		Shooter(textureManager, 7, 7, 0, 160, 0) {

		this->damage = 15;

		this->LoadAnimationForBullet("Animation\\Own_By_Many\\iceberg.png", "Animation\\Own_By_Many\\iceberg.png",
			100.f, 0, 0, 0, 0, 7, 7);
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

		this->SetVelocityY_One_ActiveBullet(-25);
		this->Shooter::Shoot(dir);

		this->SetVelocityY_One_ActiveBullet(0);
		this->Shooter::Shoot(dir);

		this->SetVelocityY_One_ActiveBullet(25);
		this->Shooter::Shoot(dir);


	}
};