#pragma once
#include "world.h"
#include "character.h"
#include "States/MainMenu.h"
class Game {
public:
	Game();
	~Game();

	//these 4 functions are called during loop, so they must be crucial!

	//game loop

	//------CALL IN ORDER!--------
	void Handling(); //get inputs, turn on booleans,...

	void Update(); //update animations, movements accordingly to inputs and booleans

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
	TextureManager* textureManager;
	
	//------STATES-------//
	std::stack<State*> states;

	void initTextureManager() {
		textureManager = new TextureManager();
	}

	void initGameState() {
		this->states.push(new GameState(&this->m_window,textureManager, &states));
	}

	void initMainMenuState() {
		this->states.push(new MainMenu(&this->m_window, textureManager, &states));
	}
};