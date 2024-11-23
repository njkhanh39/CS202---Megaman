#ifndef GAMESTATE_H
#define GAMESTATE_H
#include "character.h"
#include "PauseMenu.h"
#include "world.h"

class GameState : public State {
private:
	World* m_world;
	Character* m_character;

	PauseMenu* pauseMenu;
public:
	GameState(MainWindow* window, TextureManager* textureManager, std::stack<State*>* states):
		State(window, textureManager, states)
	, pauseMenu(nullptr){
		std::cout << "Creating GameState\n";
		m_world = new World(textureManager);
		m_character = new Character(textureManager, 100, 150);

		pauseMenu = new PauseMenu(window);
	}
	~GameState(){
		std::cout << "GameState destructor!\n";
		if (m_world) delete m_world;
		if (m_character) delete m_character;
		if (pauseMenu) delete pauseMenu;
	}

	//dont call inside class
	void EndState() override {
		std::cout << "Ending game state!\n";
		this->quit = true;
	}

	void UpdateButtons() override {
		
	}

	void UpdateKeyBinds(const float& dt) override {
		
	}


	//----FUNCTIONS CALLED IN GAME HANDLING----//

	void HandlingEvent(Event& evt, Time& dt) override {
		//-------------STATES-------------//

		//pausing = still renders background...

		//toggling
		if (evt.type == Event::KeyPressed && evt.key.code == Keyboard::Escape) {
			this->paused = !this->paused;
		}

		//-----------PAUSEMENU-----------

		if (this->paused) {
			pauseMenu->SetActive();
			pauseMenu->Handling(mousePos, evt);
		}
		else {
			pauseMenu->SetUnActive();
		}

		//-------------GAME-------------//
		//char shooting
		if (!this->paused) {
			m_character->HandleEventInput(evt, dt);
		}
	}

	void HandleInput(Time& dt) override {
		if (!this->paused) {
			//character input from keyboard
			m_character->HandleMovements(dt);
		}
	}

	void HandleCollision() override {
		if (!this->paused) {
			//World + Char = everything :>
			m_world->HandleAllEntitiesCollisions(m_character);
		}
	}

	//---FUNCTIONS CALLED IN GAME UPDATE-----//

	void Update(const float& dt) override {
		//------------MOUSE
		this->UpdateMouse(); //state-based detection of mouse

		if (!this->paused) {
			this->UpdateKeyBinds(dt);

			

			//------------BUTTON
			this->UpdateButtons();

			//-----------------CHARACTER--------------------

			m_character->Update(dt);

			//-----------------WORLD AND ENEMIES--------------------

			m_world->UpdateAllEnemies(m_character, dt);

		}
		else { //update the pause menu

			pauseMenu->Update(mousePos); //appearance

			if (pauseMenu->ReturnQuit()) { //if returns quit, we go all the way to main menu
				this->EndState();
			}

			if (pauseMenu->ReturnResume()) {// hide the pauseMenu
				this->paused = false;
				pauseMenu->SetUnActive();
			}
		}
	}

	//-------------------------------------------------

	void Render(RenderWindow* target) override {
		if (this->paused) {
			//draw this first, always
			m_world->Render(target);
			m_character->Render(target);
			//draw pause menu
			pauseMenu->Render(target);
		}
		else {
			m_world->Render(target);
			m_character->Render(target);
		}
	}
};




#endif