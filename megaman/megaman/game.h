#pragma once
#include "window.h"
#include "world.h"
#include "character.h"

class Game {
public:
	Game();
	~Game();

	//these 4 functions are called during loop, so they must be crucial!

	//game loop

	//------CALL IN ORDER!--------

	void HandlingEvent(Event& evt, Time& delt) {
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


	World* m_world;
	Character* m_character;
};