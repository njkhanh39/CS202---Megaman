#include "world.h"
World::World(TextureManager* textureManager): textureManager(textureManager) {
	background = new RectangleShape({ 1280.f,720.f });
	background->setFillColor(Color::White);

	this->CreateWorld("Animation\\Map1\\", "highway.png",  "obs_1.txt", "obs_2.txt");

	//CreatePlatform(400.f,300.f, 50.f, 200.f, "wall.jpg");
	
	CreateShooterEnemy(300.f, 10.f, 30.f, 60.f, 5.f, 5.f, false, 0.f, 200.f);

}

World::~World() {

	delete background;
	delete gameMap;

	for (int i = 0; i < 30; ++i) {
		if(platform[i]) delete platform[i];
	}

	for (int i = 0; i < 30; ++i) {
		if (enemy[i]) delete enemy[i];
	}
}

void World::Render(RenderWindow* l_window) {
	l_window->draw(*background);

	gameMap->Render(l_window);

	for (int i = 0; i < numObs; ++i) {
		if(platform[i]) platform[i]->Render(l_window);
	}

	for (int i = 0; i < numEnemy; ++i) {
		if (enemy[i] && !enemy[i]->IsDead()) enemy[i]->Render(l_window);
	}
}