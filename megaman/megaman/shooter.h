#pragma once
#include "enumdirection.h"
#include "projectile.h"
#include <iostream>
#include <SFML/Graphics.hpp>

#include <stack>

class Shooter {
protected:
	int activeBullets;
	std::vector<Projectile*> bullets;
	bool yesAnimation = false;

	//---if cntDelay < delay, does not allow shoot!
	float delay = 0.5f;
	float cntDelay = 0.f;
	//-------------

	TextureManager* textureManager;

	Projectile* sampleBullet = nullptr;
public:
	Shooter(TextureManager* textureManager): textureManager(textureManager) {
		activeBullets = 1;
		sampleBullet = new Projectile(textureManager, 0.f, 500.f, 0.f);
		

		for (int i = 0; i < 1; ++i) {
			Projectile* tmp = nullptr;
			bullets.push_back(tmp);
		}

		for (int i = 0; i < 1; ++i) {
			bullets[i] = new Projectile(*sampleBullet);
		}
	}

	virtual ~Shooter() {
		std::cout << "Destructor of the Shooter (gun).\n";
		for (auto b: bullets) {
			if(b) delete b;
		}

		delete sampleBullet;
	}

	//render

	virtual void RenderProjectiles(RenderWindow* l_window) {
		int n = bullets.size();
		for (int i = n - 1; i >= activeBullets; --i) {
			bullets[i]->Render(l_window);
		}
	}

	//load

	virtual void Load(Direction dir) {
		Projectile* tmp = nullptr;
		bullets.push_back(tmp);
		
		bullets.back() = new Projectile(*sampleBullet);
		++activeBullets;

		bullets.back()->FaceDirection(dir);
		std::swap(bullets[activeBullets - 1], bullets.back());
	}

	//shoot

	void Shoot(Direction dir) {



		if (activeBullets == 0) {
		//temporarily, we only give 10 bullets
			return;
		}


		int n = bullets.size();

		bullets[activeBullets-1]->FaceDirection(dir);
		--activeBullets;
		
		//load
		Load(dir);

		std::cout << "Number of bullets in vector: " << (int)bullets.size() << '\n';
	}

	//make it virtual for bullets that use animations
	virtual void UpdateMovingProjectiles(float delt, Vector2f pos) {
		int n = bullets.size();
		for (int i = n - 1; i >= activeBullets; --i) {
			bullets[i]->ProjectileFly(delt, pos);
			bullets[i]->Entity::UpdateMovements(delt);
		}
	}

	virtual void HandleProjectileCollision(Obstacle* obs, Entity* en) {
		int n = bullets.size();
		for (int i = n - 1; i >= activeBullets; --i) {
			if (bullets[i]->IsHit(obs) || bullets[i]->IsHit(en)) {

				//pop that bullet out
				std::swap(bullets[i], bullets[n - 1]);

				//this bullet go out of scope, hence deleted
				delete bullets.back();
				bullets.pop_back();
				--n;
			}
		}
	}

	virtual void HandleProjectileCollision(Obstacle* obs) {
		int n = bullets.size();
		for (int i = n - 1; i >= activeBullets; --i) {
			if (bullets[i]->IsHit(obs)) {

				//pop that bullet out
				std::swap(bullets[i], bullets[n - 1]);

				//this bullet go out of scope, hence deleted
				delete bullets.back();
				bullets.pop_back();
				--n;
			}
		}
	}

	virtual void HandleProjectileCollision(Entity* en) {
		int n = bullets.size();
		for (int i = n - 1; i >= activeBullets; --i) {
			if (bullets[i]->IsHit(en)) {

				//pop that bullet out
				std::swap(bullets[i], bullets[n - 1]);

				//this bullet go out of scope, hence deleted
				delete bullets.back();
				bullets.pop_back();
				--n;
			}
		}
	}

	//allow bullets animation

	void LoadAnimationForBullet(const std::string& l_file, const std::string& r_file
		, float animationTimer, int start_frame_x, int start_frame_y,
		int frames_x, int frames_y, int _width, int _height) {
		

		sampleBullet->AddAnimations(l_file, r_file, animationTimer, start_frame_x, start_frame_y,
			frames_x, frames_y, _width, _height);

		//edit the frame

		sampleBullet->setSize({ float(_width),float(_height) });

		if (!bullets.empty()) {
			bullets.back()->AddAnimations(l_file, r_file, animationTimer, start_frame_x, start_frame_y,
				frames_x, frames_y, _width, _height);
			bullets.back()->setSize({float(_width),float(_height)});
		}
	}
};

//-------WEAPON--------

class XBuster : public Shooter {
public:
	class FullChargeBuster : public Shooter {
	public:
		FullChargeBuster(TextureManager* textureManager): Shooter(textureManager) {
			if (this->sampleBullet) {
				delete this->sampleBullet;
				this->sampleBullet = new Projectile(textureManager, 0, 550.f, 0); //faster
			}
			LoadAnimationForBullet("Animation\\X\\XBuster_FullChargeLeft.png", "Animation\\X\\XBuster_FullChargeRight.png",
				70.f, 0, 0, 5, 0, 150, 90);
		}
		~FullChargeBuster() {

		}
	};
	class SemiChargeBuster : public Shooter {
	public:
		SemiChargeBuster(TextureManager* textureManager): Shooter(textureManager) {

		}
		~SemiChargeBuster() {

		}
	};
	Clock clk;
	float maxTime = 2.f;
	float deltCharge = 0.f;

	FullChargeBuster* fullcharge;
	SemiChargeBuster* semicharge;
public:
	XBuster(TextureManager* textureManager): Shooter(textureManager) {
		
		fullcharge = new FullChargeBuster(textureManager);
		semicharge = new SemiChargeBuster(textureManager);
	}
	
	~XBuster() override {
		delete fullcharge;
		delete semicharge;
	}

	void Charge() {
		clk.restart();
	}

	void UnCharge() {
		deltCharge = std::min(maxTime, clk.getElapsedTime().asSeconds());
	}

	void ChargeShoot(Direction dir){
		if (deltCharge < 0.2f) {
			return; //delay
		}

		if (deltCharge > 0.7f) {
			std::cout << deltCharge << '\n';
			fullcharge->Shoot(dir);
		}
	}

	//overrides

	void UpdateMovingProjectiles(float delt, Vector2f pos) override {
		this->Shooter::UpdateMovingProjectiles(delt, pos);
		fullcharge->Shooter::UpdateMovingProjectiles(delt, pos);
		//semicharge->Shooter::UpdateMovingProjectiles(delt, pos);
	}

	void HandleProjectileCollision(Obstacle* obs, Entity* en) override {
		this->Shooter::HandleProjectileCollision(obs, en);
		fullcharge->Shooter::HandleProjectileCollision(obs, en);
		//semicharge->Shooter::HandleProjectileCollision(obs, en);
	}
	
	void HandleProjectileCollision(Obstacle* obs) override {
		this->Shooter::HandleProjectileCollision(obs);
		fullcharge->Shooter::HandleProjectileCollision(obs);
		//semicharge->Shooter::HandleProjectileCollision(obs);
	}

	void HandleProjectileCollision(Entity* en) override {
		this->Shooter::HandleProjectileCollision(en);
		fullcharge->Shooter::HandleProjectileCollision(en);
		//semicharge->Shooter::HandleProjectileCollision(en);
	}

	void RenderProjectiles(RenderWindow* l_window) override {
		int n = bullets.size();
		for (int i = n - 1; i >= activeBullets; --i) {
			bullets[i]->Render(l_window);
		}
		fullcharge->RenderProjectiles(l_window);
	}
};