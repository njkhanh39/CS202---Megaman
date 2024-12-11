#include "obstacle.h"


Obstacle::Obstacle(TextureManager* textureManager, Vector2f pos, Vector2f siz): position(pos), textureManager(textureManager) {
	position = pos;

	frame = new RectangleShape(siz);

	frame->setPosition(position);
	frame->setFillColor(Color(35, 101, 51));

	sprite = new Sprite();
	texture = nullptr;

	sprite->setPosition(position);
}

Obstacle::Obstacle(TextureManager* textureManager, Vector2f pos, Vector2f siz, bool invisible) : 
	invisible(invisible), position(pos), textureManager(textureManager) {
	position = pos;

	frame = new RectangleShape(siz);

	frame->setPosition(position);
	frame->setFillColor(Color(35, 101, 51));

	sprite = new Sprite();
	texture = nullptr;

	sprite->setPosition(position);
}

Obstacle::Obstacle(TextureManager* textureManager, Vector2f pos, Vector2f siz, bool invisible, int damage, bool canClimb) :
	Obstacle(textureManager, pos, siz, invisible) {
	this->damage = damage;
	this->canClimb = canClimb;
}


Obstacle::Obstacle(TextureManager* textureManager, Vector2f pos, const std::string& file) : position(pos), textureManager(textureManager) {
	textureManager->BorrowTexture(file, texture);

	Vector2u tmp = textureManager->getTextureSize(file);
	
	Vector2f siz = { (float)tmp.x, (float)tmp.y };
	
	frame = new RectangleShape(siz);

	frame->setPosition(position);
	frame->setFillColor(Color(35, 101, 51));

	sprite = new Sprite();

	sprite->setPosition(position);

	//set pic
	if(texture) sprite->setTexture(*texture);

}

Obstacle::Obstacle(TextureManager* textureManager, Vector2f pos, const std::string& file, int damage, bool canClimb):
	Obstacle(textureManager, pos, file) {
	this->damage = damage;
	this->canClimb = canClimb;
}

Obstacle::~Obstacle() {
	delete frame;
	//delete texture; we borrow!
	delete sprite;
}

//getters


Vector2f Obstacle::getPosition() {
	return position;
}

Vector2f Obstacle::getSize() {
	return frame->getSize();
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
	frame->setSize(siz);
}

//set color incase we dont load sprite
void Obstacle::setColor(Color col) {
	frame->setFillColor(col);
}


//others

void Obstacle::Render(RenderWindow* l_window) {
	if (!invisible) {
		if (frame) l_window->draw(*frame);
		if (sprite) l_window->draw(*sprite);
	}
}
