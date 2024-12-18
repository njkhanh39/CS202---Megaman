#pragma once
#include "item.h"
#include "fstream"
#include "sstream"

class ItemManager {
private:
	TextureManager* textureManager;
	std::vector<Item*> items;
public:
	ItemManager(TextureManager* textureManager, const std::string& file):
		textureManager(textureManager){

		this->LoadFile(file);
	}

	~ItemManager() {
		for (auto& i : items) delete i;
	}

	void Render(RenderTarget* target) {
		for (auto& i : items) i->Render(target);
	}

	void Update(Character* character, float delt) {
		for (auto& i : items) i->Update(character, delt);

		//pop used items
		for (auto it = items.begin(); it != items.end(); ++it) {
			if ((*it)->IsDone()) {
				std::cout << "Item collected!\n";
				delete *it;
				items.erase(it);
				break;
			}
		}
	}

private:
	// type, x, y
	void LoadFile(const std::string& file) {
		std::ifstream fin;
		fin.open(file);

		if (!fin.is_open()) {
			std::cout << "Unable to open " << file << '\n';
			fin.close();
			return;
		}

		std::string line;

		while (std::getline(fin, line)) {
			std::stringstream sstr(line);
			std::vector<int> info;

			std::string field;
			while (std::getline(sstr, field, ',')) {
				info.push_back(std::stoi(field));
			}

			if ((int)info.size() == 3) {
				if (info[0] == 1) {
					items.push_back(nullptr);
					items.back() = new HealthItem(textureManager, info[1], info[2]);
				}
			}
		}

		fin.close();
	}
};