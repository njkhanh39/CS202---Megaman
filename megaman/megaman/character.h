#pragma once
#include <iostream>
#include "entity.h"
#include <SFML/Graphics.hpp>
#include "shooter.h"

using namespace sf;

class Character: public Entity {
private:
	//------Weapon----------//

	Shooter* blaster;


public:
	//constructor and destructor

	Character(float x, float y);

	~Character() override;

	//Rendering character
	void Render(RenderWindow* l_window) override;

	//animations

	//Handle keyboard inputs
	void HandleInput(Time& elapsed);

	//Shooting
	void Shoot() override;

	//-----collision checks-----

	void HandleProjectileCollision(Obstacle* obs);

	void HandleProjectileCollision(Obstacle* obs, Entity* en);

	void HandleProjectileCollision(Entity* en);


	//virtual
	bool canKeepFalling(Obstacle* obs) override;


	//-------UPDATING-------

	//literally everything
	void Update(float delt) {
		UpdateMovements(delt);
		UpdateCharacterProjectiles(delt);
	}

	//helper

	void setPosition(Vector2f vec) override;

private:
	//animations
	void LoadAndAddAnimations();

	//update
	void UpdateCharacterProjectiles(float delt);

	//helper
	void fixSpriteToFrame();

};