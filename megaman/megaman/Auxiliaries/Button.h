#ifndef BUTTON_H
#define BUTTON_H

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <sstream>

#include "SFML\System.hpp"
#include "SFML\Window.hpp"
#include "SFML\Graphics.hpp"
#include "SFML\Audio.hpp"
enum button_states {
	BTN_IDLE = 0,
	BTN_HOVER = 1,
	BTN_ACTIVE = 2
};

class Button {
private:
	sf::RectangleShape shape;
	sf::Font font;
	sf::Text text;

	sf::Color idleColor;
	sf::Color hoverColor;
	sf::Color activeColor;


	short unsigned buttonState;
public:
	Button(float x, float y, float width, float height,sf::Font font, std::string text, sf::Color idleColor, 
		sf::Color hoverColor, sf::Color activeColor);

	Button(float x, float y, float width, float height, sf::Font font, int fontsize, std::string text, sf::Color idleColor,
		sf::Color hoverColor, sf::Color activeColor);
	~Button();

	//Accessors
	const bool isPressed() const;

	//Functions

	void SetIdle() {
		this->buttonState = button_states::BTN_IDLE;
	}

	void Handling(sf::Vector2f mousePos, sf::Event& evt);

	void Update(sf::Vector2f mousePos); 

	void Render(sf::RenderTarget* target);

	//setters

	void setPosition(sf::Vector2f v) {
		shape.setPosition(v);

		sf::Vector2f rectCenter = shape.getPosition() + shape.getSize() / 2.f;

		// Get the text's local bounds
		sf::FloatRect textBounds = this->text.getLocalBounds();

		// Set the origin of the text to its center
		this->text.setOrigin(textBounds.left + textBounds.width / 2.f,
			textBounds.top + textBounds.height / 2.f);

		// Set the text's position to the rectangle's center
		this->text.setPosition(rectCenter);
	}
};
#endif