#ifndef GAMESTATE_H
#define GAMESTATE_H
#include "camera.h"
#include "healthbar.h"
#include "PauseMenu.h"
#include "world.h"

class GameState : public State {
private:
	World* m_world;
	Character* m_character;
	HealthBar* charHealthBar;

	Camera* camera;
	PauseMenu* pauseMenu;
public:
	GameState(MainWindow* window, TextureManager* textureManager, std::queue<STATECOMMAND>* statequeue):
		State(window, textureManager, statequeue)
	, pauseMenu(nullptr), charHealthBar(nullptr){

		std::cout << "Creating GameState\n";

		m_world = new World(textureManager);

		m_character = new Character(textureManager, 50, 50);
		
		CreateCameraMap1();

		CreateCharacterHealthBar();

		//this comes after camera. Every time we open, it needs to readjust

		pauseMenu = new PauseMenu(window, this->getCenterViewX(), this->getCenterViewY());
	}
	~GameState(){
		std::cout << "GameState destructor!\n";
		if (m_world) delete m_world;
		if (m_character) delete m_character;
		if (pauseMenu) delete pauseMenu;
		if (camera) delete camera;
		if (charHealthBar) delete charHealthBar;
	}

	////dont call inside class
	//void EndState() override {
	//	std::cout << "Ending game state!\n";
	//	this->quit = true;
	//}

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

		if (evt.type == evt.KeyPressed && evt.key.code == sf::Keyboard::P) {
			auto v = camera->GetTopLeftCoordinates();
			std::cout << v.x << "," << v.y << '\n';
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
			if(!m_character->IsDead()) m_character->HandleEventInput(evt, dt);
		}

	}

	void HandleInput(Time& dt) override {
		if (!this->paused) {
			//character input from keyboard
			if(!m_character->IsDead()) m_character->HandleMovements(dt);
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

			this->charHealthBar->Update(m_character, camera->GetViewCenter(), camera->GetViewSize());

			if (!m_character->IsDead()) {
				

				m_character->Update(dt);


				//update view

				camera->UpdateFollowCharacter(m_character);
			}
			else {
				if (!this->lockQueueCommand) {
					lockQueueCommand = true;
					this->statequeue->push(QUIT);
					this->statequeue->push(PUSH_GAMEOVER);
				}
			}

			//-----------------WORLD AND ENEMIES--------------------

			 m_world->UpdateWorld(m_character, dt);

		}
		else { //update the pause menu

			pauseMenu->Update(mousePos, Vector2f(getCenterViewX(), getCenterViewY())); //appearance

			if (pauseMenu->ReturnQuit()) { //if returns quit, we go all the way to main menu

				if (this->lockQueueCommand == false) {
					lockQueueCommand = true;
					this->statequeue->push(QUIT);
				}
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
			if(m_character && !m_character->IsDead()) m_character->Render(target);

			//draw the healthbar
			this->charHealthBar->Render(target);

			//draw pause menu
			pauseMenu->Render(target);
		}
		else {
			m_world->Render(target);
			if(m_character && !m_character->IsDead()) m_character->Render(target);

			//draw the healthbar
			this->charHealthBar->Render(target);
		}
	}

private:
	void CreateCameraMap1() {
		camera = new Camera(0, 0, 350, 200, MAP1CONST::UPBOUND, MAP1CONST::LOWBOUND, MAP1CONST::UPLIMIT,
			MAP1CONST::DOWNLIMIT, this->stateview);
	}

	void CreateCharacterHealthBar() {
		charHealthBar = new HealthBar(textureManager, "Animation\\X\\X_Healthbar.png",
			m_character->getHealth(), 30, 2.f, 2.f);
	}
};



#endif