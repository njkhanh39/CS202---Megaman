#ifndef MAINMENU_H
#define MAINMENU_H

#include "GameState.h"


class MainMenu : public State {
private:
	RectangleShape* background;
	Button* mainMenu_button;
public:
	MainMenu(MainWindow* window): background(nullptr) , State(window) {
		background = new RectangleShape({ 1600.f,900.f });
		background->setFillColor(Color::Black);

		sf::Font fnt; fnt.loadFromFile("Fonts\\palatinobold.ttf");

		mainMenu_button = new Button(100, 100, 150, 50, fnt, "NEW GAME", Color::White, Color(255, 153, 51),
			Color(255, 153, 51));

	}
	virtual ~MainMenu() {
		delete background;
		delete mainMenu_button;
	}

	void EndState() override {
		std::cout << "Ending main menu state!\n";

		delete background;
	}

	void UpdateKeyBinds(const float& dt) override {
		this->CheckForQuit(); //constantly checking
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
		this->UpdateKeyBinds(dt);
		this->UpdateMousePosition();

		this->mainMenu_button->Update(mousePos);
	}

	//-------------------------------------------------

	void Render(RenderWindow* target) override {
		target->draw(*background);
		mainMenu_button->Render(target);
	}
};

#endif