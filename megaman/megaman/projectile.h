#pragma once
#include "obstacle.h"
#include "enumdirection.h"
#include "entity.h"
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;

class Projectile: public Entity {
private:
	Clock clk;
	float lifespan = 2.0f;
public:
    bool isFired = false;
	
	//constructor
	Projectile(TextureManager* textureManager);

	Projectile(TextureManager* textureManager, float _gravity,float _veloX, float _veloY);

	Projectile(TextureManager* textureManager, float width, float height, float _gravity, float _veloX, float _veloY);

	//NOTE: UPDATE COPY CONSTRUCTOR WHENEVER YOU ADD NEW ATTRIBUTES TO CLASS
	Projectile(const Projectile& other): Entity(other), isFired(other.isFired) {
		
	}

	void Render(RenderWindow* l_window) override {
		//l_window->draw(frame);
		l_window->draw(sprite);
	}

	~Projectile() override;


	//----------PHYSICS-----------------

	//Some basic trajectories

	//In the update functions of shooters, we invoke this function of the bullet for flying physics + Entity::UpdateMovement() for animation.
	void ProjectileFly(float delt, Vector2f pos);

	//is Hit

	bool IsHit(Obstacle* obs);

	bool IsHit(Entity* en);

	bool IsStopped();

	float getCurrentTime() {
		return clk.getElapsedTime().asSeconds();
	}

	//---------ANIMATION----------------


	//depending on each bullet that we run this function or not 
	void AddAnimations(const std::string& l_file, const std::string& r_file
	, float animationTimer, int start_frame_x, int start_frame_y,
		int frames_x, int frames_y, int _width, int _height) {


		//Moving right
		movingAnimation->AddAnimation("MovingRight", r_file, animationTimer, start_frame_x, start_frame_y, frames_x, frames_y,
			_width, _height);

		//Moving left
		movingAnimation->AddAnimation("MovingLeft", l_file, animationTimer, start_frame_x, start_frame_y, frames_x, frames_y,
			_width, _height);
	}
};