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

	virtual void Shoot(Direction dir);


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

	void Charge();

	void UnCharge();

	void ChargeShoot(Direction dir);

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
			if (bullets[i]->IsHit(en) && en && !en->isInvisible()) {
				en->TakeDamage(this->damage);
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
			if (bullets[i]->IsHit(en) && en && !en->isInvisible()) {
				en->TakeDamage(this->damage);
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
