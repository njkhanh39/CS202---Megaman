#ifndef MAINMENU_H
#define MAINMENU_H

#include "StageSelection.h"


class MainMenu : public State {
private:
	RectangleShape* background;
	Button* new_game_btn, *quit_game_btn;

	Button* title_text;
public:
	MainMenu(MainWindow* window,TextureManager* textureManager, std::queue<STATECOMMAND>* statequeue):
		background(nullptr) , State(window,textureManager, statequeue) {
		background = new RectangleShape({ 1280.f,720.f });
		background->setFillColor(Color::Black);

		this->innitButtons();
		this->innitAudio();
	}

	//innit button
	void innitButtons() override {
		sf::Font fnt; fnt.loadFromFile("Fonts\\audiowide.ttf");

		new_game_btn = new Button(500, 300, 300, 80, fnt, 30, "PLAY", Color::White, Color(255, 153, 51),
			Color(255, 153, 51));

		quit_game_btn = nullptr;

		title_text = new Button(100, 100, 1080, 100, fnt, 100, "MEGAMAN X", Color::Cyan, Color::Transparent, Color::Transparent
			, Color::Transparent);
	}

	void innitAudio() override {
		SoundManager::GetInstance().LoadSound("menu", "Audio\\MainMenu\\mainmenu.mp3");
		SoundManager::GetInstance().PlaySound("menu", false);
	}

	virtual ~MainMenu() {
		if(background) delete background;

		//buttons
		if(new_game_btn) delete new_game_btn;

		if (title_text) delete title_text;
	}

	/*void EndState() override {
		std::cout << "Ending main menu state!\n";
	}*/

	void UpdateButtons() override {
		this->new_game_btn->Update(mousePos); // appearance

		if (this->new_game_btn->isPressed()) {
			std::cout << "New game button pressed :>\n";
			this->new_game_btn->SetIdle();
			
			//QUEUE STAGE SELECTION

		//	this->statequeue->push(STATECOMMAND::QUIT); 
			if (lockQueueCommand == false) {
				lockQueueCommand = true;
				this->statequeue->push(STATECOMMAND::PUSH_STAGESELECTION);
			}
		}
	}

	void UpdateKeyBinds(const float& dt) override {
		
	}

	void UpdateAudio() override{
		
	}


	//----FUNCTIONS CALLED IN GAME HANDLING----//

	void HandlingEvent(Event& evt, Time& dt) override {
		this->new_game_btn->Handling(this->mousePos, evt);
	}

	void HandleInput(Time& dt) override {

	}

	void HandleCollision() override {

	}

	//---FUNCTIONS CALLED IN GAME UPDATE-----//

	void Update(const float& dt) override {

		
		this->UpdateKeyBinds(dt);

		//------------MOUSE
		this->UpdateMouse();

		//------------BUTTON
		this->UpdateButtons();

		//audio

		this->UpdateAudio();
	}

	//-----------RENDER------------------------------

	void Render(RenderWindow* target) override {
		target->draw(*background);
		new_game_btn->Render(target);
		title_text->Render(target);
	}
};

#endif