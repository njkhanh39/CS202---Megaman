#pragma once
#include "attackenemies.h"
#include <fstream>
#include <sstream>

enum EnemyType {
	SHOOTER=1,MOVING=2
};

struct EnemyInfo {
	int type;
	int no; //type = Shooter,no = 1 -> shooterenemy1
	int left;
	sf::Vector2f position;

	EnemyInfo(int _type, int _no,int left, Vector2f _position):
	type(_type), no(_no), position(_position), left(left){

	}
};

class EnemySpawner {
private:
	TextureManager* textureManager; //borrow

	std::vector<Enemy*>* enemy; //borrow
	std::vector<EnemyInfo> info;

	int current = 0;
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
		}

		fin.close();
	}

	void UpdateSpawn(Character* character) {
		if (current == (int)info.size()) return;

		float x = character->getCenterPosition().x;

		if (info[current].left <= x) {
			std::cout << "Spawn enemy!\n";
			Spawn();
			++current;
		}
	}

	void Spawn() {
		if (info[current].type == EnemyType::SHOOTER) {
			if (info[current].no == 1) {
				enemy->push_back(nullptr);
				enemy->back() = new ShooterEnemy1(textureManager, info[current].position.x, info[current].position.y);
			}
			else if (info[current].no == 2) {
				enemy->push_back(nullptr);
				enemy->back() = new ShooterEnemy2(textureManager, info[current].position.x, info[current].position.y);
			}
		}
		else if (info[current].type == EnemyType::MOVING) {
			if (info[current].no == 1) {
				enemy->push_back(nullptr);
				enemy->back() = new AttackEnemy1(textureManager, info[current].position.x, info[current].position.y);
			}
		}
	}
};