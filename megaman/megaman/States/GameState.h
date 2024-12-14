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
	Boss* m_boss; //borrow from world
	HealthBar* charHealthBar, *bossHealthBar;
	Camera* camera;
	PauseMenu* pauseMenu;
public:
	GameState(MainWindow* window, TextureManager* textureManager, std::queue<STATECOMMAND>* statequeue):
		State(window, textureManager, statequeue)
	, pauseMenu(nullptr), charHealthBar(nullptr), bossHealthBar(nullptr), m_boss(nullptr){

		std::cout << "Creating GameState\n";

		CreateCameraMap2();

		CreateWorld2();

		CreateCharacterAndHealthBar();

		CreateBossHealthBar();


		//this comes after camera. Every time we open, it needs to readjust
		
		CreatePauseMenu();
	}
	~GameState(){
		std::cout << "GameState destructor!\n";
		if (m_world) delete m_world;
		if (m_character) delete m_character;
		if (pauseMenu) delete pauseMenu;
		if (camera) delete camera;
		if (charHealthBar) delete charHealthBar;
		if (bossHealthBar) delete bossHealthBar;
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
			camera->GetViewInfo();
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


		//------CAMERA

			this->charHealthBar->Update(m_character, camera->GetViewCenter(), camera->GetViewSize());
			if (m_boss) this->bossHealthBar->Update(m_boss, camera->GetViewCenter(), camera->GetViewSize());
	//-----------------CHARACTER--------------------
			if (!m_character->IsDead()) {
				

				m_character->Update(dt);
				camera->Update(m_character, dt);
				// ---- BOSS FIGHT-- -//

				if (m_character->IsInBossRegion(m_world->GetBossRegion())) {
					m_world->isBossFight = true;

					 if(m_world->GetBossPointer() && !m_boss) m_boss = m_world->GetBossPointer();
				}

				//update view

				
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

			if (m_boss) this->bossHealthBar->Render(target);

			//draw pause menu
			pauseMenu->Render(target);
		}
		else {
			m_world->Render(target);
			if(m_character && !m_character->IsDead()) m_character->Render(target);

			//draw the healthbar
			this->charHealthBar->Render(target);

			if (m_boss) this->bossHealthBar->Render(target);
		}
	}

private:

	//-----------------WORLD 1-----------------//
	void CreateCameraMap1() {
		//these are restricted regions in a map that the camera cannot access
		std::vector<FloatRect> tmp = { FloatRect(0, 200, 2358, 200), FloatRect(3100, 200, 4000, 200) };

		camera = new Camera(0, 0, 350, 200, MAP1CONST::LEFTLIMIT, MAP1CONST::RIGHTLIMIT, MAP1CONST::UPLIMIT,
			MAP1CONST::DOWNLIMIT, MAP1CONST::BOSS_REGION_LEFT, MAP1CONST::BOSS_REGION_RIGHT, this->stateview, tmp);
	}

	void CreateWorld1() {
		m_world = new World(textureManager, MAP1CONST::BOSS_REGION_LEFT, MAP1CONST::BOSS_REGION_RIGHT, "Animation\\Map1\\"
			, "highway.png");
	}
	//-----------------------------------------//

	//-----------------WORLD 2-----------------//
	void CreateCameraMap2() {
		//these are restricted regions in a map that the camera cannot access
		std::vector<FloatRect> tmp = {FloatRect(0,512,1884,10), FloatRect(0,0,2941,270)};

		camera = new Camera(0, 0, 350, 200, MAP2CONST::LEFTLIMIT2, MAP2CONST::RIGHTLIMIT2, MAP2CONST::UPLIMIT2,
			MAP2CONST::DOWNLIMIT2, MAP2CONST::BOSS_REGION_LEFT2, MAP2CONST::BOSS_REGION_RIGHT2, this->stateview, tmp);
	}

	void CreateWorld2() {
		m_world = new World(textureManager, MAP2CONST::BOSS_REGION_LEFT2, MAP2CONST::BOSS_REGION_RIGHT2, "Animation\\Map2\\"
			, "factory.png");
	}
	//-----------------------------------------//

	void CreateCharacterAndHealthBar() {
		m_character = new Character(textureManager, 50, 350);

		charHealthBar = new HealthBar(textureManager, "Animation\\X\\X_Healthbar.png",
			m_character->getHealth(), 30, 2.f, 2.f);
	}

	void CreateBossHealthBar() {
		bossHealthBar = new HealthBar(textureManager, "Animation\\Map1\\Vile\\Vile_HealthBar.png",
			1000, 40, 332.f, 2.f);
	}

	void CreatePauseMenu() {
		pauseMenu = new PauseMenu(window, this->getCenterViewX(), this->getCenterViewY());
	}
};



#endif