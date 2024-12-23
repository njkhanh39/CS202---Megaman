#include "WeaponManager.h"
#include "character.h"

void  WeaponManager::HandleEventInput(Event& evt, Time& elapsed, Character* character) {//swap guns
	if (weapons.empty()) return;


	//weapon swapping

	if (evt.type == Event::KeyPressed && evt.key.code == Keyboard::Q) {
		--cur;
		if (cur < 0) cur = (int)weapons.size() - 1;
	}
	else if (evt.type == Event::KeyPressed && evt.key.code == Keyboard::E) {
		++cur;
		if (cur >= (int)weapons.size()) cur = 0;
	}

	if (dynamic_cast<XBuster*>(weapons[cur])) {
		if (evt.type == Event::KeyPressed && evt.key.code == Keyboard::Enter) {
			this->Shoot(elapsed.asSeconds(), character);
		}
		else if (evt.type == Event::KeyReleased && evt.key.code == Keyboard::Enter) {
			std::cout << "Release!\n";

			this->ChargeShoot(elapsed.asSeconds(), character);
		}
		else character->isChargeShooting = false;
	}
	else {//other guns
		if (evt.type == Event::KeyPressed && evt.key.code == Keyboard::Enter) {
			this->Shoot(elapsed.asSeconds(), character);
		}
		else if (evt.type == Event::KeyReleased && evt.key.code == Keyboard::Enter) {
			character->isShooting = false;
		}
	}
}

void WeaponManager::RenderProjectiles(RenderWindow* l_window) {
	if (weapons.empty()) return;

	for(auto& x: weapons) x->RenderProjectiles(l_window);
}

void WeaponManager::HandleProjectileCollision(Obstacle* obs) {
	if (weapons.empty()) return;
	for (auto& x : weapons) x->HandleProjectileCollision(obs);
}

void WeaponManager::HandleProjectileCollision(Obstacle* obs, Entity* en) {
	if (weapons.empty()) return;
	for (auto& x : weapons) x->HandleProjectileCollision(obs, en);
}

void WeaponManager::HandleProjectileCollision(Entity* en) {
	if (weapons.empty()) return;
	for (auto& x : weapons) x->HandleProjectileCollision(en);
}

void WeaponManager::UpdateMovingProjectiles(float delt, Character* character) {
	if (weapons.empty()) return;
	for (auto& x : weapons) x->UpdateMovingProjectiles(delt, character->getCenterPosition() + Vector2f(0.f, -10.f));
}

std::vector<short int> WeaponManager::GetWeaponsIDs() {
	std::vector<short int> tmp;

	for (auto& x : weapons) {
		if (dynamic_cast<XBuster*>(x)) {
			tmp.push_back(1);
		}
		else if (dynamic_cast<ElectricCannon*>(x)) {
			tmp.push_back(2);
		}
		else if (dynamic_cast<FlameThrower*>(x)) {
			tmp.push_back(3);
		}
		else if (dynamic_cast<IceShotgun*>(x)) {
			tmp.push_back(4);
		}
	}

	return tmp;
}

short int WeaponManager::GetActiveWeaponIndex() {
	return cur;
}

//privates

void WeaponManager::Shoot(float delt, Character* character) {
	if (weapons.empty()) return;

	if (dynamic_cast<XBuster*>(weapons[cur])) {
		XBuster_Shoot(delt, character, weapons[cur]);
	}
	else {
		if (!character->isShooting) {
			weapons[cur]->Shoot(character->direction);
			character->isShooting = true;
		}
		//else character->isShooting = false; //we'll set this in handle event input
	}
}

void WeaponManager::ChargeShoot(float delt, Character* character) {
	if (weapons.empty()) return;

	if (dynamic_cast<XBuster*>(weapons[cur])) {
		XBuster_ChargeShoot(delt, character, weapons[cur]);
	}
}

void WeaponManager::LoadWeapon(const std::string& file) {
	std::ifstream fin;

	fin.open(file);

	if (fin.is_open()) {
		std::string line;
		while (std::getline(fin, line)) {
			std::stringstream sstr(line);
			std::vector<int> coords;

			std::string field;
			while (std::getline(sstr, field, ',')) {
				coords.push_back(std::stoi(field));
			}

			if ((int)coords.size() == 1) {
				if (coords[0] == 1) {//Xbuster
					weapons.push_back(nullptr);
					weapons.back() = new XBuster(textureManager);
					weapons.back()->ScaleProjectileAnimation(0.25, 0.85);
				}
				else if (coords[0] == 2) {//electric cannon
					weapons.push_back(nullptr);
					weapons.back() = new ElectricCannon(textureManager);
				}
				else if (coords[0] == 3) {//flame thrower
					weapons.push_back(nullptr);
					weapons.back() = new FlameThrower(textureManager);
				}
				else if (coords[0] == 4) {//ice shotgun
					weapons.push_back(nullptr);
					weapons.back() = new IceShotgun(textureManager);
				}
			}
		}
	}
	fin.close();
}

void WeaponManager::XBuster_Shoot(float delt, Character* character, Shooter* blaster) {
	if (!character->isShooting) {
		character->isChargeShooting = false;
		blaster->Shoot(character->direction);
		blaster->Charge();
		character->isShooting = true;
	}
}

void WeaponManager::XBuster_ChargeShoot(float delt, Character* character, Shooter* blaster) {
	character->isShooting = false;
	blaster->UnCharge();
	blaster->ChargeShoot(character->direction);
	character->isChargeShooting = true;
}