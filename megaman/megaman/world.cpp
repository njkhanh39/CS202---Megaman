#include "world.h"
World::World(TextureManager* textureManager): textureManager(textureManager) {
	background = new RectangleShape({ 1280.f,720.f });
	background->setFillColor(Color::White);


	gameMap = new Map(textureManager, "Animation\\highway.png");

	CreatePlatform(-500.f,125.f, 2280.f,10.f);

	//CreatePlatform(400.f,300.f, 50.f, 200.f, "wall.jpg");
	
	//CreateShooterEnemy(800.f, 200.f, false, 0.f, 700.f);
	//CreateShooterEnemy(900.f, 200.f, false, 0.f, 800.f);
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

	for (int i = 0; i < 30; ++i) {
		if(platform[i]) platform[i]->Render(l_window);
	}

	for (int i = 0; i < 30; ++i) {
		if (enemy[i] && !enemy[i]->IsDead()) enemy[i]->Render(l_window);
	}
}