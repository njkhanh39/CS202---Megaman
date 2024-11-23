#ifndef STATE_H
#define STATE_H
#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <SFML/Graphics.hpp>
#include "window.h"
#include "Auxiliaries/Button.h"


class State {
protected:
	MainWindow* window; //points to game->m_window??
	std::vector<sf::Texture> textures;
	bool quit;
	bool paused;

	//mouse
	sf::Vector2f mousePos;

	//it needs to know this, so that states can be pushed from a state
	std::stack<State*>* states;

	//---VIEW

	sf::View view;
public:
	State(MainWindow* window, std::stack<State*>* states): view(window->GetDefaultView()), paused(false), quit(false), mousePos({0,0}) {
		this->window = window;
		this->states = states;
	}

	//Innit

	virtual void innitButtons(){}

	virtual ~State() {
		//delete window; //Do not!
	}


	const bool& getQuit() const {
		return this->quit;
	}
	
	void Pause() {
		this->paused = true;
	}

	void Resume() {
		this->paused = false;
	}

	//dont call inside class
	virtual void EndState() = 0;

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

	// for states, call in Update()
	void UpdateMouse() {//state-based detection of mouse
		auto win = window->GetRenderWindow();
		this->mousePos = win->mapPixelToCoords(sf::Mouse::getPosition(*win));
	}

	sf::View& GetView() { return this->view; }
};

#endif