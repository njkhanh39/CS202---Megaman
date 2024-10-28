#pragma once
#include <iostream>
#include "entity.h"
#include <SFML/Graphics.hpp>
#include "animation.h"
#include "shooter.h"

using namespace sf;

class Character: public Entity {
private:

	//------Animation-------//


	//sheets

	Texture* texture_idle_left, * texture_idle_right, *texture_jump_left, *texture_jump_right;

	Texture* texture_shoot_left, * texture_shoot_right, * texture_shoot_jump_left, * texture_shoot_jump_right;

	Texture* texture_movement_right,* texture_movement_left, *texture_movement_shoot_right
		, *texture_movement_shoot_left;



	AnimationComponent* movingAnimation;


	//------Weapon----------//

	Shooter* blaster;


public:


	//constructor and destructor

	Character(float x, float y);

	~Character();

	//virtual drawing
	void Render(RenderWindow* l_window);


	//animations - is called in constructor

	void CreateAnimationComponent();


	//--------actions-------

	void HandleInput(Time& elapsed);

	//virtual
	void Shoot();

	//-----collision checks-----

	void HandleProjectileCollision(Obstacle* obs);


	//virtual
	bool canKeepFalling(Obstacle* obs);


	//-------UPDATING-------

	void UpdateCharacterProjectiles(float delt);

	void UpdateCharacter(float delt);


	//helper

	void fixSpriteToFrame();
};