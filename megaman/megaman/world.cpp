#include "world.h"
World::World(TextureManager* textureManager): textureManager(textureManager) {
	//background = new RectangleShape({ 1280.f,720.f });
	//background->setFillColor(Color::White);

	this->CreateWorld("Animation\\Map1\\", "highway.png",  "obs_1.txt", "obs_2.txt");

	//CreatePlatform(400.f,300.f, 50.f, 200.f, "wall.jpg");
	this->enemySpawner = new EnemySpawner(textureManager, &enemy, "Animation\\Map1\\enemyinfo.txt");
}

World::World(TextureManager* textureManager, int boss_left, int boss_right, const std::string& dir, const std::string& mapname) :
	textureManager(textureManager), boss_region_left(boss_left), boss_region_right(boss_right) {

	this->CreateWorld(dir, mapname, "obs_1.txt", "obs_2.txt");

	//CreatePlatform(400.f,300.f, 50.f, 200.f, "wall.jpg");
	this->enemySpawner = new EnemySpawner(textureManager, &enemy, dir + "enemyinfo.txt");
}

World::~World() {

	//delete background;
	delete gameMap;
	delete enemySpawner;

	for (int i = 0; i < (int)platform.size(); ++i) {
		if(platform[i]) delete platform[i];
	}

	for (int i = 0; i < (int)enemy.size(); ++i) {
		if (enemy[i]) delete enemy[i];
	}


}

void World::Render(RenderWindow* l_window) {
//	l_window->draw(*background);

	gameMap->Render(l_window);

	for (int i = 0; i < (int)platform.size(); ++i) {
		if(platform[i]) platform[i]->Render(l_window);
	}

	for (int i = 0; i < (int)enemy.size(); ++i) {
		if (enemy[i]) enemy[i]->Render(l_window);
	}
}