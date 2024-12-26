#ifndef PAUSEMENU_H
#define PAUSEMENU_H
#include "State.h"

//UI for GameState, technically not a state
class PauseMenu {
private:
	MainWindow* window;

	Sprite* sprite;

	Texture* borrow;
	TextureManager* textureManager; //borrow

	Button* resume, * quit;

	bool isActive;

	bool returnResume = false, returnQuit = false;
public:

	PauseMenu(MainWindow* win,TextureManager* textureManager, float x, float y) : window(win), isActive(false),
	textureManager(textureManager){

		sf::Font fnt;
		fnt.loadFromFile("Fonts\\audiowide.ttf");
		
		textureManager->BorrowTexture("Animation\\PauseMenu\\pause.png", borrow);


		sprite = new Sprite();
		if(borrow) sprite->setTexture(*borrow);

		resume = new Button(x+9.f, y+56.f, 50, 10, fnt, 10, "", Color::Transparent, Color(255, 153, 51), Color(255, 153, 51));
		quit = new Button(x+84.f, y+56.f, 50, 10, fnt, 10, "", Color::Transparent, Color(255, 153, 51), Color(255, 153, 51));

		//set pos

		this->UpdateButtonPositions(Vector2f(x, y));

	}

	~PauseMenu() {
		delete sprite;
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
		target->draw(*sprite);
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
	void UpdateButtonPositions(Vector2f viewCenter) {
		float x = viewCenter.x;
		float y = viewCenter.y;

		Vector2f newPos = Vector2f(x - borrow->getSize().x / 2, y - borrow->getSize().y / 2);

		sprite->setPosition(newPos);
		resume->setPosition(Vector2f(newPos.x + 9.f, newPos.y + 56.f));
		quit->setPosition(Vector2f(newPos.x + 84.f, newPos.y + 56.f));
	}
};

#endif