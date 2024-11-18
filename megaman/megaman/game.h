#pragma once
#include "world.h"
#include "character.h"
#include "States/GameState.h"
class Game {
public:
	Game();
	~Game();

	//these 4 functions are called during loop, so they must be crucial!

	//game loop

	//------CALL IN ORDER!--------

	void HandlingEvent(Event& evt, Time& delt) {
		//char shooting
		m_character->HandleEventInput(evt, delt);
	}

	void Handling(); 

	void Update();

	void HandleCollision();

	void Render();
	void RestartClock();
	//------------------

	MainWindow* GetWindow();

	Time GetElapsed();
	
	//...

	
private:
	MainWindow m_window;
	Clock m_clock;
	Time m_elapsed;

	
	//------STATES-------//
	std::stack<State*> states;


	void initState() {
		this->states.push(new GameState(&this->m_window));
	}
	

	//----game stuffs-----

	World* m_world;
	Character* m_character;
};