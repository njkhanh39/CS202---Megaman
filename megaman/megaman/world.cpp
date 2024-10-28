#include "world.h"
World::World() {
	platform[0] = new Obstacle({0.f,700.f}, {1600.f,50.f});

	wall = new Obstacle({ 900.f,300.f }, { 50.f, 200.f });
	wall->setColor(Color(118, 57, 49));
	wall->loadImage("wall.jpg");

	enemy = new ShooterEnemy(1100.f, 600.f, false, 0.f, 800.f);
}

World::~World() {
	for (int i = 0; i < 1; ++i) {
		if(platform[i]) delete platform[i];
	}

	delete wall;
	delete enemy;
}

void World::Render(RenderWindow* l_window) {
	for (int i = 0; i < 1; ++i) {
		if(platform[i]) platform[i]->Render(l_window);
	}

	wall->Render(l_window);
	enemy->Render(l_window);
}