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
	std::queue<STATECOMMAND>* statequeue;

	void initStateQueue() {
		statequeue = new std::queue<STATECOMMAND>;
	}

	void initTextureManager() {
		textureManager = new TextureManager();
	}

	void initMainMenuState() {
		this->states.push(new MainMenu(&this->m_window, textureManager, statequeue));
	}

	void ExecuteStateQueue() {
		while (!this->statequeue->empty()) {
			STATECOMMAND cmd = statequeue->front();
			this->statequeue->pop();

			if (cmd == QUIT) {
				delete states.top();
				states.pop();
			}

			if (cmd == PUSH_GAMEOVER_1) {
				this->states.push(new GameoverState(&m_window, textureManager, statequeue, 1));
			}
			if (cmd == PUSH_GAMEOVER_2) {
				this->states.push(new GameoverState(&m_window, textureManager, statequeue, 2));
			}
			if (cmd == PUSH_GAMEOVER_3) {
				this->states.push(new GameoverState(&m_window, textureManager, statequeue, 3));
			}
			if (cmd == PUSH_GAMESTATE_1) {
				this->states.push(new GameState(&m_window, textureManager, statequeue, 1));
			}
			if (cmd == PUSH_GAMESTATE_2) {
				this->states.push(new GameState(&m_window, textureManager, statequeue, 2));
			}
			if (cmd == PUSH_GAMESTATE_3) {
				this->states.push(new GameState(&m_window, textureManager, statequeue, 3));
			}
			if (cmd == PUSH_MAINMENU) {
				this->states.push(new MainMenu(&m_window, textureManager, statequeue));
			}
			if (cmd == PUSH_STAGESELECTION) {
				this->states.push(new StageSelection(&m_window, textureManager, statequeue));
			}
			if (cmd == PUSH_WIN_STATE) {
				this->states.push(new WinState(&m_window, textureManager, statequeue));
			}
		}
	}
};