#include "game.h"

Game::Game() : m_window("Megaman X Spinoff", Vector2u(1280, 720)){


	this->initStateQueue();


	this->initTextureManager();

	//------------STATE------------

	
	this->initMainMenuState();
	 

} 

Game::~Game() {

	//states
	while (!states.empty()) {
		delete states.top();
		states.pop();
	}

	delete statequeue;

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
		//------------UPDATE STATE
		this->states.top()->Update(m_elapsed.asSeconds());

		//------------UPDATE VIEW

		this->m_window.SetView(*this->states.top()->getView());

		//------------EXECUTE STATE PUSHING POPPING--------//

		this->states.top()->ResetQueueLock();
		this->ExecuteStateQueue();
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