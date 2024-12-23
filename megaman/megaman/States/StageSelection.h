#include "GameoverState.h"

class StageSelection : public State {
private:
	RectangleShape* background;
	Texture* stage_select_text; //borrow
	Sprite* stage_select; 

	//buttons
	
	Button* stages[3];
	Button* back;
public:
	StageSelection(MainWindow* window, TextureManager* textureManager, std::queue<STATECOMMAND>* statequeue) :
		State(window, textureManager, statequeue), background(nullptr) {

		this->innitBackground();
		this->innitPics();
		this->innitButtons();
		this->innitAudio();
		this->innitStateView();
	}

	virtual ~StageSelection() {
		delete stage_select;
		for (int i = 0; i < 3; ++i) delete stages[i];
		delete back;
	}

	void innitStateView() {
		float width = 378, height = 212;
		this->stateview->setSize(sf::Vector2f(width,height));
		this->stateview->setCenter(sf::Vector2f(width / 2, height / 2));
	}

	void innitBackground() {
		background = new RectangleShape({ 1280.f,720.f });
		background->setFillColor(Color::Black);
	}

	void innitPics() {

		stage_select = new Sprite();

		this->textureManager->BorrowTexture("Animation\\StageSelect\\bosses.png", stage_select_text);
		if (stage_select_text) stage_select->setTexture(*stage_select_text);

	}

	//innit buttons
	void innitButtons() override {
		sf::Font fnt; fnt.loadFromFile("Fonts\\audiowide.ttf");

		float left = 16.f;
		float top = 91.f;
		for (int i = 0; i < 3; ++i) {
			stages[i] = new Button(left + i*123.f, top, 102, 102, fnt, "", Color::Transparent, Color(255, 153, 51), Color(255, 153, 51));
		}

		float left_back = 314.f;
		float top_back = 18.f;
		back = new Button(left_back, top_back, 43, 43, fnt, "", Color::Transparent, Color(255, 153, 51), Color(255, 153, 51));
	}

	void innitAudio() override {

	}

	void UpdateButtons() override {
		//appearance

		for (int i = 0; i < 3; ++i) {
			stages[i]->Update(mousePos); 
		}
		back->Update(mousePos);

		//function

		for (int i = 0; i < 3; ++i) {
			if (stages[i]->isPressed()) {
				stages[i]->SetIdle();

				if (lockQueueCommand == false) {
					lockQueueCommand = true;

					if(i==0) this->statequeue->push(STATECOMMAND::PUSH_GAMESTATE_1);
					else if (i == 1) {
						this->statequeue->push(STATECOMMAND::PUSH_GAMESTATE_3); //a little weird here
					}
					else this->statequeue->push(STATECOMMAND::PUSH_GAMESTATE_2);
				}
			}
		}

		if (this->back->isPressed()) {
			std::cout << "back pressed \n";
			this->back->SetIdle();

			//pop this

			if (lockQueueCommand == false) {
				lockQueueCommand = true;
				this->statequeue->push(STATECOMMAND::QUIT);
			}
		}
	}

	void UpdateKeyBinds(const float& dt) override {

	}

	void UpdateAudio() override {

	}

	//----FUNCTIONS CALLED IN GAME HANDLING----//

	void HandlingEvent(Event& evt, Time& dt) override {
		for (int i = 0; i < 3; ++i) {
			this->stages[i]->Handling(this->mousePos, evt);
		}
		back->Handling(this->mousePos, evt);
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
		target->draw(*stage_select);

		for (int i = 0; i < 3; ++i) stages[i]->Render(target);
		back->Render(target);
	}
};