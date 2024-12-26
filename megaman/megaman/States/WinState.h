#ifndef WINSTATE_H
#define WINSTATE_H
#include "GameState.h"

class WinState : public State {
	sf::Clock clk;
	Button* win_view;
public:
	WinState(MainWindow* window, TextureManager* textureManager, std::queue<STATECOMMAND>* statequeue) :
		State(window, textureManager, statequeue) {

		innitPics();
	}

	~WinState() {
		delete win_view;
	}

	void innitPics() {
		sf::Font fnt;
		fnt.loadFromFile("Fonts\\audiowide.ttf");
		win_view = new Button(100, 100, 1080, 100, fnt, 100, "STAGE COMPLETED", Color(255, 153, 51), Color::Transparent, Color::Transparent
			, Color::Transparent);
	}

	void innitAudio() override {
		//victory sound
	}


	void UpdateKeyBinds(const float& dt) override {

	}

	void UpdateAudio() override {

	}

	void UpdateButtons() override {

	}

	//----FUNCTIONS CALLED IN GAME HANDLING----//

	void HandlingEvent(Event& evt, Time& dt) override {
		
	}

	void HandleInput(Time& dt) override {

	}

	void HandleCollision() override {

	}

	//---FUNCTIONS CALLED IN GAME UPDATE-----//

	void Update(const float& dt) override {

		if (clk.getElapsedTime().asSeconds() >= 3) {
			if (!this->lockQueueCommand) {
				this->lockQueueCommand;

				statequeue->push(STATECOMMAND::QUIT); //quit win state
				statequeue->push(STATECOMMAND::QUIT); //quit game
				statequeue->push(STATECOMMAND::PUSH_STAGESELECTION);
			}
		}


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
		this->win_view->Render(target);
	}
};

#endif // !WINSTATE_H

