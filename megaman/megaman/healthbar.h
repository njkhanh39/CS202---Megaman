#pragma once
#include "character.h"
#include "Auxiliaries/Button.h"

class HealthBar {
private:
	TextureManager* textureManager;
	Texture* frame;
	Sprite* sprite;
	RectangleShape blackrect;

	
	int curbars;

	const int totalbars;
	const int totalhealth;

	const Vector2f defaultstartpoint = { 3.6,4 }; //unscaled = 9, 10
	const Vector2f defaultsize = { 8.8,2.4 }; //unscaled = 22,6
	const Vector2f margin;
	const float defaultscale = 0.4;
public:
	HealthBar(TextureManager* textureManager, const std::string& file, int health, int numbars,
		float x, float y) :
		textureManager(textureManager), frame(nullptr), totalhealth(health), totalbars(numbars),
		curbars(numbars), margin(Vector2f(x,y)) {

		//get pic for the bar
		this->textureManager->BorrowTexture(file, frame);

		this->sprite = new Sprite(*frame);
		this->sprite->setPosition({ x,y });

		//rect
		this->blackrect.setFillColor(Color::Black);
		this->blackrect.setSize({ 0.f,0.f });
		this->blackrect.setPosition(Vector2f(x, y) + defaultstartpoint);

		//-------SCALE---------//
		this->sprite->setScale(Vector2f(defaultscale, defaultscale));
	}

	~HealthBar() {
		delete sprite;
	}

	void Render(RenderWindow* target) {
		target->draw(*sprite);
		target->draw(blackrect);
	}

	void Update(Entity* character, Vector2f viewCenter, Vector2f viewSize) {
		this->UpdateHealth(character);
		this->UpdatePosition(viewCenter, viewSize);
	}

private:

	void UpdatePosition(Vector2f viewCenter, Vector2f viewSize) {
		Vector2f screenPos = viewCenter - (viewSize / 2.f) + margin; 
		sprite->setPosition(screenPos);
		blackrect.setPosition(screenPos + defaultstartpoint);
	}

	void UpdateHealth(Entity* character) {
		if (character->isInvisible()) { //only update when needed for best performance
			int curhealth = character->getHealth(); //multiple of 10

			//update curbars

			curbars = (this->totalbars) * curhealth / (this->totalhealth);

			//update black rect

			blackrect.setSize(Vector2f(defaultsize.x, (totalbars - curbars) * defaultsize.y));
		}
	}
};