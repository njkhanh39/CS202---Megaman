#pragma once
#include "obstacle.h"
#include "enumdirection.h"
#include "entity.h"
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;

class Projectile: public Entity {
private:
public:
    bool isFired = false;
	

	//constructor
	Projectile();


	//----------PHYSICS-----------------

	//Some basic trajectories

	void ProjectileFly(float delt, Vector2f pos);

	//is Hit

	bool IsHit(Obstacle* obs);

	bool IsHit(Entity* en);

	//---------------OOP stuffs------------------

	Projectile(const Projectile& other);

	~Projectile();

	friend void swap(Projectile& obj1, Projectile& obj2) {
		std::swap(obj1.frame, obj2.frame);
		std::swap(obj1.sprite, obj2.sprite);
		//shallow copy, but don't worry!
		std::swap(obj1.texture, obj2.texture);
	}

	Projectile& operator=(Projectile other);
	

	//setters

	void setPosition(Vector2f pos);

	void setDirection(Direction dir);
	
};