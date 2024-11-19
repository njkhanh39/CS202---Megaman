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

	sf::Vector2f mousePos;
public:
	State(MainWindow* window): quit(false), mousePos({0,0}) {
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


	//----FUNCTIONS CALLED IN GAME HANDLING----//

	virtual void HandlingEvent(Event& evt, Time& dt) = 0;

	virtual void HandleInput(Time& dt) = 0;

	//for game state only
	virtual void HandleCollision() = 0;

	//---FUNCTIONS CALLED IN GAME UPDATE-----//

	virtual void EndState() = 0;

	virtual void UpdateKeyBinds(const float& dt) = 0;

	virtual void Update(const float& dt) = 0;
	virtual void Render(RenderWindow* target) = 0;

	//---Update others

	void UpdateMousePosition() {
		auto win = window->GetRenderWindow();

		this->mousePos = win->mapPixelToCoords(sf::Mouse::getPosition(*win));
		//std::cout << "Pos: " << mousePos.x << "," << mousePos.y << '\n';
	}
};

#endif