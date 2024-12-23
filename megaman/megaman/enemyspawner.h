#pragma once
#include "attackenemies.h"
#include "boss_1.h"
#include "boss_2.h"
#include "boss_3.h"
#include <fstream>
#include <sstream>

enum EnemyType {
	SHOOTER=1,MOVING=2, BOSS = 3
};

struct EnemyInfo {
	int type;
	int no; //type = Shooter,no = 1 -> shooterenemy1
	int left;
	sf::Vector2f position;
	int right;
	Direction dir;

	EnemyInfo(int _type, int _no, int left, Vector2f _position) : //4 pars
		type(_type), no(_no), position(_position), left(left) {
		right = -1;
		dir = Direction::Left;
	}

	EnemyInfo(int _type, int _no, int left, Vector2f _position, bool yes) : //5 pars
		type(_type), no(_no), position(_position), left(left) {
		right = -1;
		if (yes) dir = Direction::Right;
		else dir = Direction::Left;
	}

	EnemyInfo(int _type, int _no, int left, Vector2f _position, int right, bool yes) : //6 pars
		type(_type), no(_no), position(_position), left(left), right(right) {
		if (yes) dir = Direction::Right;
		else dir = Direction::Left;
	}
};

class EnemySpawner {
private:
	TextureManager* textureManager; //borrow

	std::vector<Enemy*>* enemy; //borrow
	std::vector<EnemyInfo> info;
	std::vector<EnemyInfo> auto_spawn_info;

	int current = 0;

	float timer = 0.f;
	float spawn_rate = 4000.f;
public:
	EnemySpawner(TextureManager* textureManager, std::vector<Enemy*>* enemy, const std::string& file):
	textureManager(textureManager), enemy(enemy) {
		LoadFile(file);
	}

	~EnemySpawner() {

	}

	void LoadFile(const std::string& file) {
		std::ifstream fin;

		fin.open(file);

		if (!fin.is_open()) {
			fin.close();
			std::cout << "Fail to open " << file << '\n';
			return;
		}

		if (!info.empty()) info.clear();

		int n = -1;

		fin >> n;

		std::string line;
		std::getline(fin, line);

		while (std::getline(fin, line)) {
			std::stringstream sstr(line);
			std::vector<int> coords;

			std::string field;
			while (std::getline(sstr, field, ',')) {
				coords.push_back(std::stoi(field));
			}

			if ((int)coords.size() == 5) {
				this->info.push_back(EnemyInfo(coords[0], coords[1], coords[2], Vector2f(coords[3], coords[4])));
			}
			else if ((int)coords.size() == 6) { //direction, no rightlimit
				this->info.push_back(EnemyInfo(coords[0], coords[1], coords[2], Vector2f(coords[3], coords[4]), coords[5]));
			}
			else if ((int)coords.size() == 7) {//both dir and right limit
				this->auto_spawn_info.push_back(EnemyInfo(coords[0], coords[1], coords[2], Vector2f(coords[3], coords[4]), coords[5], coords[6]));
			}
		}

		fin.close();
	}

	void UpdateSpawn(Character* character, float delt) {
		float x = character->getCenterPosition().x;

		timer += 600 * delt;

		if (timer >= spawn_rate) {

			for (int i = 0; i < (int)auto_spawn_info.size(); ++i) {
				if (auto_spawn_info[i].left <= x && x <= auto_spawn_info[i].right) {
					Spawn("RepetitiveSpawn", i);
				}
			}

			timer = 0;
		}

		if (current == (int)info.size()) return;

		if (info[current].left <= x) {
			std::cout << "Spawn enemy!\n";
			Spawn("NormalSpawn", current);
			++current;
		}
	}

	void Spawn(const std::string& type, int index) {

		EnemyInfo* ptr = nullptr;

		if (type == "RepetitiveSpawn") ptr = &auto_spawn_info[index];
		if (type == "NormalSpawn") ptr = &info[index];

		if (!ptr) {
			return;
		}

		if (ptr->type == EnemyType::SHOOTER) {
			if (ptr->no == 1) { //big blue robot
				enemy->push_back(nullptr);
				enemy->back() = new ShooterEnemy1(textureManager, ptr->position.x, ptr->position.y);
			}
			else if (ptr->no == 2) { // wasp with missile launcher
				enemy->push_back(nullptr);
				enemy->back() = new ShooterEnemy2(textureManager, ptr->position.x, ptr->position.y);
			}
			else if (ptr->no == 3) { // handicapped laser robot
				enemy->push_back(nullptr);
				enemy->back() = new ShooterEnemy3(textureManager, ptr->position.x, ptr->position.y, ptr->dir);
			}
			else if (ptr->no == 4) {// miner that throws pickaxes
				enemy->push_back(nullptr);
				enemy->back() = new ShooterEnemy4(textureManager, ptr->position.x, ptr->position.y, ptr->dir);
			}
			else if (ptr->no == 5) {//fixed flamethrower
				enemy->push_back(nullptr);
				enemy->back() = new ShooterEnemy5(textureManager, ptr->position.x, ptr->position.y, ptr->dir);
			}
			else if (ptr->no == 6) {//snow cannon
				enemy->push_back(nullptr);
				enemy->back() = new ShooterEnemy6(textureManager, ptr->position.x, ptr->position.y, ptr->dir);
			}
			else if (ptr->no == 7) {//ostrich
				enemy->push_back(nullptr);
				enemy->back() = new ShooterEnemy7(textureManager, ptr->position.x, ptr->position.y, ptr->dir);
			}
			else if (ptr->no == 8) {//snow catapult
				enemy->push_back(nullptr);
				enemy->back() = new ShooterEnemy8(textureManager, ptr->position.x, ptr->position.y, ptr->dir);
			}
		}
		else if (ptr->type == EnemyType::MOVING) {
			if (ptr->no == 1) { //spike wheel
				enemy->push_back(nullptr);
				enemy->back() = new AttackEnemy1(textureManager, ptr->position.x, ptr->position.y, ptr->dir);
			}
		}
		else if (ptr->type == EnemyType::BOSS) {
			if (ptr->no == 1) {
				enemy->push_back(nullptr);
				enemy->back() = new Vile(textureManager, ptr->position.x, ptr->position.y, Vector2f(6730, 7080));
			}
			else if (ptr->no == 2) {
				enemy->push_back(nullptr);
				enemy->back() = new FlameMammoth(textureManager, ptr->position.x, ptr->position.y, Vector2f(7754, 8111));
			}
			else if (ptr->no == 3) {
				enemy->push_back(nullptr);
				enemy->back() = new ChillPenguin(textureManager, ptr->position.x, ptr->position.y, Vector2f(5927, 6237));
			}
		}
	}
};