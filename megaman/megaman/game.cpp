#include "game.h"

Game::Game() : m_window("Chapter 2", Vector2u(1600, 900)){
	this->initTextureManager();

	//------------STATE------------
	
	//this->initGameState();

	
	this->initMainMenuState();
	 
	

	//in actual project, world may be loaded somewhere else -> yeah, in gamestate silly

} 

Game::~Game() {

	//states
	while (!states.empty()) {
		delete states.top();
		states.pop();
	}

	//game
	std::cout << "Game Terminated!\n";
}

//game loop
void Game::Handling() {
	Event evt;
	RenderWindow* getwin = m_window.GetRenderWindow();



	while (getwin->pollEvent(evt)) {
		m_window.HandleEvents(evt);
		
		//Only handle state top
		if(!states.empty()) this->states.top()->HandlingEvent(evt, m_elapsed);

		if (evt.type == evt.KeyPressed && evt.key.code == sf::Keyboard::P) {
			this->textureManager->ShowAllUses();
		}
	}	

	//Only handle state top
	if (!states.empty()) {
		this->states.top()->HandleInput(m_elapsed);

		
		//---WE MUST CALL UPDATE TWICE TO MAKE THINGS FASTER---//
		this->states.top()->Update(m_elapsed.asSeconds());

		//--Only for game state---

		if (dynamic_cast<GameState*>(states.top())) {
			this->states.top()->HandleCollision();
		}
	}
	
}



void Game::Update() { //game updating

	//-----------------STATES-----------------------

	if (!this->states.empty()) {
		this->states.top()->Update(m_elapsed.asSeconds());

		//when running update function, at some time, getQuit() may be set to true
		if (this->states.top()->getQuit()) {
			//end state and deallocate
			this->states.top()->EndState();

			//deallocate

			if(this->states.top()) delete this->states.top();

			this->states.pop();
		}
	}
	//application ends, since no states :0
	else {
		this->m_window.SetFinish();
	}
	
	//this->states.top()->Update(m_elapsed.asSeconds());
}

void Game::Render() {
	m_window.BeginDraw(); //clear


	//-----------------STATES-----------------------

	if (!this->states.empty()) {
		this->states.top()->Render(m_window.GetRenderWindow());
	}

	//----------------------------------------------

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