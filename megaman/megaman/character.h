#pragma once
#include <iostream>
#include "entity.h"
#include <SFML/Graphics.hpp>
#include "shooter.h"

using namespace sf;

class Character: public Entity {
private:
	//------Weapon----------//

	XBuster* blaster;

public:
	//constructor and destructor

	Character(TextureManager* textureManager, float x, float y);

	~Character() override;

	//Rendering character
	void Render(RenderWindow* l_window) override;

	//animations

	//Handle EVENT INPUT
	void HandleEventInput(Event& evt, Time& elapsed);

	//Handle instant keyboard inputs
	void HandleMovements(Time& elapsed);

	//Shooting
	void Shoot() override;

	void ChargeShoot();
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
		//update char's projectiles
		UpdateCharacterProjectiles(delt);
	}

	//helper

	void setPosition(Vector2f vec) override;

	//animations
	void LoadAndAddAnimations();

private:
	

	//update
	void UpdateCharacterProjectiles(float delt);

	//helper
	void fixSpriteToFrame();

};