#ifndef STATE_H
#define STATE_H
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <SFML/Graphics.hpp>
#include "window.h"
#include "Auxiliaries/Button.h"
#include "Manager//SoundManager.h"

enum STATECOMMAND {
	QUIT, PUSH_MAINMENU, PUSH_GAMEOVER_1, PUSH_GAMEOVER_2, PUSH_GAMEOVER_3,
	PUSH_GAMESTATE_1, PUSH_GAMESTATE_2, PUSH_GAMESTATE_3, PUSH_STAGESELECTION, PUSH_WIN_STATE

};

class State {
protected:
	MainWindow* window; 
	bool quit;
	bool paused;

	//mouse
	sf::Vector2f mousePos;

	std::queue<STATECOMMAND>* statequeue;
	bool lockQueueCommand = false; //since we update twice, commands can be queued twice, which is bad

	//--Texture manager
	TextureManager* textureManager;

	//View
	View* stateview;
public:
	State(MainWindow* window,TextureManager* textureManager, std::queue<STATECOMMAND>* statequeue): 
		textureManager(textureManager), paused(false), quit(false), mousePos({0,0}), statequeue(statequeue) {
		this->window = window;
		//this->states = states;

		//initialy, the view = window view. It may change during gamestate!
		stateview = new View();
		*stateview = window->GetDefaultView();
	}

	//Innit

	virtual void innitButtons(){}

	virtual void innitAudio(){}

	virtual ~State() {
		delete stateview;
		//delete window; //Do not!
	}

	float getLeftView() {
		return (stateview->getCenter().x - stateview->getSize().x / 2);
	}

	float getTopView() {
		return (stateview->getCenter().y - stateview->getSize().y / 2);
	}

	float getCenterViewX() {
		return (stateview->getCenter().x);
	}

	float getCenterViewY() {
		return (stateview->getCenter().y);
	}

	const sf::View* getView() {
		return this->stateview;
	}

	
	void Pause() {
		this->paused = true;
	}

	void Resume() {
		this->paused = false;
	}

	////dont call inside class
	//virtual void EndState() = 0;

	//----FUNCTIONS CALLED IN GAME HANDLING----// (char shooting, moving, physics)

	virtual void HandlingEvent(Event& evt, Time& dt) = 0; //character events (charge shoot)
														  // or just state button presses
	virtual void HandleInput(Time& dt) = 0; //character inputs

	virtual void HandleCollision() = 0; //for game state only


	//---FUNCTIONS CALLED IN GAME UPDATE-----// (update char's movements, physics

	virtual void Update(const float& dt) = 0; //this one is game + state
	

	//-----------CALL IN GAME RENDER----------------------
	virtual void Render(RenderWindow* target) = 0;

	
	//---Helpers

	// for states, call in Update()
	virtual void UpdateKeyBinds(const float& dt) = 0; //Ex: ESC to pause in GameState

	virtual void UpdateButtons() = 0;

	virtual void UpdateAudio() = 0;

	// for states, call in Update()
	void UpdateMouse() {//state-based detection of mouse
		auto win = window->GetRenderWindow();
		this->mousePos = win->mapPixelToCoords(sf::Mouse::getPosition(*win));
	}

	void ResetQueueLock() {
		this->lockQueueCommand = false;
	}
};

#endif