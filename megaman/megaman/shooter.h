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

	Projectile* sampleBullet = nullptr;
public:
	Shooter() {
		activeBullets = 1;
		sampleBullet = new Projectile(0.f, 500.f, 0.f);
		

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

	void RenderProjectiles(RenderWindow* l_window) {
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

	void HandleProjectileCollision(Obstacle* obs, Entity* en) {
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

	void HandleProjectileCollision(Obstacle* obs) {
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

	void HandleProjectileCollision(Entity* en) {
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

	void LoadAnimationForSampleBullet(const std::string& l_file, const std::string& r_file
		, float animationTimer, int start_frame_x, int start_frame_y,
		int frames_x, int frames_y, int _width, int _height) {
		

		sampleBullet->AddAnimations(l_file, r_file, animationTimer, start_frame_x, start_frame_y,
			frames_x, frames_y, _width, _height);

		//edit the frame

		sampleBullet->setSize({ float(_width),float(_height) });
	}
};

//-------WEAPON--------

class XBuster : public Shooter {
private:
	class FullChargeBuster : public Shooter {
	public:
		FullChargeBuster() {
			//bulletVelocityX = 900.f; //make it a little faster
			if (sampleBullet) {
				delete sampleBullet;
				sampleBullet = new Projectile(0, 900.f, 0); //faster
			}

			
			
		}

	};
public:

	
};