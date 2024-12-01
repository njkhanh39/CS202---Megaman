#include "shooter.h"

Shooter::Shooter(TextureManager* textureManager) : textureManager(textureManager) {
	activeBullets = 1;
	sampleBullet = new Projectile(textureManager, 0.f, 250.f, 0.f);


	for (int i = 0; i < 1; ++i) {
		Projectile* tmp = nullptr;
		bullets.push_back(tmp);
	}

	for (int i = 0; i < 1; ++i) {
		bullets[i] = new Projectile(*sampleBullet);
	}
}

Shooter::Shooter(TextureManager* textureManager, float bulletsizeX, float bulletsizeY) : textureManager(textureManager) {
	activeBullets = 1;
	sampleBullet = new Projectile(textureManager, bulletsizeX, bulletsizeY, 0.f, 120.f, 0.f);


	for (int i = 0; i < 1; ++i) {
		Projectile* tmp = nullptr;
		bullets.push_back(tmp);
	}

	for (int i = 0; i < 1; ++i) {
		bullets[i] = new Projectile(*sampleBullet);
	}
}

Shooter::Shooter(TextureManager* textureManager, float bulletsizeX, float bulletsizeY, float gravity, float velox, float veloy) :
	textureManager(textureManager) {
	activeBullets = 1;
	sampleBullet = new Projectile(textureManager, bulletsizeX, bulletsizeY, gravity, velox, veloy);


	for (int i = 0; i < 1; ++i) {
		Projectile* tmp = nullptr;
		bullets.push_back(tmp);
	}

	for (int i = 0; i < 1; ++i) {
		bullets[i] = new Projectile(*sampleBullet);
	}
}

Shooter::~Shooter() {
	std::cout << "Destructor of the Shooter (gun).\n";
	for (auto b : bullets) {
		if (b) delete b;
	}

	delete sampleBullet;
}

void Shooter::RenderProjectiles(RenderWindow* l_window) {
	int n = bullets.size();
	for (int i = n - 1; i >= activeBullets; --i) {
		bullets[i]->Render(l_window);
	}
}

void Shooter::Shoot(Direction dir) {



	if (activeBullets == 0) {
		//temporarily, we only give 10 bullets
		return;
	}


	int n = bullets.size();

	bullets[activeBullets - 1]->FaceDirection(dir);
	--activeBullets;

	//load
	Load(dir);

	std::cout << "Number of bullets in vector: " << (int)bullets.size() << '\n';
}

void Shooter::UpdateMovingProjectiles(float delt, Vector2f pos) {
	int n = bullets.size();
	for (int i = n - 1; i >= activeBullets; --i) {
		bullets[i]->ProjectileFly(delt, pos); //this moves the projectile
		bullets[i]->Entity::UpdateEntity(delt); //this plays animation of bullets
	}
}

void Shooter::HandleProjectileCollision(Obstacle* obs, Entity* en) {
	int n = bullets.size();
	for (int i = n - 1; i >= activeBullets; --i) {
		if (bullets[i]->IsHit(obs) || bullets[i]->IsHit(en) || bullets[i]->IsStopped()) {
			if (bullets[i]->IsHit(en) && en) {
				en->TakeDamage(this->damage);
			}

			//pop that bullet out
			std::swap(bullets[i], bullets[n - 1]);

			//this bullet go out of scope, hence deleted
			delete bullets.back();
			bullets.pop_back();
			--n;
		}
	}
}

void Shooter::HandleProjectileCollision(Obstacle* obs) {
	int n = bullets.size();
	for (int i = n - 1; i >= activeBullets; --i) {
		if (bullets[i]->IsHit(obs) || bullets[i]->IsStopped()) {
			//pop that bullet out
			std::swap(bullets[i], bullets[n - 1]);

			//this bullet go out of scope, hence deleted
			delete bullets.back();
			bullets.pop_back();
			--n;
		}
	}
}

void Shooter::HandleProjectileCollision(Entity* en) {
	int n = bullets.size();
	for (int i = n - 1; i >= activeBullets; --i) {
		if (bullets[i]->IsHit(en) || bullets[i]->IsStopped()) {

			if (bullets[i]->IsHit(en) && en) en->TakeDamage(this->damage);

			//pop that bullet out
			std::swap(bullets[i], bullets[n - 1]);

			//this bullet go out of scope, hence deleted
			delete bullets.back();
			bullets.pop_back();
			--n;
		}
	}
}

void Shooter::LoadAnimationForBullet(const std::string& l_file, const std::string& r_file
	, float animationTimer, int start_frame_x, int start_frame_y,
	int frames_x, int frames_y, int _width, int _height) {


	sampleBullet->AddAnimations(l_file, r_file, animationTimer, start_frame_x, start_frame_y,
		frames_x, frames_y, _width, _height);

	//edit the frame

	//sampleBullet->setSize({ float(_width),float(_height) });

	if (!bullets.empty()) {
		bullets.back()->AddAnimations(l_file, r_file, animationTimer, start_frame_x, start_frame_y,
			frames_x, frames_y, _width, _height);
		//bullets.back()->setSize({ float(_width),float(_height) });
	}
}

void Shooter::ScaleProjectileAnimation(float f1, float f2) {
	if (this->sampleBullet) {
		sampleBullet->Entity::setSpriteScale(f1, f2);
	}
	if (!bullets.empty()) {
		bullets.back()->Entity::setSpriteScale(f1, f2);
	}
}


void Shooter::Load(Direction dir) {
	Projectile* tmp = nullptr;
	bullets.push_back(tmp);

	bullets.back() = new Projectile(*sampleBullet);
	++activeBullets;

	bullets.back()->FaceDirection(dir);
	std::swap(bullets[activeBullets - 1], bullets.back());
}



//-----------------X-BUSTER---------------

//-------FULL CHARGE

XBuster::FullChargeBuster::FullChargeBuster(TextureManager* textureManager) : Shooter(textureManager, 37.5, 22.5, 0, 150.f, 0) {
	this->Shooter::LoadAnimationForBullet("Animation\\X\\XBuster_FullChargeLeft.png", "Animation\\X\\XBuster_FullChargeRight.png",
		70.f, 0, 0, 5, 0, 150, 90);

	this->damage = 50;
}

XBuster::FullChargeBuster::~FullChargeBuster() {

}

void XBuster::FullChargeBuster::HandleProjectileCollision(Obstacle* obs, Entity* en) {
	int n = bullets.size();
	for (int i = n - 1; i >= activeBullets; --i) {

		//pierces through entities
		if (bullets[i]->IsHit(en) && en) {
			en->TakeDamage(this->damage);
		}

		if (bullets[i]->IsHit(obs) || bullets[i]->IsStopped()) {

			//pop that bullet out
			std::swap(bullets[i], bullets[n - 1]);

			//this bullet go out of scope, hence deleted
			delete bullets.back();
			bullets.pop_back();
			--n;
		}
	}
}

void XBuster::FullChargeBuster::HandleProjectileCollision(Entity * en) {
	int n = bullets.size();
	for (int i = n - 1; i >= activeBullets; --i) {

		//pierces through entities
		if (bullets[i]->IsHit(en) && en) {
			en->TakeDamage(this->damage);
		}

		if (bullets[i]->IsStopped()) {

			//pop that bullet out
			std::swap(bullets[i], bullets[n - 1]);

			//this bullet go out of scope, hence deleted
			delete bullets.back();
			bullets.pop_back();
			--n;
		}
	}
}

//------SEMI CHARGE

XBuster::SemiChargeBuster::SemiChargeBuster(TextureManager* textureManager) : Shooter(textureManager) {
	if (this->sampleBullet) {
		delete this->sampleBullet;
		this->sampleBullet = new Projectile(textureManager, 0, 500.f, 0);
	}
}

XBuster::SemiChargeBuster::~SemiChargeBuster() {

}

//------XBUSTER

XBuster::XBuster(TextureManager* textureManager) : Shooter(textureManager, 5.f, 5.f, 0.f, 125.f, 0.f) {

	fullcharge = new FullChargeBuster(textureManager);
	semicharge = new SemiChargeBuster(textureManager);
}

XBuster::~XBuster() {
	delete fullcharge;
	delete semicharge;
}

void XBuster::Charge() {
	clk.restart();
}

void XBuster::UnCharge() {
	deltCharge = std::min(maxTime, clk.getElapsedTime().asSeconds());
}

void XBuster::ChargeShoot(Direction dir) {
	if (deltCharge < 0.2f) {
		return; //delay
	}

	if (deltCharge > 0.7f) {
		std::cout << deltCharge << '\n';
		fullcharge->Shoot(dir);
	}
}


void XBuster::UpdateMovingProjectiles(float delt, Vector2f pos)  {
	this->Shooter::UpdateMovingProjectiles(delt, pos);
	fullcharge->Shooter::UpdateMovingProjectiles(delt, pos);
	//semicharge->Shooter::UpdateMovingProjectiles(delt, pos);
}

void XBuster::HandleProjectileCollision(Obstacle* obs, Entity* en)  {
	this->Shooter::HandleProjectileCollision(obs, en);
	fullcharge->HandleProjectileCollision(obs, en);
	//semicharge->Shooter::HandleProjectileCollision(obs, en);
}

void XBuster::HandleProjectileCollision(Obstacle* obs)  {
	this->Shooter::HandleProjectileCollision(obs);
	fullcharge->Shooter::HandleProjectileCollision(obs);
	//semicharge->Shooter::HandleProjectileCollision(obs);
}

void XBuster::HandleProjectileCollision(Entity* en)  {
	this->Shooter::HandleProjectileCollision(en);
	fullcharge->HandleProjectileCollision(en);
	//semicharge->Shooter::HandleProjectileCollision(en);
}

void XBuster::RenderProjectiles(RenderWindow* l_window)  {
	int n = bullets.size();
	for (int i = n - 1; i >= activeBullets; --i) {
		bullets[i]->Render(l_window);
	}
	fullcharge->RenderProjectiles(l_window);
}

void XBuster::ScaleProjectileAnimation(float f1, float f2) {
	if (this->sampleBullet) {
		this->sampleBullet->Entity::setSpriteScale(f1, f2);
		if (!bullets.empty()) bullets.back()->Entity::setSpriteScale(f1, f2);
	}
	this->fullcharge->Shooter::ScaleProjectileAnimation(f1, f2);
	this->semicharge->Shooter::ScaleProjectileAnimation(f1, f2);
}