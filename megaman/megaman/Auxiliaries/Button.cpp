#include "Button.h"

Button::Button(float x, float y, float width, float height, sf::Font font, std::string text, sf::Color idleColor,
	sf::Color hoverColor, sf::Color activeColor) {

	this->shape.setPosition(sf::Vector2f(x, y));
	this->shape.setSize(sf::Vector2f(width, height));
	

	this->font = font;
	this->text.setFont(this->font);
	this->text.setString(text);
	this->text.setFillColor(sf::Color::Black);
	this->text.setCharacterSize(16);

	//state
	this->buttonState = BTN_IDLE;


	//set pos to mid

	sf::Vector2f rectCenter = shape.getPosition() + shape.getSize() / 2.f;

	// Get the text's local bounds
	sf::FloatRect textBounds = this->text.getLocalBounds();

	// Set the origin of the text to its center
	this->text.setOrigin(textBounds.left + textBounds.width / 2.f,
		textBounds.top + textBounds.height / 2.f);

	// Set the text's position to the rectangle's center
	this->text.setPosition(rectCenter);
	//this->text.setPosition(
	//	this->shape.getPosition().x / 2.f - this->text.getGlobalBounds().width / 2.f,
	//	this->shape.getPosition().y / 2.f - this->text.getGlobalBounds().height / 2.f
	//);

	this->idleColor = idleColor;
	this->hoverColor = hoverColor;
	this->activeColor = activeColor;

	this->shape.setFillColor(this->idleColor);

	
}

Button::~Button() {
	
}

//Accessors
const bool Button::isPressed() const {
	if (this->buttonState == BTN_ACTIVE) {
		return true;
	}
	return false;
}

void Button::Handling(sf::Vector2f mousePos, sf::Event& evt) {
	//idle
	this->buttonState = BTN_IDLE;

	//hover
	if (this->shape.getGlobalBounds().contains(mousePos)) {
		this->buttonState = BTN_HOVER;

		//pressed
		if (evt.type == sf::Event::MouseButtonPressed &&
			evt.mouseButton.button == sf::Mouse::Left) {
			this->buttonState = BTN_ACTIVE;
		}
	}
}

void Button::Update(sf::Vector2f mousePos) {
	

	switch (this->buttonState) {
	case BTN_IDLE:
		this->shape.setFillColor(this->idleColor);
		break;
	case BTN_HOVER:
		this->shape.setFillColor(this->hoverColor);
		break;
	case BTN_ACTIVE:
		this->shape.setFillColor(this->activeColor);
		break;
	default:
		this->shape.setFillColor(sf::Color::Red);
		break;
	}
}

void Button::Render(sf::RenderTarget* target) {
	target->draw(this->shape);
	target->draw(this->text);
}