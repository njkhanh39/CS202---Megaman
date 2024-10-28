#pragma once
#include "enumdirection.h"
#include "projectile.h"
#include <iostream>
#include <SFML/Graphics.hpp>

#include <stack>

class Shooter {
private:
	int activeBullets;
	std::vector<Projectile*> bullets;
public:
	Shooter() {
		activeBullets = 1;

		for (int i = 0; i < 1; ++i) {
			Projectile* tmp = nullptr;
			bullets.push_back(tmp);
		}

		for (int i = 0; i < 1; ++i) {
			bullets[i] = new Projectile();
		}
	}

	~Shooter() {
		for (auto b: bullets) {
			if(b) delete b;
		}
	}

	//render

	void RenderProjectiles(RenderWindow* l_window) {
		int n = bullets.size();
		for (int i = n - 1; i >= activeBullets; --i) {
			bullets[i]->Render(l_window);
		}
	}

	//load

	void Load(Direction dir) {
		Projectile* tmp = nullptr;
		bullets.push_back(tmp);
		
		bullets.back() = new Projectile();
		++activeBullets;

		bullets.back()->setDirection(dir);
		std::swap(bullets[activeBullets - 1], bullets.back());
	}

	//shoot

	void Shoot(Direction dir) {

		if (activeBullets == 0) {
			//temporarily, we only give 10 bullets
			return;
		}


		int n = bullets.size();

		bullets[activeBullets-1]->setDirection(dir);
		--activeBullets;
		
		//load
		Load(dir);

		//std::cout << "Number of bullets: " << (int)bullets.size() << '\n';
	}

	void UpdateMovingProjectiles(float delt, Vector2f pos) {
		int n = bullets.size();
		for (int i = n - 1; i >= activeBullets; --i) {
			bullets[i]->ProjectileFly(delt, pos);
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
};