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

	//---if cntDelay < delay, does not allow shoot!
	float delay = 0.5f;
	float cntDelay = 0.f;
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

	void LoadAnimationForBullet(const std::string& l_file, const std::string& r_file
		, float animationTimer, int start_frame_x, int start_frame_y,
		int frames_x, int frames_y, int _width, int _height);


	//scale animation

	virtual void ScaleProjectileAnimation(float f1, float f2);
private:
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
