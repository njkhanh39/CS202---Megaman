#ifndef GAMEOVERSTATE_H
#define GAMEOVERSTATE_H

#include "GameState.h"

class GameoverState: public State {
private:
	Texture* texture; //borrow 612 x 108
	Sprite* background;
	Button* yesButton, * noButton;

	short int current_map;
public:
	GameoverState(MainWindow* window, TextureManager* textureManager,std::queue<STATECOMMAND>* statequeue, short int current_map) :
		State(window, textureManager, statequeue), yesButton(nullptr), noButton(nullptr), current_map(current_map){
		this->textureManager->BorrowTexture("Animation\\gameover.jpg", this->texture);


		background = new Sprite();
		if (texture) background->setTexture(*texture);
		else return;

		//----------VIEW---------//

		sf::Vector2u size = texture->getSize();

		this->stateview->setSize(Vector2f(size.x, size.y));
		this->stateview->setCenter(Vector2f(size.x / 2, size.y / 2));


		innitButtons();

		//stop sound

		SoundManager::GetInstance().StopSound();
	}

	virtual ~GameoverState() {
		delete background;
		delete yesButton;
		delete noButton;
	}

	void innitButtons() override {
		sf::Font fnt; fnt.loadFromFile("Fonts\\palatinobold.ttf");
		//buttons
		yesButton = new Button(247, 283, 58, 37, fnt, "", Color::Transparent, Color::Transparent, Color::Transparent);
		noButton = new Button(324, 283, 58, 37, fnt, "", Color::Transparent, Color::Transparent, Color::Transparent);
	}

	/*void EndState() override {
		std::cout << "Ending main menu state!\n";

		
	}*/

	void UpdateButtons() override {
		this->noButton->Update(mousePos); // appearance
		this->yesButton->Update(mousePos);

		if (this->noButton->isPressed()) {
			this->noButton->SetIdle();
			
			if (!this->lockQueueCommand) { //back to mainmenu
				lockQueueCommand = true;
				this->statequeue->push(QUIT);
			}
		}

		if (this->yesButton->isPressed()) {
			this->yesButton->SetIdle();

			if (!this->lockQueueCommand) {
				lockQueueCommand = true;
				this->statequeue->push(QUIT);

				if(this->current_map == 1) this->statequeue->push(PUSH_GAMESTATE_1);
				else if(this->current_map == 2) this->statequeue->push(PUSH_GAMESTATE_2);
				else this->statequeue->push(PUSH_GAMESTATE_3);
			}
		}
	}

	void UpdateKeyBinds(const float& dt) override {

	}

	void UpdateAudio() {

	}

	//----FUNCTIONS CALLED IN GAME HANDLING----//

	void HandlingEvent(Event& evt, Time& dt) override {
		this->noButton->Handling(this->mousePos, evt);
		this->yesButton->Handling(this->mousePos, evt);
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
		//new_game_btn->Render(target);
	}
};

#endif