#include "game.h"

Game::Game() : m_window("Chapter 2", Vector2u(1600, 900)){
	//setting up class members

	//in actual project, world may be loaded somewhere else

	m_world = new World();
	m_character = new Character(300.f, 10.f);

} 

Game::~Game() {
	delete m_world;
	delete m_character;
	std::cout << "Game Terminated!\n";
}

//game loop
void Game::Handling() {
	Event evt;
	RenderWindow* getwin = m_window.GetRenderWindow();



	while (getwin->pollEvent(evt)) {
		m_window.HandleInput(evt);
	}	
	m_character->HandleInput(m_elapsed);

	//update character jump,fall

	Update();
	
	HandleCollision();
	
}

void Game::HandleCollision() {

	//handle collisions from character + every entity in world (including projectiles)
	m_world->HandleAllEntitiesCollisions(m_character);

}

void Game::Update() { //game updating

	//-----------------CHARACTER--------------------

	m_character->Update(m_elapsed.asSeconds());

	//-----------------ENEMIES--------------------

	m_world->UpdateAllEnemies(m_character, m_elapsed.asSeconds());
}

void Game::Render() {
	m_window.BeginDraw(); //clear

	//draw sth...
	m_world->Render(m_window.GetRenderWindow());
	m_character->Render(m_window.GetRenderWindow());

	m_window.EndDraw(); //display
}

MainWindow* Game::GetWindow() {
	auto ptr = &m_window;
	return ptr;
}


Time Game::GetElapsed() { 
	return m_elapsed; 
}
void Game::RestartClock() { 
	m_elapsed = m_clock.restart(); 
}