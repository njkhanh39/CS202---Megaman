#ifndef ITEM_H
#define ITEM_H

#include <SFML/Graphics.hpp>
#include "character.h"

enum ItemType {
	HEALTH = 1
};

class Item {
protected:
	TextureManager* textureManager;
	AnimationComponent* animation;
	sf::Sprite* sprite;
	sf::RectangleShape* frame;

	bool taken = false;
	bool done = false;
public:
	Item(TextureManager* textureManager, float x, float y, float sizex, float sizey): 
		sprite(nullptr), textureManager(textureManager), animation(nullptr){

		frame = new sf::RectangleShape(sf::Vector2f(sizex,sizey));
		frame->setPosition({ x,y });

		this->sprite = new Sprite();
		sprite->setPosition({ x,y });

		this->animation = new AnimationComponent(textureManager, sprite);
	}

	virtual ~Item() {
		delete frame;
		if(sprite) delete sprite;
		if (animation) delete animation;
	}

	void Render(sf::RenderTarget* target) {
		//target->draw(*frame);
		target->draw(*sprite);
	}

	virtual void Update(Character* character, float delt) = 0;

	virtual void InnitAnimation() = 0;

	bool IsDone() {
		return this->done;
	}
protected:

	void UpdateTakenState(Character* character) {
		if (this->IsTaken(character)) {
			this->taken = true;
		}
	}

	bool IsTaken(Character* character){
		auto rect = character->getFloatRect();
		auto rect2 = FloatRect(frame->getPosition(), frame->getSize());
		
		return rect.intersects(rect2);
	}
};

class HealthItem : public Item {
private:
	const int hp = 200;
public:
	HealthItem(TextureManager* textureManager, float x, float y): 

	Item(textureManager, x, y, 12, 8){
		InnitAnimation();
	}


	void Update(Character* character, float delt) override {
		this->UpdateTakenState(character);

		animation->Play("Idle", delt);

		if (this->taken && !this->done) {
			//give health to char
 			character->addHealth(hp);
			this->done = true;
		}
	}

private:
	void InnitAnimation() override {
		this->animation->AddAnimation("Idle", "Animation\\Items\\idle_hp.png", 300.f, 0, 0, 2, 0, 12, 8);
	}
};

#endif