#include "game.h"

Game::Game() : m_window("Chapter 2", Vector2u(1280, 720)){


	this->initStateQueue();
	//----ALL OF OUR TEXTURES ARE INITIALIZED HERE

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

		if (evt.type == evt.KeyPressed && evt.key.code == sf::Keyboard::P) {
			this->textureManager->ShowAllUses();

			auto tmp = this->states.top()->getView();
			std::cout << "Rect:" << tmp->getSize().x << ", "
				<< tmp->getSize().y << '\n';

			std::cout << "Rect pos: " << tmp->getCenter().x - tmp->getSize().x / 2 << ","
				<< tmp->getCenter().y - tmp->getSize().y / 2 << '\n';
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