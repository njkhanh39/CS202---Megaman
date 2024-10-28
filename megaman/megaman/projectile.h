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

	~Projectile();


	//----------PHYSICS-----------------

	//Some basic trajectories

	void ProjectileFly(float delt, Vector2f pos);

	//is Hit

	bool IsHit(Obstacle* obs);

	bool IsHit(Entity* en);

	

	//setters

	void setDirection(Direction dir);
	
};