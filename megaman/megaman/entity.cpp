#include "entity.h"

Entity::Entity(float x, float y) {
	frame.setPosition({ x, y });
	frame.setSize({ 50.f,50.f });
	frame.setFillColor(Color::Red);
	direction = Direction::Right;

	texture = new Texture();
}

Entity::Entity() {
	frame.setSize({ 50.f,50.f });
	frame.setFillColor(Color::Red);
	direction = Direction::Right;

	texture = new Texture();
}

Entity::~Entity() {
	delete texture;
}

void Entity::Render(RenderWindow* l_window) {
	l_window->draw(frame);
	l_window->draw(sprite);
}

//movements

void Entity::MoveLeft(float delt) {

	direction = Direction::Left;

	//move sprite
	sprite.move(-velocityX * delt, 0);
	//move frame
	/*frame.setPosition({ frame.getPosition().x - (float)speed * delt,frame.getPosition().y });*/
	frame.move(-velocityX * delt, 0);
}

void Entity::MoveRight(float delt) {
	direction = Direction::Right;

	//move sprite
	sprite.move(velocityX * delt, 0);
	//move frame
	frame.move(velocityX * delt, 0);

}

void Entity::Jump(float delt) {
	if (!isJumping) {
		//std::cout << "Jump!" << '\n';

		isJumping = true;
		velocityY = -jumpStrength;
	}
}

void Entity::Shoot() {
	if (!isShooting) {
		isShooting = true;
	}
}


//collision checks

bool Entity::canMoveRight(Obstacle* obs) {
	//if (obs->getLeftMostX() <= getRightMostX()) {
	//	//check for overlapping. Practically, we have
	//	//to check for every existing obstacle

	//	float u = getUpMostY(), v = getDownMostY();
	//	float l = obs->getUpMostY(), r = obs->getDownMostY();

	//	if (v-1.f <= l || u >= r) return true;

	//	

	//	return false;
	//	
	//}
	//return true;

	float lm = getLeftMostX(), rm = getRightMostX();
	float um = obs->getLeftMostX(), vm = obs->getRightMostX();

	//not horizontally intersected
	if (rm < um || lm > vm) return true;

	float lmy = getUpMostY(), rmy = getDownMostY();
	float umy = obs->getUpMostY(), vmy = obs->getDownMostY();

	//if horizontally intersected, check if vertically intersected

	//absolutely no
	if (rmy < umy || lmy > vmy) {
		return true;
	}

	//minor intersection, caused by "jamming" by jumping or falling
	if (rmy - 1.f < umy || lmy + 1.f > vmy) {
		return true;
	}

	//minor horizontal intersection
	if (lm + 1.f > vm) {
		return true;
	}

	return false;

}

bool Entity::canMoveLeft(Obstacle* obs) {
	//if (obs->getRightMostX() >= getLeftMostX()) {
	//	//check for overlapping. Practically, we have
	//	//to check for every existing obstacle

	//	float u = getUpMostY(), v = getDownMostY();
	//	float l = obs->getUpMostY(), r = getDownMostY();

	//	if (v-1.f <= l || u >= r) return true;

	//	return false;
	//}
	//return true;

	float lm = getLeftMostX(), rm = getRightMostX();
	float um = obs->getLeftMostX(), vm = obs->getRightMostX();

	//not horizontally intersected
	if (rm < um || lm > vm) return true;

	float lmy = getUpMostY(), rmy = getDownMostY();
	float umy = obs->getUpMostY(), vmy = obs->getDownMostY();

	//if horizontally intersected, check if vertically intersected

	//absolutely no
	if (rmy < umy || lmy > vmy) {
		return true;
	}

	//minor vertical intersection, caused by "jamming" by jumping or falling
	if (rmy - 1.f < umy || lmy + 1.f > vmy) {
		return true;
	}

	//minor horizontal intersection
	if (rm - 1.f < um) {
		return true;
	}

	return false;
}

bool Entity::canKeepFalling(Obstacle* obs) {
	float lmy = getUpMostY(), rmy = getDownMostY();
	float umy = obs->getUpMostY(), vmy = obs->getDownMostY();

	if (rmy >= umy && (rmy - umy < 1.f || rmy - umy < 3.f)) {
		float l = getLeftMostX(), r = getRightMostX();
		float u = obs->getLeftMostX(), v = obs->getRightMostX();

		if (!(r < u || l > v)) {

			//make the position even

			frame.setPosition({ getLeftMostX(), umy - rmy + lmy });
			sprite.setPosition(frame.getPosition());

			return false;
		}

		return true;
	}
	return true;
}

bool Entity::isHeadBlocked(Obstacle* obs) {
	float lmx = getLeftMostX(), rmx = getRightMostX();
	float umx = obs->getLeftMostX(), vmx = obs->getRightMostX();

	if (!(rmx < umx || lmx > vmx)) {

		float lmy = getUpMostY();
		float umy = obs->getDownMostY();



		if (lmy == umy || (umy > lmy && umy - lmy < 1.f)) {
			return true;
		}
	}

	return false;
}

Vector2f Entity::getUpLeftPosition() {
	return frame.getPosition();
}

Vector2f Entity::getUpRightPosition() {
	auto width = frame.getSize().x;
	return { frame.getPosition().x + width, frame.getPosition().y };
}

Vector2f Entity::getDownLeftPosition() {
	auto height = frame.getSize().y;
	return { frame.getPosition().x, frame.getPosition().y + height };
}

Vector2f Entity::getDownRightPosition() {
	auto width = frame.getSize().x;
	auto height = frame.getSize().y;
	return { frame.getPosition().x + width, frame.getPosition().y + height };
}

float Entity::getRightMostX() {
	return (frame.getPosition().x + frame.getSize().x);
}

float Entity::getLeftMostX() {
	return frame.getPosition().x;
}

float Entity::getUpMostY() {
	return frame.getPosition().y;
}

float Entity::getDownMostY() {
	return (frame.getPosition().y + frame.getSize().y);
}