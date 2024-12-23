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

	PauseMenu(MainWindow* win, float x, float y) : window(win), text(nullptr), isActive(false) {
		sf::Font fnt; fnt.loadFromFile("Fonts\\palatinobold.ttf");
		pauseFrame = new Button(x, y, 150, 50, fnt, "", Color(47, 79, 79), Color(47, 79, 79),
			Color(47, 79, 79));

		text = new Text();
		text->setString("PAUSE");
		text->setPosition({x+50.f, y+15.f});
		text->setFont(fnt);
		text->setCharacterSize(10);

		resume = new Button(x+15.f, y+25.f, 50, 15, fnt, 10, "RESUME", Color::White, Color(255, 153, 51), Color(255, 153, 51));
		quit = new Button(x+75.f, y+25.f, 50, 15, fnt, 10, "QUIT", Color::White, Color(255, 153, 51), Color(255, 153, 51));

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

	void Update(Vector2f mousePos, Vector2f viewCenter) {
		if (this->isActive) {

			this->UpdateButtonPositions(viewCenter);

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

private:
	void UpdateButtonPositions(Vector2f newPos) {
		float x = newPos.x;
		float y = newPos.y;

		pauseFrame->setPosition(newPos);
		text->setPosition({ x + 50.f, y + 15.f });
		resume->setPosition(Vector2f(x + 15.f, y + 25.f));
		quit->setPosition(Vector2f(x + 75.f, y + 25.f));
	}
};

#endif