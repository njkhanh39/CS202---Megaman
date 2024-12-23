#ifndef WEAPONMANAGER_H
#define WEAPONMANAGER_H

#include "shooter.h"
#include <fstream>
#include <sstream>

class Character;

class WeaponManager { //for character
private:
	TextureManager* textureManager;
	std::vector<Shooter*> weapons;
	short int cur = 0;
public:
	WeaponManager(TextureManager* textureManager) : textureManager(textureManager) {
		this->LoadWeapon("Animation\\X\\weapons.txt");
	}

	~WeaponManager() {
		for (auto& x : weapons) delete x;
	}

	void HandleEventInput(Event& evt, Time& elapsed, Character* character);

	void RenderProjectiles(RenderWindow* l_window);

	
	void HandleProjectileCollision(Obstacle* obs);

	void HandleProjectileCollision(Obstacle* obs, Entity* en);

	void HandleProjectileCollision(Entity* en);

	void UpdateMovingProjectiles(float delt, Character* character);

	std::vector<short int> GetWeaponsIDs();

	short int GetActiveWeaponIndex();
private:
	void Shoot(float delt, Character* character);

	void ChargeShoot(float delt, Character* character);


	void LoadWeapon(const std::string& file);
	//-------XBUSTER------------//
	void XBuster_Shoot(float delt, Character* character, Shooter* blaster);

	void XBuster_ChargeShoot(float delt, Character* character, Shooter* blaster);
};

#endif 