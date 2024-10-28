#pragma once
#include <iostream>
#include "entity.h"
#include <SFML/Graphics.hpp>
#include "shooter.h"

using namespace sf;

class Character: public Entity {
private:

	//------Animation-------//


	//sheets



	//------Weapon----------//

	Shooter* blaster;


public:


	//constructor and destructor

	Character(float x, float y);

	~Character();

	//virtual drawing
	void Render(RenderWindow* l_window);


	//animations - is called in constructor


	//--------actions-------

	void HandleInput(Time& elapsed);

	//virtual
	void Shoot();

	//-----collision checks-----

	void HandleProjectileCollision(Obstacle* obs);

	void HandleProjectileCollision(Obstacle* obs, Entity* en);

	void HandleProjectileCollision(Entity* en);


	//virtual
	bool canKeepFalling(Obstacle* obs);


	//-------UPDATING-------

	void UpdateCharacterProjectiles(float delt);


		




	//helper

	void fixSpriteToFrame();

	void setPosition(Vector2f vec) {
		frame.setPosition(vec);
		auto x = vec.x;
		auto y = vec.y;
		sprite.setPosition({ x - 20.f,y });
	}
};