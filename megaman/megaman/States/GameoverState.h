#ifndef GAMEOVERSTATE_H
#define GAMEOVERSTATE_H

#include "GameState.h"

class GameoverState: public State {
private:
	Texture* texture; //borrow
	Sprite* background;
public:
	GameoverState(MainWindow* window, TextureManager* textureManager,std::queue<STATECOMMAND>* statequeue) :
		State(window, textureManager, statequeue) {

		this->textureManager->BorrowTexture("Animation\\gameover.jpg", this->texture);

		if (texture) background->setTexture(*texture);
		else return;

		//----------VIEW---------//

		sf::Vector2u size = texture->getSize();

		this->stateview->setSize(Vector2f(size.x, size.y));
		this->stateview->setCenter(Vector2f(size.x / 2, size.y / 2));
	}

	virtual ~GameoverState() {
		delete background;
	}

	/*void EndState() override {
		std::cout << "Ending main menu state!\n";

		
	}*/

	void UpdateButtons() override {
		//this->new_game_btn->Update(mousePos); // appearance

		//if (this->new_game_btn->isPressed()) {
		//	std::cout << "New game button pressed :>\n";
		//	//push game state
		//	this->new_game_btn->SetIdle();
		//	this->states->push(new GameState(this->window, textureManager, this->states));
		//}
	}

	void UpdateKeyBinds(const float& dt) override {

	}


	//----FUNCTIONS CALLED IN GAME HANDLING----//

	void HandlingEvent(Event& evt, Time& dt) override {
		//this->new_game_btn->Handling(this->mousePos, evt);
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