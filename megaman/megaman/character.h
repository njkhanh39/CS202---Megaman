#ifndef CHARACTER_H
#define CHARACTER_H

#include <iostream>
#include "entity.h"
#include <SFML/Graphics.hpp>

using namespace sf;

class WeaponManager;

class Character: public Entity {
private:
	//------Weapon----------//

	WeaponManager* weaponManager;

	const float scaleFactor = 0.25f;
	const Vector2f framesize = { 20.f, 35.f };

	//-----------------------//

	const int maxHealth = 300;
	const int IMMORTAL = 2e9;
	int temphealth = -1;
	
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

	////Shooting
	//void Shoot(float delt) override;

	//void ChargeShoot();
	//-----collision checks-----

	void HandleProjectileCollision(Obstacle* obs);

	void HandleProjectileCollision(Obstacle* obs, Entity* en);

	void HandleProjectileCollision(Entity* en);


	//virtual

	int getHealth() override;

	void addHealth(int h);


	bool IsInBossRegion(Vector2f bossRegion);

	std::vector<short int> GetWeaponsIDs();

	short int GetActiveWeaponIndex();
	//-------UPDATING-------

	//literally everything
	void Update(float delt);

	void UpdateEntity(float delt) override;
	//helper

	//animations
	void LoadAndAddAnimations();

private:
	

	//update
	void UpdateCharacterProjectiles(float delt);


};

#endif
