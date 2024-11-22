#ifndef PAUSEMENU_H
#define PAUSEMENU_H
#include "State.h"

//UI for GameState, technically not a state
class PauseMenu {
private:
	MainWindow* window;

	Button* pauseFrame;
	Text* text;

	Button* resume, * quit;

	bool isActive;

	bool returnResume = false, returnQuit = false;
public:

	PauseMenu(MainWindow* win) : window(win), text(nullptr), isActive(false) {
		sf::Font fnt; fnt.loadFromFile("Fonts\\palatinobold.ttf");
		pauseFrame = new Button(500, 300, 350, 150, fnt, "", Color(47, 79, 79), Color(47, 79, 79),
			Color(47, 79, 79));

		text = new Text();
		text->setString("PAUSE");
		text->setPosition({ 600.f, 325.f });
		text->setFont(fnt);
		text->setCharacterSize(20);

		resume = new Button(550, 375, 100, 50, fnt, "RESUME", Color::White, Color(255, 153, 51), Color(255, 153, 51));
		quit = new Button(700, 375, 100, 50, fnt, "QUIT", Color::White, Color(255, 153, 51), Color(255, 153, 51));

	}

	~PauseMenu() {
		delete pauseFrame;
		delete text;
		delete resume;
		delete quit;
	}

	void SetActive() {
		this->isActive = true;
	}

	void SetUnActive() {
		this->isActive = false;
	}

	bool ReturnResume() {
		return returnResume;
	}

	bool ReturnQuit() {
		return returnQuit;
	}

	void Handling(Vector2f mousePos, sf::Event& evt) {
		if (this->isActive) {
			resume->Handling(mousePos, evt);
			quit->Handling(mousePos, evt);
		}
	}

	void Update(Vector2f mousePos) {
		if (this->isActive) {
			resume->Update(mousePos);
			quit->Update(mousePos);

			if (resume->isPressed()) {
				returnResume = true;
			}
			else returnResume = false;

			if (quit->isPressed()) {
				returnQuit = true;
			}
			else returnQuit = false;
		}
	}

	void Render(RenderWindow* target) {
		pauseFrame->Render(target);
		resume->Render(target);
		quit->Render(target);

		/*if (target) {
			target->draw(*this->text);
		}
		else {
			std::cout << "?\n";
		}*/
	}
};

#endif