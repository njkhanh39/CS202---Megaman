#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "State.h"

class GameState : public State {
private:

public:
	GameState(MainWindow* window): State(window){}
	virtual ~GameState(){}
	
	void EndState() override {
		std::cout << "Ending game state!\n";
	}

	void UpdateKeyBinds(const float& dt) override {
		this->CheckForQuit(); //constantly checking
	}

	void Update(const float& dt) override {
		
		this->UpdateKeyBinds(dt);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
			std::cout << "Update gamestate, P pressed!\n";
		}
	}
	void Render(MainWindow* target) override {

	}
};


#endif