#ifndef MAINMENU_H
#define MAINMENU_H

#include "GameoverState.h"


class MainMenu : public State {
private:
	RectangleShape* background;
	Button* new_game_btn, *quit_game_btn;
public:
	MainMenu(MainWindow* window,TextureManager* textureManager, std::queue<STATECOMMAND>* statequeue):
		background(nullptr) , State(window,textureManager, statequeue) {
		background = new RectangleShape({ 1280.f,720.f });
		background->setFillColor(Color::Black);

		this->innitButtons();
	}

	//innit button
	void innitButtons() override {
		sf::Font fnt; fnt.loadFromFile("Fonts\\palatinobold.ttf");

		new_game_btn = new Button(600, 300, 150, 50, fnt, "NEW GAME", Color::White, Color(255, 153, 51),
			Color(255, 153, 51));
		quit_game_btn = nullptr;
	}

	virtual ~MainMenu() {
		if(background) delete background;

		//buttons
		if(new_game_btn) delete new_game_btn;
	}

	/*void EndState() override {
		std::cout << "Ending main menu state!\n";
	}*/

	void UpdateButtons() override {
		this->new_game_btn->Update(mousePos); // appearance

		if (this->new_game_btn->isPressed()) {
			std::cout << "New game button pressed :>\n";
			//push game state
			this->new_game_btn->SetIdle();
			
			//QUEUE GAME STATE

		//	this->statequeue->push(STATECOMMAND::QUIT); 
			if (lockQueueCommand == false) {
				lockQueueCommand = true;
				this->statequeue->push(STATECOMMAND::PUSH_GAMESTATE);
			}
		}
	}

	void UpdateKeyBinds(const float& dt) override {
		
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
	}

	//-----------RENDER------------------------------

	void Render(RenderWindow* target) override {
		target->draw(*background);
		new_game_btn->Render(target);
	}
};

#endif