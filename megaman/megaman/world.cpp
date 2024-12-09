#include "world.h"
World::World(TextureManager* textureManager): textureManager(textureManager) {
	background = new RectangleShape({ 1280.f,720.f });
	background->setFillColor(Color::White);

	this->CreateWorld("Animation\\Map1\\", "highway.png",  "obs_1.txt", "obs_2.txt");

	//CreatePlatform(400.f,300.f, 50.f, 200.f, "wall.jpg");
	this->enemySpawner = new EnemySpawner(textureManager, &enemy, "Animation\\Map1\\enemyinfo.txt");
	

	/*CreateShooterEnemy1(300.f, 10.f);

	CreateAttackEnemy1(700.f, 10.f, Direction::Left);


	CreateShooterEnemy2(1200.f, 5.f, Direction::Left);
	CreateShooterEnemy2(1600.f, 5.f, Direction::Left);
	CreateShooterEnemy2(2000.f, 5.f, Direction::Left);
	CreateShooterEnemy2(2400.f, 5.f, Direction::Left);
	CreateShooterEnemy2(2800.f, 5.f, Direction::Left);
	CreateShooterEnemy2(3200.f, 5.f, Direction::Left);
	CreateShooterEnemy2(4000.f, 5.f, Direction::Left);
	CreateShooterEnemy2(5000.f, 5.f, Direction::Left);
	CreateShooterEnemy2(5400.f, 5.f, Direction::Left);
	CreateShooterEnemy2(6000.f, 5.f, Direction::Left);*/

}

World::~World() {

	delete background;
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
	l_window->draw(*background);

	gameMap->Render(l_window);

	for (int i = 0; i < (int)platform.size(); ++i) {
		if(platform[i]) platform[i]->Render(l_window);
	}

	for (int i = 0; i < (int)enemy.size(); ++i) {
		if (enemy[i] && !enemy[i]->IsDead()) enemy[i]->Render(l_window);
	}
}