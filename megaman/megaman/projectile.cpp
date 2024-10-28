#include "projectile.h"

//----------PHYSICS-----------------

	//Some basic trajectories

void Projectile::ProjectileFly(float delt, Vector2f pos) {
	if (!isFired) {
		isFired = true;

		//innit a position right in front of gun barrel
		setPosition(pos);
	}

	velocityY += gravity * delt;


	//draw
	if (direction == Direction::Right) {
		sprite.move(velocityX * delt, velocityY * delt);
		frame.move(velocityX * delt, velocityY * delt);
	}
	else {
		sprite.move(-velocityX * delt, velocityY * delt);
		frame.move(-velocityX * delt, velocityY * delt);
	}
}

//is Hit

bool Projectile::IsHit(Obstacle* obs) {


	float l1 = obs->getLeftMostX(), l2 = getLeftMostX();
	float r1 = obs->getRightMostX(), r2 = getRightMostX();
	float u1 = obs->getUpMostY(), u2 = getUpMostY();
	float d1 = obs->getDownMostY(), d2 = getDownMostY();


	//----temporary---

	if (r2 >= 1600.f) return true;
	if (l2 <= 0.f) return true;

	//----------------

	if (!(d2 <= u1 || u2 >= d1)) {
		//left->right
		if (r2 == l1 || (r2 > l1 && r2 - l1 < 1.f)) {
			return true;
		}

		//left <- right
		if (l2 == r1 || (l2 < r1 && r1 - l2 < 1.f)) {
			return true;
		}
	}

	//bullet goes straight, doesnt hit right or left -> no hit
	if (velocityY == 0.f) return false;

	if (!(r2 <= l1 || l2 >= r1)) {
		//high to low

		if (d2 == u1 || (d2 > u1 && d2 - u1 < 1.f)) {
			return true;
		}

		//low to high

		if (u2 == d1 || (u2 < d1 && d1 - u2 < 1.f)) {
			return true;
		}
	}

	return false;
}

bool Projectile::IsHit(Entity* en) {

	float l1 = en->getLeftMostX(), l2 = getLeftMostX();
	float r1 = en->getRightMostX(), r2 = getRightMostX();
	float u1 = en->getUpMostY(), u2 = getUpMostY();
	float d1 = en->getDownMostY(), d2 = getDownMostY();


	//----temporary---

	if (r2 >= 1600.f) return true;
	if (l2 <= 0.f) return true;

	//----------------

	if (!(d2 <= u1 || u2 >= d1)) {
		//left->right
		if (r2 == l1 || (r2 > l1 && r2 - l1 < 1.f)) {
			return true;
		}

		//left <- right
		if (l2 == r1 || (l2 < r1 && r1 - l2 < 1.f)) {
			return true;
		}
	}

	//bullet goes straight, doesnt hit right or left -> no hit
	if (velocityY == 0.f) return false;

	if (!(r2 <= l1 || l2 >= r1)) {
		//high to low

		if (d2 == u1 || (d2 > u1 && d2 - u1 < 1.f)) {
			return true;
		}

		//low to high

		if (u2 == d1 || (u2 < d1 && d1 - u2 < 1.f)) {
			return true;
		}
	}

	return false;
}

//----------DRAWING RENDERING UPDATING-----------------


Projectile::Projectile(){
	gravity = 0;

	velocityY = 0.f;
	velocityX = 800.f;

	direction = Direction::Right;

	frame.setFillColor(Color::Red);
	frame.setSize({ 20.f,20.f });
}

//OOP stuffs
//
//Projectile::Projectile(const Projectile& other) {
//	gravity = other.gravity;
//
//	velocityX = other.velocityX;
//	velocityY = other.velocityY;
//
//	isFired = other.isFired;
//
//	direction = other.direction;
//
//	frame = other.frame;
//	sprite = other.sprite;
//
//	this->texture = new Texture();
//
//	*texture = *other.texture;
//}

Projectile::~Projectile() {
	std::cout << "Destructor of Projectile.\n";
}



//Projectile& 
//Projectile::operator=(Projectile other) {
//	swap(*this, other);
//	return *this;
//}
