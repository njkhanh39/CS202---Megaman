#ifndef GAMESTATE_H
#define GAMESTATE_H
#include "character.h"
#include "State.h"
#include "world.h"

class GameState : public State {
private:
	World* m_world;
	Character* m_character;
public:
	GameState(MainWindow* window): State(window){
		m_world = new World();
		m_character = new Character(100, 150);
	}
	virtual ~GameState(){}
	
	void EndState() override {
		std::cout << "Ending game state!\n";
		delete m_world;
		delete m_character;
	}

	void UpdateKeyBinds(const float& dt) override {
		this->CheckForQuit(); //constantly checking
	}


	//----FUNCTIONS CALLED IN GAME HANDLING----//

	void HandlingEvent(Event& evt, Time& dt) override {
		//char shooting
		m_character->HandleEventInput(evt, dt);
	}

	void HandleInput(Time& dt) override {
		//character input from keyboard
		m_character->HandleMovements(dt);
	}

	void HandleCollision() override {
		//World + Char = everything :>
		m_world->HandleAllEntitiesCollisions(m_character);
	}

	//---FUNCTIONS CALLED IN GAME UPDATE-----//

	void Update(const float& dt) override {
		
		this->UpdateKeyBinds(dt);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
			std::cout << "Update gamestate, P pressed!\n";
		}

		//-----------------CHARACTER--------------------

		m_character->Update(dt);

		//-----------------ENEMIES--------------------

		m_world->UpdateAllEnemies(m_character, dt);

		//------------MOUSE
		this->UpdateMousePosition();
	}

	//-------------------------------------------------

	void Render(RenderWindow* target) override {
		m_world->Render(target);
		m_character->Render(target);
	}
};


#endif