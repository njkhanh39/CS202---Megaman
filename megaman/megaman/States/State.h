#ifndef STATE_H
#define STATE_H
#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <SFML/Graphics.hpp>
#include "window.h"

class State {
private:
	MainWindow* window; //points to game->m_window??
	std::vector<sf::Texture> textures;
	bool quit;

public:
	State(MainWindow* window): quit(false) {
		this->window = window;
	}

	virtual ~State() {
		//delete window; //Do not!
	}

	const bool& getQuit() const {
		return this->quit;
	}

	virtual void CheckForQuit() {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			std::cout << "Escape pressed, setting state-quit boolean!\n";
			this->quit = true;
		}
	}

	//----PURE VIRTUALS----

	virtual void EndState() = 0;

	virtual void UpdateKeyBinds(const float& dt) = 0;

	virtual void Update(const float& dt) = 0;
	virtual void Render(MainWindow* target) = 0;
};

#endif