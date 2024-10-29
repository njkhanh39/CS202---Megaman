#include "world.h"
World::World() {
	CreatePlatform(0.f,700.f, 1600.f,50.f);

	CreatePlatform(900.f,300.f, 50.f, 200.f, "wall.jpg");
	
	//CreateShooterEnemy(1100.f, 600.f, false, 0.f, 700.f);
	//CreateShooterEnemy(1200.f, 200.f, false, 0.f, 800.f);
}

World::~World() {
	for (int i = 0; i < 30; ++i) {
		if(platform[i]) delete platform[i];
	}

	for (int i = 0; i < 30; ++i) {
		if (enemy[i]) delete enemy[i];
	}
}

void World::Render(RenderWindow* l_window) {
	for (int i = 0; i < 30; ++i) {
		if(platform[i]) platform[i]->Render(l_window);
	}

	for (int i = 0; i < 30; ++i) {
		if (enemy[i]) enemy[i]->Render(l_window);
	}
}