#include "entity.h"

Entity::Entity(TextureManager* textureManager, float x, float y) {
	frame.setPosition({ x, y });
	frame.setSize({ 50.f,50.f });
	frame.setFillColor(Color::Red);
	direction = Direction::Right;
	
	this->textureManager = textureManager;

	this->InnitNullTextures();

	//animation

	CreateAnimationComponent();

	sprite.setPosition(frame.getPosition() - dilation);
}

Entity::Entity(TextureManager* textureManager) {
	frame.setSize({ 50.f,50.f });
	frame.setFillColor(Color::Red);
	direction = Direction::Right;

	this->textureManager = textureManager;

	this->InnitNullTextures();

	//animation

	CreateAnimationComponent();
	sprite.setPosition(frame.getPosition() - dilation);
}

Entity::Entity(TextureManager* textureManager, float x, float y, float width, float height) {
	frame.setSize({ width, height });
	frame.setPosition({ x, y });
	frame.setFillColor(Color::Red);
	direction = Direction::Right;

	this->textureManager = textureManager;

	this->InnitNullTextures();

	//animation

	CreateAnimationComponent();
	sprite.setPosition(frame.getPosition() - dilation);
}

Entity::~Entity() {
	
	//do not delete textures
	
	//animations



	delete movingAnimation;

	//std::cout << "Destructor of Entity.\n";
}

//void Entity::CreateTextures() {
//	texture_idle_left = new Texture();
//	texture_idle_right = new Texture();
//	texture_jump_left = new Texture();
//	texture_jump_right = new Texture();
//
//	texture_shoot_left = new Texture();
//	texture_shoot_right = new Texture();
//	texture_shoot_jump_left = new Texture();
//	texture_shoot_jump_right = new Texture();
//}

void Entity::InnitNullTextures() {
	texture_idle_left = nullptr;
	texture_idle_right = nullptr;
	texture_jump_left = nullptr;
	texture_jump_right = nullptr;

	texture_shoot_left = nullptr;
	texture_shoot_right = nullptr;
	texture_shoot_jump_left = nullptr;
	texture_shoot_jump_right = nullptr;
}

void Entity::CreateAnimationComponent() {
	//dont forget to create, or its nullptr!

	movingAnimation = new AnimationComponent(textureManager, &sprite);
	idleAnimation = new AnimationComponent(textureManager, &sprite);
}


void Entity::Render(RenderWindow* l_window) {
	l_window->draw(frame);
	l_window->draw(sprite);
}

//movements

void Entity::MoveLeft(float delt) {
	isRight = false;
	isLeft = true;
	direction = Direction::Left;

	//move sprite
	sprite.move(-velocityX * delt, velocityY*delt);
	//move frame
	/*frame.setPosition({ frame.getPosition().x - (float)speed * delt,frame.getPosition().y });*/
	frame.move(-velocityX * delt, velocityY * delt);
}

void Entity::MoveRight(float delt) {
	isLeft = false;
	isRight = true;
	direction = Direction::Right;

	//move sprite
	sprite.move(velocityX * delt, velocityY * delt);
	//move frame
	frame.move(velocityX * delt, velocityY * delt);

}

void Entity::Jump(float delt) {
	if (!isJumping) {
		//std::cout << "Jump!" << '\n';

		isJumping = true;
		velocityY = -jumpStrength;
	}
}

void Entity::PushedUpward(float delt) {
	isJumping = true;
	velocityY = -delt;

	//move sprite
	sprite.move(0, velocityY);
	//move frame
	frame.move(0, velocityY);
}

void Entity::Shoot(float delt) {
	if (!isShooting) {
		isShooting = true;
	}
}

void Entity::TurnLeft() {
	direction = Direction::Left;
	if(texture_idle_left) sprite.setTexture(*texture_idle_left, true);
}

void Entity::TurnRight() {
	direction = Direction::Right;
	if(texture_idle_right) sprite.setTexture(*texture_idle_right, true);
}

void Entity::FaceDirection(Direction dir) {
	direction = dir;
	if (dir == Direction::Left) {
		if(texture_idle_left) sprite.setTexture(*texture_idle_left, true);
	}
	else if(texture_idle_right) sprite.setTexture(*texture_idle_right, true);
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

bool Entity::canMoveRight(Entity* en) {
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
	float um = en->getLeftMostX(), vm = en->getRightMostX();

	//not horizontally intersected
	if (rm < um || lm > vm) return true;

	float lmy = getUpMostY(), rmy = getDownMostY();
	float umy = en->getUpMostY(), vmy = en->getDownMostY();

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

bool Entity::canMoveLeft(Entity* en) {
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
	float um = en->getLeftMostX(), vm = en->getRightMostX();

	//not horizontally intersected
	if (rm < um || lm > vm) return true;

	float lmy = getUpMostY(), rmy = getDownMostY();
	float umy = en->getUpMostY(), vmy = en->getDownMostY();

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
			sprite.setPosition(frame.getPosition() - dilation);

			return false;
		}

		return true;
	}
	return true;
}

bool Entity::canKeepFalling(Entity* en) {
	float lmy = getUpMostY(), rmy = getDownMostY();
	float umy = en->getUpMostY(), vmy = en->getDownMostY();

	if (rmy >= umy && (rmy - umy < 1.f || rmy - umy < 3.f)) {
		float l = getLeftMostX(), r = getRightMostX();
		float u = en->getLeftMostX(), v = en->getRightMostX();

		if (!(r < u || l > v)) {

			//make the position even

			frame.setPosition({ getLeftMostX(), umy - rmy + lmy });
			sprite.setPosition(frame.getPosition() - dilation);

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

bool Entity::isHeadBlocked(Entity* en) {
	float lmx = getLeftMostX(), rmx = getRightMostX();
	float umx = en->getLeftMostX(), vmx = en->getRightMostX();

	if (!(rmx < umx || lmx > vmx)) {

		float lmy = getUpMostY();
		float umy = en->getDownMostY();



		if (lmy == umy || (umy > lmy && umy - lmy < 1.f)) {
			return true;
		}
	}

	return false;
}

//update


//Update performing actions & animations when moving all here
void Entity::UpdateEntity(float delt) {

	//Death
	if (this->IsDead()) return;

	//invisibility 

	if (this->invisible) { //true = is being hurt
		this->invisibleTimer += 100 * delt;

		if (this->invisibleTimer >= this->invisibleMaxTimer) { // we are done with invisibility!
			this->invisible = false;
			this->invisibleTimer = 0;
		}
	}


	//---Movement----

	if (isJumping || fall) {
		//Entity fall



		velocityY += gravity * delt;
		isGrabbing = false;


		//when reaches a limit, fall & isJumping will be turned to false
		//by our canKeepFalling function


		//move our 2 attributes

		sprite.move(0, velocityY * delt);
		frame.setPosition({ frame.getPosition().x, frame.getPosition().y + velocityY * delt });
	}
	else velocityY = 0.0f; //not falling & not jumping = 0 velocityY ?? :D

	//-----ANIMATION UPDATE---------

	//from first if to last if, it follows the animation priority

	if (invisible) { //play the hurt animation
		movingAnimation->Play("HurtAnimation", delt/2); //make animation slower by dividing
	}
	else if (isJumping) {
		if (isShooting) {
			if (direction == Direction::Right && texture_shoot_jump_right) sprite.setTexture(*texture_shoot_jump_right, true);
			if (direction == Direction::Left && texture_shoot_jump_left) sprite.setTexture(*texture_shoot_jump_left, true);
		}
		else {
			if (direction == Direction::Right && texture_jump_right) sprite.setTexture(*texture_jump_right, true);
			if (direction == Direction::Left && texture_jump_left) sprite.setTexture(*texture_jump_left, true);
		}
	}
	else if (isRight) {
		if (isShooting) movingAnimation->Play("MovingShootRight", delt);
		else movingAnimation->Play("MovingRight", delt);
	}
	else if (isLeft) {
		if (isShooting) movingAnimation->Play("MovingShootLeft", delt);
		else movingAnimation->Play("MovingLeft", delt);
	}
	else {
		//Character not jumping, not running

		//this one helpssss, animations can colide (play many at the same time)
		//movingAnimation->Reset();

		//-----PURE SPRITE SET, NOT ANIMATION-----//

		if (isShooting) {
			if (direction == Direction::Left) {
				if (movingAnimation->DoesExist("ShootLeft")) movingAnimation->Play("ShootLeft", delt);
				else if(texture_shoot_left) sprite.setTexture(*texture_shoot_left);
			}

			if (direction == Direction::Right) {
				if (movingAnimation->DoesExist("ShootRight")) movingAnimation->Play("ShootRight", delt);
				else if (texture_shoot_right) sprite.setTexture(*texture_shoot_right);
			}
		}
		else if (direction == Direction::Right && texture_idle_right) {
			sprite.setTexture(*texture_idle_right, true);
		}
		else if (texture_idle_left) {
			sprite.setTexture(*texture_idle_left, true);
		}
		else {
			//plays idle
			this->idleAnimation->Play("Idle", delt);
		}
	}
}

//helper update

bool Entity::IsDead() {
	if (health) return false;
	return true;
}

void Entity::TakeDamage(int damage) {
	if (damage <= 0) return;
	health = std::max(0, health - damage);

	if (health == 0) {
		std::cout << "Entity health is now zero!\n";
		return;
	}

	//we set it off later
	this->invisible = true;
}

//getters

Vector2f Entity::getPosition() {
	return getUpLeftPosition();
}

Vector2f Entity::getCenterPosition() {
	auto width = frame.getSize().x;
	auto height = frame.getSize().y;
	return { frame.getPosition().x + width / 2, frame.getPosition().y + height / 2 };
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

Vector2f Entity::getFrameSize() {
	return frame.getSize();
}

//setters

void Entity::setSpriteScale(float s1, float s2) {
	this->sprite.setScale(s1, s2);
}


void Entity::setSpritePosition(Vector2f pos) {
	sprite.setPosition(pos);
}

void Entity::setSize(Vector2f size) {
	frame.setSize(size);
}

void Entity::setVelocityY(float _y) {
	velocityY = _y;
}

void Entity::setVelocityX(float _x) {
	velocityX = _x;
}

void Entity::setGravity(float _g) {
	this->gravity = _g;
}


void Entity::setPosition(Vector2f pos) {
	frame.setPosition(pos);
	sprite.setPosition(pos - dilation);
}