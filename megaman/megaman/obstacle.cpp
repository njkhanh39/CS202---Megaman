#include "obstacle.h"
Obstacle::Obstacle() {
	position = { 50.f,50.f };
	size = position;

	frame = new RectangleShape();

	frame->setPosition(position);
	frame->setSize(size);
	frame->setFillColor(Color(35, 101, 51));

	texture = new Texture();
	sprite = new Sprite();

	sprite->setPosition(position);
}
Obstacle::Obstacle(Vector2f pos, Vector2f siz) {
	position = pos;
	size = siz;

	frame = new RectangleShape();

	frame->setPosition(position);
	frame->setSize(size);
	frame->setFillColor(Color(35, 101, 51));

	texture = new Texture();
	sprite = new Sprite();

	sprite->setPosition(position);
}
Obstacle::~Obstacle() {
	delete frame;
	delete texture;
	delete sprite;
}

//getters


Vector2f Obstacle::getPosition() {
	return position;
}

Vector2f Obstacle::getSize() {
	return size;
}

Vector2f Obstacle::getUpLeftPosition() {
	return frame->getPosition();
}

Vector2f Obstacle::getUpRightPosition() {
	auto width = frame->getSize().x;
	return { frame->getPosition().x + width, frame->getPosition().y };
}

Vector2f Obstacle::getDownLeftPosition() {
	auto height = frame->getSize().y;
	return { frame->getPosition().x, frame->getPosition().y + height };
}

Vector2f Obstacle::getDownRightPosition() {
	auto width = frame->getSize().x;
	auto height = frame->getSize().y;
	return { frame->getPosition().x + width, frame->getPosition().y + height };
}

float Obstacle::getRightMostX() {
	return (frame->getPosition().x + frame->getSize().x);
}

float Obstacle::getLeftMostX() {
	return frame->getPosition().x;
}

float Obstacle::getUpMostY() {
	return frame->getPosition().y;
}

float Obstacle::getDownMostY() {
	return (frame->getPosition().y + frame->getSize().y);
}

//setters

void Obstacle::setPosition(Vector2f pos) {
	position = pos;
	frame->setPosition(pos);
	sprite->setPosition(pos);
}

//can only setSize of sprite is not loaded
void Obstacle::setSize(Vector2f siz) {
	size = siz;
	frame->setSize(siz);
}

//set color incase we dont load sprite
void Obstacle::setColor(Color col) {
	frame->setFillColor(col);
}

void Obstacle::loadImage(const std::string& file) {
	texture->loadFromFile(file);
	sprite->setTexture(*texture);
}

//others

void Obstacle::Render(RenderWindow* l_window) {
	l_window->draw(*frame);
	l_window->draw(*sprite);
}
