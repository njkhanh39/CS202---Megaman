#ifndef GAMESTATE_H
#define GAMESTATE_H
#include "camera.h"
#include "healthbar.h"
#include "weaponbar.h"
#include "PauseMenu.h"
#include "world.h"

class GameState : public State {
private:
	World* m_world;
	Character* m_character;
	Boss* m_boss; //borrow from world
	HealthBar* charHealthBar, *bossHealthBar;
	WeaponBar* charWeaponBar;
	Camera* camera;
	PauseMenu* pauseMenu;

	short int current_map;
public:
	GameState(MainWindow* window, TextureManager* textureManager, std::queue<STATECOMMAND>* statequeue, short int map_idx):
		State(window, textureManager, statequeue)
	, pauseMenu(nullptr), charHealthBar(nullptr), bossHealthBar(nullptr), m_boss(nullptr), current_map(map_idx){

		std::cout << "Creating GameState\n";

		if(map_idx==1){
			CreateCameraMap1();
			CreateWorld1();
			CreateCharacterAndUIs(50,50);
			
		}
		else if (map_idx == 2) {
			CreateCameraMap2();
			CreateWorld2();
			CreateCharacterAndUIs(55,302);
		}
		else {
			CreateCameraMap3();
			CreateWorld3();
			CreateCharacterAndUIs(15,1052);
		}
		

		

		CreateBossHealthBar();


		//this comes after camera. Every time we open, it needs to readjust
		
		CreatePauseMenu();
		innitAudio();
	}
	~GameState(){
		std::cout << "GameState destructor!\n";
		if (m_world) delete m_world;
		if (m_character) delete m_character;
		if (pauseMenu) delete pauseMenu;
		if (camera) delete camera;
		if (charHealthBar) delete charHealthBar;
		if (bossHealthBar) delete bossHealthBar;
		if (charWeaponBar) delete charWeaponBar;
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

	void UpdateAudio() {

	}


	//----FUNCTIONS CALLED IN GAME HANDLING----//

	void HandlingEvent(Event& evt, Time& dt) override {
		//-------------STATES-------------//

		//pausing = still renders background...

		//toggling
		if (evt.type == Event::KeyPressed && evt.key.code == Keyboard::Escape) {
			this->paused = !this->paused;

			if (this->paused) SoundManager::GetInstance().PauseSound();

			if (!this->paused) {
				//resume

				if (this->current_map == 3) SoundManager::GetInstance().PlaySound("chillpenguin", true);
				if (this->current_map == 2) SoundManager::GetInstance().PlaySound("flamemammoth", true);
				if (this->current_map == 1) SoundManager::GetInstance().PlaySound("vile", true);
			}
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
			this->charWeaponBar->Update(camera->GetViewCenter(), camera->GetViewSize(), m_character->GetActiveWeaponIndex());
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

					if (this->current_map == 1) this->statequeue->push(PUSH_GAMEOVER_1);
					else if (this->current_map == 2) this->statequeue->push(PUSH_GAMEOVER_2);
					else this->statequeue->push(PUSH_GAMEOVER_3);
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
			this->charWeaponBar->Render(target);

			if (m_boss) this->bossHealthBar->Render(target);

			//draw pause menu
			pauseMenu->Render(target);
		}
		else {
			m_world->Render(target);
			if(m_character && !m_character->IsDead()) m_character->Render(target);

			//draw the healthbar
			this->charHealthBar->Render(target);
			this->charWeaponBar->Render(target);

			if (m_boss) this->bossHealthBar->Render(target);
		}
	}

	short int GetCurrentMap() {
		return this->current_map;
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
		std::vector<FloatRect> tmp = {FloatRect(0,512,1884,10), FloatRect(0,0,2941,270), FloatRect(4413,232,962,58), 
		FloatRect(4158,-150, 1410,182), FloatRect(5368,512,2054,100), FloatRect(5725,25, 2030,247), FloatRect(7756,125,688,154),
		FloatRect(7391,480,365,255), FloatRect(7421,125,335,156)};

		camera = new Camera(0, 0, 350, 200, MAP2CONST::LEFTLIMIT2, MAP2CONST::RIGHTLIMIT2, MAP2CONST::UPLIMIT2,
			MAP2CONST::DOWNLIMIT2, MAP2CONST::BOSS_REGION_LEFT2, MAP2CONST::BOSS_REGION_RIGHT2, this->stateview, tmp);
	}

	void CreateWorld2() {
		m_world = new World(textureManager, MAP2CONST::BOSS_REGION_LEFT2, MAP2CONST::BOSS_REGION_RIGHT2, "Animation\\Map2\\"
			, "factory.png");
	}
	//-----------------------------------------//


	//-----------------WORLD 3-----------------//
	void CreateCameraMap3() {
		//these are restricted regions in a map that the camera cannot access
		std::vector<FloatRect> tmp = {FloatRect(213,0,2252,292), FloatRect(1020,491,595,200), FloatRect(1852,705,1071,225),
		FloatRect(2466,292,594,116), FloatRect(3098,705, 1342,208), FloatRect(4439,526,1262,223), FloatRect(5576,208,717,210)};

		camera = new Camera(0, 0, 350, 200, MAP3CONST::LEFTLIMIT3, MAP3CONST::RIGHTLIMIT3, MAP3CONST::UPLIMIT3,
			MAP3CONST::DOWNLIMIT3, MAP3CONST::BOSS_REGION_LEFT3, MAP3CONST::BOSS_REGION_RIGHT3, this->stateview, tmp);
	}

	void CreateWorld3() {
		m_world = new World(textureManager, MAP3CONST::BOSS_REGION_LEFT3, MAP3CONST::BOSS_REGION_RIGHT3, "Animation\\Map3\\"
			, "snow_fortress.png");
	}


	//-----------------------------------------//

	void CreateCharacterAndUIs(float x, float y) {
		m_character = new Character(textureManager,x, y);

		charHealthBar = new HealthBar(textureManager, "Animation\\X\\X_Healthbar.png",
			m_character->getHealth(), 30, 2.f, 2.f);

		charWeaponBar = new WeaponBar(textureManager, m_character->GetWeaponsIDs());
	}

	void CreateBossHealthBar() {
		bossHealthBar = new HealthBar(textureManager, "Animation\\Map1\\Vile\\Vile_HealthBar.png",
			1500, 40, 332.f, 2.f);
	}

	void CreatePauseMenu() {
		pauseMenu = new PauseMenu(window, this->getCenterViewX(), this->getCenterViewY());
	}

	void innitAudio() override {
		SoundManager::GetInstance().StopSound("menu");
		if (current_map == 1) {
			SoundManager::GetInstance().LoadSound("vile", "Audio\\Map1\\vile.mp3");
			SoundManager::GetInstance().SetSoundVolume("vile", 50);
			SoundManager::GetInstance().PlaySound("vile", true);
		}
		if (current_map == 2) {
			SoundManager::GetInstance().LoadSound("flamemammoth", "Audio\\Map2\\flamemammoth.mp3");
			SoundManager::GetInstance().SetSoundVolume("flamemammoth", 50);
			SoundManager::GetInstance().PlaySound("flamemammoth", true);
		}
		if (current_map == 3) {
			SoundManager::GetInstance().LoadSound("chillpenguin", "Audio\\Map3\\chillpenguin.mp3");
			SoundManager::GetInstance().SetSoundVolume("chillpenguin", 50);
			SoundManager::GetInstance().PlaySound("chillpenguin", true);
		}
	}
};



#endif